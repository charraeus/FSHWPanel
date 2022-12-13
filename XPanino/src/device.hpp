/*********************************************************************************************************//**
 * @file device.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em Device sowie diverse Aufzählungstypen
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 ************************************************************************************************************/

#pragma once

/// @brief Aufzählungstyp für Powerstatus
enum class PowerStatusTyp {POWER_ON = true, POWER_OFF = false};

/// @brief Device - allgemeine Funktionen für jedes Gerät
class Device {
public:
    bool isPowerOn() { return true; };

private:

};