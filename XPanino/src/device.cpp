/***************************************************************************************************
 * @file device.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Basisklasse @em Device sowie diverse Aufzählungstypen.
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 **************************************************************************************************/

#include <device.hpp>

/**
 * @brief Construct a new Device:: Device object
 *
 */
Device::Device() {
    batteryPower   = true;
    avionics1Power = true;
    avionics2Power = true;
    devicePower = true;

};


void Device::processEvent(EventClass *event) const {
    #ifdef DEBUG
    Serial.println(F("***Device.processEvent()"));
    if (event != nullptr) {
        event->printEvent();
    } else {
        Serial.println(F("Err: nullptr statt event angekommen."));
    }
    Serial.println(F("***End Device.processEvent()"));
    #endif
}


bool Device::isDevicePowerAvailable() {
    return devicePower and isPowerAvailable();
}


void Device::setDevicePower(bool devicePowerSwitchState) {
    devicePower = devicePowerSwitchState;
}

inline bool Device::isBatteryPowerOn() {
    return batteryPower;
};


inline bool Device::isAvionics1PowerOn() {
    return avionics1Power;
};


inline bool Device::isAvionics2PowerOn() {
    return avionics2Power;
};


inline bool Device::isPowerAvailable() {
    return isBatteryPowerOn() and isAvionics1PowerOn();
}


void Device::show() {
    ;
}