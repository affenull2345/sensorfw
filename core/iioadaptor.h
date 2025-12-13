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

#ifndef IIOADAPTOR_H
#define IIOADAPTOR_H

#include <sysfsadaptor.h>

/**
 * @brief Base class for Industrial I/O sensor adaptors.
 *
 * This adaptor uses the sysfs interface in /sys/bus/iio/devices/iio:deviceX/
 * and supports two modes:
 * <ul>
 *   <li><tt>SysfsAdaptor::IntervalMode</tt> - Poll raw values with a constant interval.</li>
 *   <li><tt>SysfsAdaptor::SelectMode</tt>   - Read from /dev/iio:deviceX when data is available (buffer mode).</li>
 * </ul>
 */
class IioAdaptor : public SysfsAdaptor
{
    Q_OBJECT

    struct IioChannel {
        QByteArray name;
        int index;
        int scanIndex;
        int bytes;
        qreal scale;

        bool operator<(const IioChannel &other) const
        {
            return scanIndex < other.scanIndex;
        }
    };

public:
    IioAdaptor(const QString &id, const char *name, const char* const *channelNames);
    ~IioAdaptor();

    virtual void init();

    virtual bool startSensor();
    virtual void stopSensor();

    virtual bool resume();

protected:
    virtual void processSample(int fileId, int fd);

    virtual void processSample(const qreal *data, quint64 timestamp) = 0;

private:
    bool findDevice(const QString &name);

    QByteArray m_iioDevName;
    QByteArray m_devicePath;

    int m_channelCount;
    IioChannel *m_channels; /* [m_channelCount] */
    qreal *m_data; /* [m_channelCount] */

    QByteArray m_buffer;
};

#endif
