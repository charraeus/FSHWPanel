/*********************************************************************************************************//**
 * @file switch.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Implementierung der Klasse @em Switch
 * @version 0.1
 * @date 2020-04-10
 *
 * Copyright © 2017 - 2020 Christian Harraeus. All rights reserved.
************************************************************************************************************/

#include <buffer.hpp>
#include <event.hpp>
#include <switch.hpp>

/// @brief Dauer, ab wann ein Schalter lange eingeschaltet ist (3000 Millisekunden)
const unsigned long LONG_ON = 3000;


/// @brief Logischen Schalter auf "eingeschaltet" setzen
void Switch::setOn() {
    status = LOW;
    changed = true;
    switchPressTime = millis();     // Einschalt"zeit" merken
    longOnSent = false;
    longOn = false;
    onTime = 0;
}


void Switch::setOff() {
    status = HIGH;
    changed = true;
    onTime = calcTimeDiff(switchPressTime, millis());   // Differenz zwischen Ein- und Ausschalt"zeit" merken
    longOnSent = true;
    longOn = false;
}


void Switch::checkLongOn() {
    longOn = (onTime >= LONG_ON);
    if (longOn && (! longOnSent)) {
        changed = true;
    }
}


void Switch::updateOnTime(const unsigned long &newOnTime) {
    onTime = calcTimeDiff(switchPressTime, newOnTime);
}


void Switch::transmitStatus(uint8_t &row, uint8_t &col) {
    // switchState = 0: Switch is off
    // switchState = 1: Switch is on
    // switchState = 2: Switch is long on
    uint8_t switchState = 0;
    if ((! longOnSent) && longOn) {
        // wenn der Schalter lang gedrückt ist und dieser Lang-Gedrückt-Status noch
        // nicht übertragen wurde, den Status "LON" (Long on) übertragen.
        longOnSent = true;
        changed = false;
        switchState = 2;
    } else {
        // Den An- (1) / Aus- (0) Status des Schalters übertragen.
        switchState = ((getStatus() == LOW) ? 1 : 0);
    }
    // Ereignis senden
    // Diese Methode muss überschrieben werden.
    transmit(row, col, switchState);
}


void Switch::transmit(uint8_t &row, uint8_t &col, uint8_t &switchState) {
    // switchState = 0: Switch is off
    // switchState = 1: Switch is on
    // switchState = 2: Switch is long on

    // c-string with data to be sent (e.g. via the serial port)
    char charsToSend[MAX_BUFFER_LENGTH] = "S;S;";

    // temp memory for typecast int --> c-string
    char charRowCol[MAX_PARA_LENGTH * 2] = "";

    // set charsToSend according to value of data
    if (switchState == 2) {
        strcat(charsToSend, "LON;");
    } else {
        strcat(charsToSend, ((switchState == 1) ? "ON;" : "OFF;"));
    }
    snprintf (charRowCol, MAX_PARA_LENGTH * 2, "%u;%u", row, col);
    strcat(charsToSend, charRowCol);
    Serial.println(charsToSend);
}


/*********************************************************************************************************//**
 * ab hier die privaten Methoden
*************************************************************************************************************/

/**
 * @brief Differenz zweier Zeitstempel berechnen.
 * Berechnet die Differenz zweier Zeitstempel und gibt diese als Ergebnis
 * zurück. Ein eventueller Überlauf (passiert ca. nach 50 Tagen) wird berücksichtigt.
 *
 * @param onTime Einschaltzeitstempel in Millisekunden, z.B. millis().\n
 *               Dieser Parameter ist i.d.R. der ältere Zeitstempel.
 * @param offTime Ausschaltzeitstempel in Millisekunden, z.B. millis().\n
 *                Dieser Parameter ist i.d.R. der neuere Zeitstempel.
 *
 * @return unsigned long Zeitdifferenz in Millisekunden
 */
unsigned long Switch::calcTimeDiff(const unsigned long &onTime, const unsigned long &offTime) {
    const unsigned long maxLong = 4294967295;  /// @todo besser sizeOf(unsigned long) nehmen */
    // Einschaltzeit ausrechnen. millis() kann überlaufen; das muss berücksichtigt werden
    if (offTime >= onTime) {
        // millis() ist nicht übergelaufen (Normalfall)
        // Zeit in Millisekunden berechnen, die der Schalter an wahr
        return offTime - onTime;
    }
    // millis() ist übergelaufen (passiert nach 50 Tagen)
    // onTimeStart = Zeit seit Überlauf + Maximalwert von unsigned long - Einschalt"zeit"
    return maxLong - onTime + offTime;
}


/**
 * @brief Entprellen der Schalter per Sofware.
 * Siehe Pina Merkert: Fliegender Frederick, c't 22/2020, S. 152
 * Hier ist erstmal nur der Code aufgeschrieben - Implementierung und Test stehen noch aus.
 *
 * @todo Braucht's das überhaupt?
 *
 */
uint8_t debounce(uint8_t &history, uint8_t &newStatus) {
    history = (history << static_cast<uint8_t>(1)) | (newStatus);     // bisherige history mit *aktuell gelesenem* Status aktualisieren
    switch (history){
        case 0b00000000: newStatus = LOW; break;
        case 0b11111111: newStatus = HIGH; break;
        default: ;
    }
    return newStatus;
}
