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
    Device();
    /// @brief Check, ob Batterie-Strom zur Verfügung steht
    ///        bzw. das Gerät eingeschaltet ist.
    /// @return @em true falls Batteriestrom verfügbar ist, sonst @em false
    bool isBatteryPowerOn();

    /// @brief Check, ob Avionics-Strom Bus 1 zur Verfügung steht
    ///        bzw. das Gerät eingeschaltet ist.
    /// @return @em true falls Strom am Avionicsbus 1 verfügbar ist, sonst @em false
    bool isAvionics1PowerOn();

    /// @brief Check, ob Avionics-Strom Bus 2 zur Verfügung steht
    ///        bzw. das Gerät eingeschaltet ist.
    /// @return @em true falls Strom am Avionicsbus 2 verfügbar ist, sonst @em false
    bool isAvionics2PowerOn();

    /// @brief Check, ob Batterie- und Avionicsstrom (von Bus 1) verfügbar ist.
    /// @return @em true falls Strom verfügbar ist, sonst @em false
    bool isPoweravailable();

private:
    PowerStatusTyp batteryPower;
    PowerStatusTyp avionics1Power;
    PowerStatusTyp avionics2Power;
};