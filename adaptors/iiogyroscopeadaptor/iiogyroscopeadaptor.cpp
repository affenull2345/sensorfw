/**
   @file iiogyroscopeadaptor.cpp
   @brief Sensor adaptor for IIO gyroscopes

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

#include "iiogyroscopeadaptor.h"

#define RADIANS_TO_DEGREES 57.2957795

static const char * const iio_anglvel_channels[] = {
    "in_anglvel_x",
    "in_anglvel_y",
    "in_anglvel_z",
    NULL
};

IioGyroscopeAdaptor::IioGyroscopeAdaptor(const QString &id)
    : IioAdaptor(id, "gyroscope", iio_anglvel_channels)
{
    buffer = new DeviceAdaptorRingBuffer<TimedXyzData>(1);
    setAdaptedSensor("gyroscope", "Internal gyroscope coordinates", buffer);

    setDescription("IIO gyroscope");
}

void IioGyroscopeAdaptor::processSample(const qreal *data, quint64 timestamp)
{
    TimedXyzData *d = buffer->nextSlot();

    d->timestamp_ = timestamp;
    d->x_ = data[0] * RADIANS_TO_DEGREES * 1000;
    d->y_ = data[1] * RADIANS_TO_DEGREES * 1000;
    d->z_ = data[2] * RADIANS_TO_DEGREES * 1000;

    buffer->commit();
    buffer->wakeUpReaders();
}
