/*********************************************************************************************************//**
 * @file xpdr.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klassen @em Transponder @em ClockDavtronM803 sowie diverse Aufzählungstypen
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <buffer.hpp>
#include <Switchmatrix.hpp>
#include <ledmatrix.hpp>

const char DEVICE_XPDR[] = "XPDR";

/**************************************************************************************************
 * Status-Aufzählungstpyen
 *
 **************************************************************************************************/

/**************************************************************************************************
 * @brief Modell des Transponders KT76C
 * @todo Doku ergänzen
 *
 **************************************************************************************************/
class TransponderKT76C {
public:
    void process(EventClass event);

private:

};