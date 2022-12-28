/***************************************************************************************************
 * @file m803.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em ClockDavtronM803 sowie diverse Aufzählungstypen.
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 **************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <buffer.hpp>
#include <device.hpp>
#include <ledmatrix.hpp>
#include <Switchmatrix.hpp>

/***************************************************************************************************
 *  Konstantendefinitionen
 */
const char DEVICE_M803[] = "M803";  ///< Kommando, das von X-Plane kommt.
const uint8_t CLOCK_BTN_OAT = 0;
const uint8_t CLOCK_BTN_SEL = 1;
const uint8_t CLOCK_BTN_CTL = 2;


/***************************************************************************************************
 * @brief Aufzählungstyp für die verschiedenen Zeitvarianten, die angezeigt werden.
 *
 */
enum class ClockModeState {
    LT,         ///< Local Time
    UT,         ///< Universal Time
    ET,         ///< Elapsed Time (seit Avionics on)
    FT,         ///< Flight Time
    SET_LT,     ///< Set local time directly at the device
    SET_UT,     ///< Set UTC directly at the device
    SET_ET      ///< Set ET directly at the device
};

/***************************************************************************************************
 * @brief Aufzählungstyp für die nur alternativ möglichen Anzeigen.
 *
 */
enum class OatVoltsModeState {
    EMF,            ///< Show EMF Voltage in upper display.
    FAHRENHEIT,     ///< Show temperature in Fahrenheit in upper display.
    CELSIUS,        ///< Show temperatur in Celsius upper display.
    QNH,            ///< Show current QNH in upper Display.
    ALT             ///< Show current Altimeter in inHG in upper display.
};


/***************************************************************************************************
 * @brief Modell der Uhr Davtron M803.
 *
 * @todo Doku ergänzen
 *
 */
class ClockDavtronM803 : public Device {
public:
    ClockDavtronM803();

    /**
     * @brief Switch to next time mode of the lower diplay but skip the SET_xx modes.
     *
     * @return ClockModeState The new ClockMode.
     */
    ClockModeState toggleClockMode();


    /**
     * @brief Switch to the next mode of the upper display (OAT-VOLTS).
     *
     * @return OatVoltsModeState The new OatVolsMode
     */
    OatVoltsModeState toggleOatVoltsMode();


    /**
     * @brief Set the Switch Event object
     *
     * @param switchId Id of the switch according to the table @em switchId in @em dispatcher.hpp.
     * @param switchStatus Status of the switch.
     */
    void setSwitchEvent(const uint8_t switchId, const uint8_t switchStatus);


    void setLocalTime(uint32_t &localTime);
    void setUtc(uint32_t &utc);
    void setFlightTime(uint32_t &flightTime);
    void setElapsedTime(uint32_t &elapsedTime);
    void setOatVoltsMode(OatVoltsModeState &OatVoltsMode);
    void setTemperature(int8_t &temperatureC);
    void setPowerState(bool &powerStatus);
    void setAltimeter(float &altimeter);


    /**
     * @brief Display the current values in upper and lower display.
     *
     */
    void show();


    /// @brief Die lokale Zeit als String abrufen
    /// @return Lokale Zeit als String zur Anzeige auf 7-Segment-Anzeigen
    char* getLocalTimeDigits();


private:
    uint8_t upperDisplay;                   ///< Upper display id
    uint8_t lowerDisplay;                   ///< Lower display id
    LedMatrixPos LED_TRENNER_1;             ///< Upper divider between hh and mm
    LedMatrixPos LED_TRENNER_2;             ///< Lower divider between hh and mm
    LedMatrixPos LED_LT;                    ///< Led for local time
    LedMatrixPos LED_UT;                    ///< Led for UTC
    LedMatrixPos LED_ET;                    ///< Led for elapsed time
    LedMatrixPos LED_FT;                    ///< Led for flight time
    OatVoltsModeState oatVoltsMode;     ///< Modus/Status des oberen Displays.
    bool isOatVoltsModeChanged;         ///< @em true if mode is changed, otherwise @em false
    ClockModeState clockMode;           ///< Modus/Status des unteren Displays.
    bool isClockModeChanged;            ///< @em true if mode is changed, otherwise @em false
    uint32_t localTime;                 ///< Die lokale Zeit im Format 00HHMMSS.
    uint32_t utc;                       ///< Die UTC im Format 00HHMMSS.
    uint32_t flightTime;                ///< Die Flighttime im Format 00HHMMSS.
    uint32_t elapsedTime;               ///< Die elapsed time im Format 00HHMMSS.
    int8_t temperatureC;                ///< Die Temperatur in Grad Celsius.
    float emfVoltage;                   ///< The EMF-Voltage
    float altimeter;                    ///< Luftdruck in inHg.
    const float STD_ALTIMETER_inHg = 29.92; ///< Standardluftdruck in inHg

    ///< Altimeter in QNH umrechnen
    inline float qnh();

    /// @brief Calculate temperature in Fahrenheit from Celsius.
    float temperatureF();

    /// @brief Format the temperature to not more than 3 digits.
    String formatTemperature(const float &temp);
};
