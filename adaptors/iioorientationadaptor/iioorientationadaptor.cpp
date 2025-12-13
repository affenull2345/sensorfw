/**
   @file iioorientationadaptor.cpp
   @brief Sensor adaptor for IIO orientation (compass) sensors

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

#include "iioorientationadaptor.h"

static const char * const iio_orientation_channels[] = {
    "in_rot_from_north_magnetic_tilt_comp",
    NULL
};

IioOrientationAdaptor::IioOrientationAdaptor(const QString &id)
    : IioAdaptor(id, "orientation", iio_orientation_channels)
{
    buffer = new DeviceAdaptorRingBuffer<CompassData>(1);
    setAdaptedSensor("orientation", "Internal orientation coordinates", buffer);

    setDescription("IIO orientation");
}

void IioOrientationAdaptor::processSample(const qreal *data, quint64 timestamp)
{
    CompassData *d = buffer->nextSlot();

    d->timestamp_ = timestamp;
    d->degrees_ = data[0];
    d->rawDegrees_ = data[0];

    buffer->commit();
    buffer->wakeUpReaders();
}
