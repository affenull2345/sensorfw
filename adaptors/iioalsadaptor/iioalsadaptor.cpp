/**
   @file iioalsadaptor.cpp
   @brief Sensor adaptor for IIO ambient light sensors

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

#include "iioalsadaptor.h"

static const char * const iio_illuminance_channels[] = {
    "in_illuminance",
    NULL
};

IioAlsAdaptor::IioAlsAdaptor(const QString &id)
    : IioAdaptor(id, "als", iio_illuminance_channels)
{
    buffer = new DeviceAdaptorRingBuffer<TimedUnsigned>(1);
    setAdaptedSensor("als", "Internal ambient light sensor", buffer);

    setDescription("IIO ambient light sensor");
}

void IioAlsAdaptor::processSample(const qreal *data, quint64 timestamp)
{
    TimedUnsigned *d = buffer->nextSlot();

    d->timestamp_ = timestamp;
    d->value_ = data[0];

    buffer->commit();
    buffer->wakeUpReaders();
}
