/*********************************************************************************************************//**
 * @file device.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Klasse @em Device sowie diverse Aufzählungstypen
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 ************************************************************************************************************/

#include <device.hpp>

/// @brief Device - Konstruktor
Device::Device() {
    batteryPower   = true;
    avionics1Power = true;
    avionics2Power = true;
};


inline bool Device::isBatteryPowerOn() {
    return batteryPower;
};


inline bool Device::isAvionics1PowerOn() {
    return avionics1Power;
};


inline bool Device::isAvionics2PowerOn() {
    return avionics2Power;
};


inline bool Device::isPoweravailable() {
    return isBatteryPowerOn() and isAvionics1PowerOn();
}
