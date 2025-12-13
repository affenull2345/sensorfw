/**
   @file iioproximityadaptor.cpp
   @brief Sensor adaptor for IIO proximity sensors

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

#include "iioproximityadaptor.h"
#include <config.h>

#define PROXIMITY_NEAR_VALUE 0
#define PROXIMITY_FAR_VALUE 100

static const char * const iio_proximity_channels[] = {
    "in_proximity",
    NULL
};

IioProximityAdaptor::IioProximityAdaptor(const QString &id)
    : IioAdaptor(id, "proximity", iio_proximity_channels)
{
    buffer = new DeviceAdaptorRingBuffer<ProximityData>(1);
    setAdaptedSensor("proximity", "Internal proximity sensor", buffer);

    setDescription("IIO proximity sensor");

    m_threshold = SensorFrameworkConfig::configuration()->value<int>("proximity/threshold", 250);
}

void IioProximityAdaptor::processSample(const qreal *data, quint64 timestamp)
{
    ProximityData *d = buffer->nextSlot();

    d->timestamp_ = timestamp;
    d->withinProximity_ = data[0] >= m_threshold;
    d->value_ = d->withinProximity_ ? PROXIMITY_NEAR_VALUE : PROXIMITY_FAR_VALUE;

    buffer->commit();
    buffer->wakeUpReaders();
}
