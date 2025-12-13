/**
   @file iioadaptor.h
   @brief Base class for IIO sensor adaptors

   <p>
   Copyright (C) 2025 Affe Null <affenull2345@gmail.com>

   This file is part of Sensord.

   Sensord is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Sensord is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Sensord.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */

#include "iioadaptor.h"
#include <config.h>
#include <datatypes/utils.h>
#include <unistd.h>

#include <libudev.h>

IioAdaptor::IioAdaptor(const QString &id, const char *name, const char * const *channelNames)
    : SysfsAdaptor(id, SysfsAdaptor::IntervalMode, true)
{
    m_channelCount = 0;
    for (const char * const *p = channelNames; *p; p++)
        m_channelCount++;

    qDebug() << id << "channel count:" << m_channelCount;

    m_data = new qreal[m_channelCount];
    m_channels = new IioChannel[m_channelCount];

    for (int i = 0; i < m_channelCount; i++) {
        m_channels[i].name = channelNames[i];

        qDebug() << id << "channel:" << m_channels[i].name;
    }

    const QString deviceName = SensorFrameworkConfig::configuration()->value<QString>(QString(name) + "/input_match");
    for (int retries = 10; ;) {
        if (findDevice(deviceName))
            break;

        if (--retries < 0) {
            qCWarning(lcSensorFw) << name << "device" << deviceName << "not found";
            setValid(false);
            return;
        }

        qCInfo(lcSensorFw) << "Waiting for" << name << "device" << deviceName;
        QThread::msleep(2000);
    }

    qDebug() << id << "iio device:" << m_iioDevName;
    qDebug() << id << "path:" << m_devicePath;
}

IioAdaptor::~IioAdaptor()
{
    delete[] m_channels;
    delete[] m_data;
}

