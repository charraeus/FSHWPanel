/**************************************************************************************************
 * @file switch.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Implementierung der Klasse @em Switch
 * @version 0.1
 * @date 2020-04-10
 * 
 * Copyright © 2017 - 2020 Christian Harraeus. All rights reserved.
 * 
**************************************************************************************************/

#include "switch.hpp"

const unsigned long LONG_ON = 3000;     ///< Dauer, ab wann ein Schalter lange eingeschaltet ist (3000 Millisekunden)


/******************************************************************************/
void Switch::setOn() {
    status = LOW;
    changed = true;
    onTimeStart = millis();     // Einschalt"zeit" merken
    longOnSent = false;
    longOn = false;
    onTime = 0;
}


/******************************************************************************/
void Switch::setOff() {
    status = HIGH;
    changed = true;
    onTime = calcTimeDiff(onTimeStart, millis());   // Differenz zwischen Ein- und Ausschalt"zeit" merken
    longOnSent = true;
    longOn = false;
}


/******************************************************************************/
void Switch::checkLongOn() {
    longOn = (onTime >= LONG_ON);
    if (longOn && (! longOnSent)) {
        changed = true;
    }
}


/******************************************************************************/
void Switch::updateOnTime(const unsigned long &newOnTime) {
    onTime = calcTimeDiff(onTimeStart, newOnTime);
}


/******************************************************************************/
void Switch::transmitStatus() {
    char charsToSend[100];;

    strcpy(charsToSend, "XPDR BTN ");
    strcat(charsToSend, switchName); 
    strcat(charsToSend, " ");
    if ((! longOnSent) && longOn) {
        // wenn der Schalter lang gedrückt ist und dieser Lang-Gedrückt-Status noch
        // nicht übertragen wurde, den Status "LON" (Long on) übertragen.
        longOnSent = true;
        changed = false;
        strcat(charsToSend, "LON");
    } else {
        // Den An-/Aus-Status des Schalters übertragen.
        strcat(charsToSend, (getStatus() == LOW) ? "ON" : "OFF");
    }
    #ifdef DEBUG
    Serial.println(charsToSend);
    ///@todo Switch::transmitStatus ausprogrammieren.
    #endif
}

// ab hier beginnen die privaten Methoden

/******************************************************************************/
unsigned long Switch::calcTimeDiff(const unsigned long &onTime, const unsigned long &offTime) {
    const unsigned long maxLong = 4294967295;  /** @todo besser sizeOf(unsigned long) nehmen */
    // Einschaltzeit ausrechnen. millis() kann überlaufen; das muss berücksichtigt werden
    if (offTime >= onTime) {
        // millis() ist nicht übergelaufen (Normalfall)
        // Zeit in Millisekunden berechnen, die der Schalter an wahr
        return offTime - onTime;
    } else {
        // millis() ist übergelaufen (passiert nach 50 Tagen)
        // onTimeStart = Zeit seit Überlauf + Maximalwert von unsigned long - Einschalt"zeit"
        return maxLong - onTime + offTime;
    }
}

