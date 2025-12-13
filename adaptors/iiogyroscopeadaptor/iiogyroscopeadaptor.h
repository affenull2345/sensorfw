/**
   @file iiogyroscopeadaptor.h
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

#ifndef IIOGYROSCOPEADAPTOR_H
#define IIOGYROSCOPEADAPTOR_H

#include <datatypes/orientationdata.h>
#include <deviceadaptorringbuffer.h>
#include <iioadaptor.h>

/**
 * @brief Adaptor for Industrial I/O gyroscopes.
 */
class IioGyroscopeAdaptor : public IioAdaptor
{
    Q_OBJECT

public:
    static DeviceAdaptor *factoryMethod(const QString &id) {
        return new IioGyroscopeAdaptor(id);
    }

    IioGyroscopeAdaptor(const QString &id);

protected:
    void processSample(const qreal *data, quint64 timestamp);

private:
    DeviceAdaptorRingBuffer<TimedXyzData>* buffer;
};

#endif