bool IioAdaptor::findDevice(const QString &name)
{
    struct udev *context;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *entry;
    bool found = false;

    context = udev_new();
    enumerate = udev_enumerate_new(context);

    udev_enumerate_add_match_subsystem(enumerate, "iio");
    udev_enumerate_scan_devices(enumerate);

    entry = udev_enumerate_get_list_entry(enumerate);
    while (entry) {
        const char *syspath = udev_list_entry_get_name(entry);
        struct udev_device *device;

        device = udev_device_new_from_syspath(context, syspath);

        if (name == udev_device_get_sysattr_value(device, "name")) {
            m_iioDevName = udev_device_get_sysname(device);
            m_devicePath = syspath;
            m_devicePath += "/";
            found = true;

            udev_device_unref(device);
            break;
        }

        udev_device_unref(device);

        entry = udev_list_entry_get_next(entry);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(context);

    return found;
}

void IioAdaptor::init()
{
    SysfsAdaptor::init();

    QByteArray scanBase = m_devicePath + "scan_elements/";
    bool ok;

    for (int i = 0; i < m_channelCount; i++) {
        IioChannel &ch = m_channels[i];

        /* Preserve the original index even when the list is sorted */
        ch.index = i;

        /* Try stripping name, e.g. in_accel_x -> in_accel */
        QByteArray baseName = ch.name;
        int pos = baseName.indexOf('_', 3);
        if (pos >= 0)
            baseName.truncate(pos);

        QByteArray scale = readFromFile(m_devicePath + ch.name + "_scale");
        if (scale.endsWith('\n'))
            scale.chop(1);
        ch.scale = scale.toDouble(&ok);
        if (!ok) {
            scale = readFromFile(m_devicePath + baseName + "_scale");
            if (scale.endsWith('\n'))
                scale.chop(1);
            ch.scale = scale.toDouble(&ok);
            if (!ok)
                qCWarning(lcSensorFw) << id() << "Cannot parse" << ch.name << "scale:" << scale;
        }

        if (mode() == SysfsAdaptor::IntervalMode) {
            addPath(m_devicePath + baseName + "_raw", i);
        } else {
            ok = true;

            QByteArray type = readFromFile(scanBase + ch.name + "_type");
            if (type == "le:s16/16>>0\n") {
                ch.bytes = 2;
            } else if (type == "le:s32/32>>0\n") {
                ch.bytes = 4;
            } else if (type == "le:s64/64>>0\n") {
                ch.bytes = 8;
            } else {
                qCWarning(lcSensorFw) << id() << "Unsupported type" << type << "for channel" << ch.name;
                ok = false;
            }

            /*
             * Read the scan index, which determines the order of the channels
             * in the buffer. This is not necessarily equal to the real index.
             */
            if (ok) {
                QByteArray index = readFromFile(scanBase + ch.name + "_index");
                if (index.endsWith('\n'))
                    index.chop(1);

                ch.scanIndex = index.toInt(&ok);
                if (!ok)
                    qCWarning(lcSensorFw) << id() << "Cannot parse" << ch.name << "index:" << index;
            }

            if (ok) {
                writeToFile(scanBase + ch.name + "_en", "1");
            }
        }
    }

    if (mode() != SysfsAdaptor::IntervalMode) {
        addPath("/dev/" + QString::fromLatin1(m_iioDevName));

        /* Sort channels into their buffer order */
        std::sort(m_channels, m_channels + m_channelCount);

        /* Compute the size of the buffer, taking alignment into account */
        int totalBytes = 0;
        for (int i = 0; i < m_channelCount; i++) {
            IioChannel &ch = m_channels[i];

            totalBytes = ((totalBytes - 1) | (ch.bytes - 1)) + 1 + ch.bytes;
        }

        m_buffer.resize(totalBytes);
        qDebug() << id() << "buffer size:" << totalBytes;
    }
}

bool IioAdaptor::startSensor()
{
    if (!SysfsAdaptor::startSensor())
        return false;

    if (mode() != SysfsAdaptor::IntervalMode)
        writeToFile(m_devicePath + "buffer/enable", "1");

    return true;
}

void IioAdaptor::stopSensor()
{
    if (mode() != SysfsAdaptor::IntervalMode)
        writeToFile(m_devicePath + "buffer/enable", "0");

    SysfsAdaptor::stopSensor();
}

bool IioAdaptor::resume()
{
    if (!SysfsAdaptor::resume())
        return false;

    getAdaptedSensor()->setIsRunning(true);

    if (mode() != SysfsAdaptor::IntervalMode)
        writeToFile(m_devicePath + "buffer/enable", "1");

    return true;
}

void IioAdaptor::processSample(int fileId, int fd)
{
    if (mode() == SysfsAdaptor::IntervalMode) {
        /* TODO: implement interval mode */
    } else {
        /* TODO: obtain timestamp from buffer */
        quint64 timestamp = Utils::getTimeStamp();

        ssize_t ret = read(fd, m_buffer.data(), m_buffer.size());
        if (ret == -1) {
            qCWarning(lcSensorFw) << id() << "Failed to read from buffer:" << strerror(errno);
            QThread::msleep(1000);
            return;
        }
        if (ret != m_buffer.size()) {
            qCWarning(lcSensorFw) << id() << "Read" << ret << "instead of" << m_buffer.size() << "bytes";
            QThread::msleep(1000);
            return;
        }

        char *buf = m_buffer.data();
        int pos = 0;
        for (int i = 0; i < m_channelCount; i++) {
            IioChannel &ch = m_channels[i];
            qreal val;

            switch (ch.bytes) {
            case 2:
                val = *reinterpret_cast<int16_t*>(buf + pos) * ch.scale;
                break;
            case 4:
                val = *reinterpret_cast<int32_t*>(buf + pos) * ch.scale;
                break;
            case 8:
                val = *reinterpret_cast<int64_t*>(buf + pos) * ch.scale;
                break;
            default:
                val = 0.0;
                break;
            }

            m_data[ch.index] = val;

            pos = ((pos - 1) | (ch.bytes - 1)) + 1 + ch.bytes;
        }

        processSample(m_data, timestamp);
    }
}
