/***************************************************************************************************
 * @file device.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em Device sowie diverse Aufzählungstypen.
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 **************************************************************************************************/

#pragma once

#include <event.hpp>


/***************************************************************************************************
 * @brief @em Device ist die Basisklasse für alle weiteren Geräte.
 *
 * Diese Klasse
 * * stellt u.A. Informationen über die Verfügbarkeit von Strom,
 * * Basisfunktionalitäten, die von den Subklassen geerbt werden können,
 *
 * bereit.
 *
 */
class Device {
public:
    /**
     * @brief Constructor.
     */
    Device();

    /**
     * @brief Verarbeitet die Tastendrücke und Daten.
     *
     */
    void processEvent(EventClass *event) const;


    /**
     * @brief
     *
     * @return true   Power is available and the device is switched on.
     * @return false  Either no power available or the device is switched off.
     */
    bool isDevicePowerAvailable();


    /**
     * @brief Set the device power.
     *
     * @param devicePowerSwitchState State of device's power switch.
     *                                 true   The device is switched on.
     *                                 false  The device is switched off.
     */
    void setDevicePower(bool devicePowerSwitchState);


    /**
     * @brief Check, ob Batterie-Strom zur Verfügung steht
     *        bzw. das Gerät eingeschaltet ist.
     *
     * @return @em true falls Batteriestrom verfügbar ist, sonst @em false
     */
    bool isBatteryPowerOn();


    /**
     * @brief Check, ob Avionics-Strom Bus 1 zur Verfügung steht
     *        bzw. das Gerät eingeschaltet ist.
     *
     * @return @em true falls Strom am Avionicsbus 1 verfügbar ist, sonst @em false
     */
    bool isAvionics1PowerOn();


    /**
     * @brief Check, ob Avionics-Strom Bus 2 zur Verfügung steht
     *        bzw. das Gerät eingeschaltet ist.
     *
     * @return @em true falls Strom am Avionicsbus 2 verfügbar ist, sonst @em false
     *
     */
    bool isAvionics2PowerOn();


    /**
     * @brief Check, ob Batterie- und Avionicsstrom (von Bus 1) verfügbar ist.
     *
     * @return @em true falls Strom verfügbar ist, sonst @em false
     *
     */
    bool isPowerAvailable();


    /**
     * @brief This is an abstract method which has to be implemented in every derived class.
     * @todo als abstrakt und virtuell deklarieren
     *
     */
    void show();


private:
    bool batteryPower;      ///< @em true : Battery power is available, otherwise @em false.
    bool avionics1Power;    ///< @em true : Avionics bus 1 is powered, otherwise @em false.
    bool avionics2Power;    ///< @em true : Avionics bus 2 is powered, otherwise @em false.
    bool devicePower;       ///< Power power must be available and the device must be switched on.
};