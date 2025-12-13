/**
   @file iiomagnetometeradaptor.cpp
   @brief Sensor adaptor for IIO magnetometers

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

#include "iiomagnetometeradaptor.h"

static const char * const iio_magn_channels[] = {
    "in_magn_x",
    "in_magn_y",
    "in_magn_z",
    NULL
};

IioMagnetometerAdaptor::IioMagnetometerAdaptor(const QString &id)
    : IioAdaptor(id, "magnetometer", iio_magn_channels)
{
    buffer = new DeviceAdaptorRingBuffer<CalibratedMagneticFieldData>(1);
    setAdaptedSensor("magnetometer", "Internal magnetometer coordinates", buffer);

    setDescription("IIO magnetometer");
}

void IioMagnetometerAdaptor::processSample(const qreal *data, quint64 timestamp)
{
    CalibratedMagneticFieldData *d = buffer->nextSlot();

    d->timestamp_ = timestamp;
    d->x_ = data[0] * 1000;
    d->y_ = data[1] * 1000;
    d->z_ = data[2] * 1000;
    d->rx_ = data[0] * 1000;
    d->ry_ = data[1] * 1000;
    d->rz_ = data[2] * 1000;

    buffer->commit();
    buffer->wakeUpReaders();
}
