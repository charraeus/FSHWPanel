/**************************************************************************************************
 * @file xpdr.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em ClockDavtronM803 sowie diverse Aufzählungstypen
 * @version 0.1
 * @date 2017-11-12
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
**************************************************************************************************/

#ifndef xpdr_hpp
#define xpdr_hpp

#include <Arduino.h>

// Status-Aufzählungstypen
/**
 * @brief Aufzählungstyp für Powerstatus
 * 
 */
enum StatusPower {POWER_OFF, POWER_ON};

/**
 * @brief Aufzählungstyp für die nur alternativ möglichen Anzeigen.
 * 
 */
enum StatusOatVolts {
    OAT_VOLTAGE, 
    OAT_FAHRENHEIT, 
    OAT_CELSIUS
};

/**
 * @brief Aufzählungstyp für die verschiedenen Zeitvarianten, die angezeigt werden.
 * 
 */
enum StatusTime {
    TIME_LT,            // Als Uhrzeit ist Local Time eingestellt/angezeigt
    TIME_UT,            // Als Uhrzeit ist UTC eingestellt/angezeigt 
    TIME_ET,            // Als Uhrzeit wird die Elapsed Time seit 
    TIME_FT
};

/**
 * @brief Aufzählungstyp für die möglichen Events, die verarbeitet werden müssen.
 * 
 */
enum Event {
    BTN_OAT_ON,         // Der Taster OAT/VOLTS wurde gedrückt
    BTN_SEL_ON,         // der Taster SELECT wurde gedrückt
    BTN_SEL_OFF,        // der Taster SELECT wurde wieder losgelassen
    BTN_CTL_ON,         // der Taster CONTROL wurde gedrückt
    BTN_CTL_LON,        // der Taster CONTROL wurde mindestens 3 Sekunden lang gedrückt
    XP_TIME,            // Neue Uhrzeit vom X-Plane empfangen
    XP_TEMPERATURE      // Neue Temperatur vom X-Plane empfangen
};

/**
 * @brief Modell der Uhr Davtron M803
 * @todo Doku ergänzen
 * 
 */
class ClockDavtronM803 {
public:
    // Initialisiert die Uhr
    void init();
    // Verarbeitet die Tastendrücke und Daten
    void process();
    Event event(const String &eventString);

    // Die local Time in UTC umrechnen
    uint32_t utc(const uint32_t localTime);
    
private:
    StatusPower statusPower {POWER_OFF};
    StatusOatVolts statusOatVolts {OAT_CELSIUS};
    StatusTime statusTime {TIME_LT};
    bool batteryOn {false};
    bool alternatorOn {false};
    uint32_t localTime {0};                         ///< Die lokale Zeit im Format 00HHMMSS checken wies vom Flusi kommt
    uint32_t flightTime {0};                        ///< Die Flighttime im Format 00HHMMSS checken wies vom Flusi kommt
    uint32_t elapsedTime {0};                       ///< Die elapsed time im Format 00HHMMSS
    uint8_t temperatureC {0};                       ///< Die Temperatur in Grad Celsius  checken wies vom Flus kommt
};



#endif /* xpdr_hpp */
