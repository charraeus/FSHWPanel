/*********************************************************************************************************//**
 * @file m803.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klassen @em Transponder @em ClockDavtronM803 sowie diverse Aufzählungstypen
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <Switchmatrix.hpp>
#include <ledmatrix.hpp>
#include <device.hpp>
#include <parser.hpp>

const char DEVICE_M803[] = "M803";

enum class Event {
    BTN_OAT_ON,         // Der Taster OAT/VOLTS wurde gedrückt
    BTN_SEL_ON,         // der Taster SELECT wurde gedrückt
    BTN_SEL_OFF,        // der Taster SELECT wurde wieder losgelassen
    BTN_CTL_ON,         // der Taster CONTROL wurde gedrückt
    BTN_CTL_LON,        // der Taster CONTROL wurde mindestens 3 Sekunden lang gedrückt
    XP_TIME,            // Neue Uhrzeit vom X-Plane empfangen
    XP_TEMPERATURE      // Neue Temperatur vom X-Plane empfangen
};


/**************************************************************************************************
 * Status-Aufzählungstpyen
 *
 **************************************************************************************************/

/**
 * @brief Aufzählungstyp für die nur alternativ möglichen Anzeigen.
 *
 */
enum class OatVoltsModeTyp {
    OAT_VOLTAGE,        ///< Im oberen Display wird die Spannung angezeigt.
    OAT_FAHRENHEIT,     ///< Im oberen Display wird die Temperatur in Fahrenheit angezeigt.
    OAT_CELSIUS,        ///< Im oberen Display wird die Temperatur in Celsius angezeigt.
    OAT_QNH             ///< Im oberen Display wird das akt. QNH angezeigt.
};

/**
 * @brief Aufzählungstyp für die verschiedenen Zeitvarianten, die angezeigt werden.
 *
 */
enum class TimeModeTyp {
    TIME_LT,            ///< Local Time
    TIME_UT,            ///< Universal Time
    TIME_ET,            ///< Elapsed Time (seit Avionics on)
    TIME_FT             ///< Flight Time
};

/**
 * @brief Aufzählungstyp für die möglichen Events, die verarbeitet werden müssen.
 *
 */
enum class m803Event {
    BTN_OAT_ON,         // Der Taster OAT/VOLTS wurde gedrückt
    BTN_SEL_ON,         // der Taster SELECT wurde gedrückt
    BTN_SEL_OFF,        // der Taster SELECT wurde wieder losgelassen
    BTN_CTL_ON,         // der Taster CONTROL wurde gedrückt
    BTN_CTL_LON,        // der Taster CONTROL wurde mindestens 3 Sekunden lang gedrückt
    XP_TIME,            // Neue Uhrzeit vom X-Plane empfangen
    XP_TEMPERATURE      // Neue Temperatur vom X-Plane empfangen
};

/**************************************************************************************************
 * @brief Modell der Uhr Davtron M803
 * @todo Doku ergänzen
 *
 **************************************************************************************************/
class ClockDavtronM803 : public Device {
public:
    void setTimeMode(TimeModeTyp &timeMode);
    void setLocalTime(uint32_t &localTime);
    void setUtc(uint32_t &utc);
    void setFlightTime(uint32_t &flightTime);
    void setElapsedTime(uint32_t &elapsedTime);
    void setOatVoltsMode(OatVoltsModeTyp &OatVoltsMode);
    void setTemperature(int8_t &temperatureC);
    void setPowerStatus(bool &powerStatus);
    void setAltimeter(float &altimeter);

    /// @brief Die lokale Zeit als String abrufen
    /// @return Lokale Zeit als String zur Anzeige auf 7-Segment-Anzeigen
    char* getLocalTimeDigits();

    /// @brief Durch die verschiedenen Modi des oberen Displays schalten.
    /// @return Der jetzt aktuelle Status der Anzeige
    OatVoltsModeTyp toggleOatVoltsMode();

    /// @brief Durch die verschiedenen Zeit-Modi des unteren Displays schalten.
    /// @return Der jetzt aktuelle Zeitmodus
    TimeModeTyp toggleTimeMode();

    // Verarbeitet die Tastendrücke und Daten
    void process(EventClass event);

private:
    const float STD_ALTIMETER_inHg = 29.92; ///< Standardluftdruck in inHg

    TimeModeTyp timeMode{TimeModeTyp::TIME_LT};  ///< Initial wird die lokale Zeit im unteren Display angezeigt.
    uint32_t localTime{0};      ///< Die lokale Zeit im Format 00HHMMSS checken wies vom Flusi kommt
    uint32_t utc{0};            ///< Die UTC im Format 00HHMMSS checken wies vom Flusi kommt
    uint32_t flightTime{0};     ///< Die Flighttime im Format 00HHMMSS checken wies vom Flusi kommt
    uint32_t elapsedTime{0};    ///< Die elapsed time im Format 00HHMMSS
    OatVoltsModeTyp oatVoltsMode = OatVoltsModeTyp::OAT_CELSIUS;  ///< Initial wird die Temeperatur in Celsius im oberen Display angezeigt.
    int8_t temperatureC{0};     ///< Die Temperatur in Grad Celsius  checken wie's vom Flusi kommt
    bool powerStatus = false;  ///< Initial ist die Power off.
    float altimeter{STD_ALTIMETER_inHg};     ///< Luftdruck in inHg

    // Altimeter in QNH umrechnen
    inline float qnh(float &altimeter);
};
