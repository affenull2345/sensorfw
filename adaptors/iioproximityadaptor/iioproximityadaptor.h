/**
   @file iioproximityadaptor.h
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

#ifndef IIOPROXIMITYADAPTOR_H
#define IIOPROXIMITYADAPTOR_H

#include <datatypes/orientationdata.h>
#include <deviceadaptorringbuffer.h>
#include <iioadaptor.h>

/**
 * @brief Adaptor for Industrial I/O proximity sensors.
 */
class IioProximityAdaptor : public IioAdaptor
{
    Q_OBJECT

public:
    static DeviceAdaptor *factoryMethod(const QString &id) {
        return new IioProximityAdaptor(id);
    }

    IioProximityAdaptor(const QString &id);

protected:
    void processSample(const qreal *data, quint64 timestamp);

private:
    DeviceAdaptorRingBuffer<ProximityData>* buffer;
    unsigned int m_threshold;
};

#endif
