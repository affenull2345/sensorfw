/**
   @file iioaccelerometeradaptor.cpp
   @brief Sensor adaptor for IIO accelerometers

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

#include "iioaccelerometeradaptor.h"

#define GRAVITY 9.80665

static const char * const iio_accel_channels[] = {
    "in_accel_x",
    "in_accel_y",
    "in_accel_z",
    NULL
};

IioAccelerometerAdaptor::IioAccelerometerAdaptor(const QString &id)
    : IioAdaptor(id, "accelerometer", iio_accel_channels)
{
    buffer = new DeviceAdaptorRingBuffer<AccelerationData>(1);
    setAdaptedSensor("accelerometer", "Internal accelerometer coordinates", buffer);

    setDescription("IIO accelerometer");
}

void IioAccelerometerAdaptor::processSample(const qreal *data, quint64 timestamp)
{
    AccelerationData *d = buffer->nextSlot();

    d->timestamp_ = timestamp;
    d->x_ = data[0] * (1000.0 / GRAVITY);
    d->y_ = data[1] * (1000.0 / GRAVITY);
    d->z_ = data[2] * (1000.0 / GRAVITY);

    buffer->commit();
    buffer->wakeUpReaders();
}
