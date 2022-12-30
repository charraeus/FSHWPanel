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
#include <time.hpp>

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
    // enum ClockModeState {
    //     LT,         ///< Local Time
    //     UT,         ///< Universal Time
    //     ET,         ///< Elapsed Time (seit Avionics on)
    //     FT,         ///< Flight Time
    //     SET_LT,     ///< Set local time directly at the device
    //     SET_UT,     ///< Set UTC directly at the device
    //     SET_ET      ///< Set ET directly at the device
    // }; // vgl. https://www.learncpp.com/cpp-tutorial/nested-types-in-classes/

    /**
     * @brief Construct a new Clock Davtron M803 object
     *
     */
    ClockDavtronM803();


    /**
     * @brief Switch to next clock mode of the lower diplay but skip the SET_xx modes.
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


    void setOatVoltsMode(OatVoltsModeState &OatVoltsMode);
    void setTemperature(int8_t &temperatureC);
    void setPowerState(bool &powerStatus);
    void setAltimeter(float &altimeter);


    /**
     * @brief Update data and process events
     *
     */
    virtual void updateAndProcess();


private:
    TimeClass localTime {999999};           ///< Die lokale Zeit im Format 00HHMMSS.
    TimeClass utc {999999};                 ///< Die UTC im Format 00HHMMSS.
    TimeClass elapsedTime {0};              ///< Die elapsed time im Format 00HHMMSS.
    FlightTimeClass flightTime {9930};         ///< Die Flighttime im Format 00HHMMSS.

    int8_t temperatureC {-1};               ///< Die Temperatur in Grad Celsius.
    bool isTemperatureChanged {true};       ///< @em true if temperatureC has changed, else @em false
    float emfVoltage {99.9};                ///< The EMF-Voltage
    bool isEMFchanged {true};               ///< @em true if EMF voltage has changed, else @em false
    float altimeter {STD_ALTIMETER_inHg};   ///< Luftdruck in inHg.
    const float STD_ALTIMETER_inHg {29.92}; ///< Standardluftdruck in inHg
    bool isAltimeterChanged {true};         ///< @em true if altimeter has changed, else @em false

    OatVoltsModeState oatVoltsMode;         ///< Modus/Status des oberen Displays.
    bool isOatVoltsModeChanged;             ///< @em true if mode is changed, otherwise @em false

    ClockModeState clockMode;               ///< Modus/Status des unteren Displays.
    bool isClockModeChanged;                ///< @em true if mode is changed, otherwise @em false

    uint8_t upperDisplay;                   ///< Upper display id
    uint8_t lowerDisplay;                   ///< Lower display id
    LedMatrixPos LED_TRENNER_1;             ///< Upper divider between hh and mm
    LedMatrixPos LED_TRENNER_2;             ///< Lower divider between hh and mm
    LedMatrixPos LED_LT;                    ///< Led for local time
    LedMatrixPos LED_UT;                    ///< Led for UTC
    LedMatrixPos LED_ET;                    ///< Led for elapsed time
    LedMatrixPos LED_FT;                    ///< Led for flight time


    /**
     * @brief Calculate QNH from altimeter value
    */
    inline float qnh();


    /** @brief Calculate temperature in Fahrenheit from Celsius.
     *
     */
    float temperatureF();


    /** @brief Format the temperature to not more than 3 digits.
     *
     */
    String formatTemperature(const float &temp) const;


    /**
     * @brief Display values on the upper display and switch Leds
     *
     */
    void showUpperDisplay();


    /**
     * @brief Display values on the lower display and switch Leds
     *
     */

    void showLowerDisplay();
};
