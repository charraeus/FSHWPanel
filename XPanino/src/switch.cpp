/*********************************************************************************************************//**
 * @file switch.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Implementierung der Klasse @em Switch
 * @version 0.1
 * @date 2020-04-10
 *
 * Copyright © 2017 - 2020 Christian Harraeus. All rights reserved.
************************************************************************************************************/

#include <switch.hpp>
#include <parser.hpp>

/// @brief Dauer, ab wann ein Schalter lange eingeschaltet ist (3000 Millisekunden)
const unsigned long LONG_ON = 3000;

/// @brief EventListe zum Erzeugen von Schalterevents einbinden.
extern EventQueueClass eventList;

/**
 *
 *
 */
void Switch::setOn() {
    status = LOW;
    changed = true;
    switchPressTime = millis();     // Einschalt"zeit" merken
    longOnSent = false;
    longOn = false;
    onTime = 0;
}


/**
 *
 *
 */
void Switch::setOff() {
    status = HIGH;
    changed = true;
    onTime = calcTimeDiff(switchPressTime, millis());   // Differenz zwischen Ein- und Ausschalt"zeit" merken
    longOnSent = true;
    longOn = false;
}


/**
 *
 *
 */
void Switch::checkLongOn() {
    longOn = (onTime >= LONG_ON);
    if (longOn && (! longOnSent)) {
        changed = true;
    }
}


/**
 *
 *
 */
void Switch::updateOnTime(const unsigned long &newOnTime) {
    onTime = calcTimeDiff(switchPressTime, newOnTime);
}


/**
 *
 * @todo: von String auf char* umstellen
 */
void Switch::transmitStatus(uint8_t row, uint8_t col) {
    const String XPDR_SWITCH = "X S ";
    String charsToSend;

    if ((! longOnSent) && longOn) {
        // wenn der Schalter lang gedrückt ist und dieser Lang-Gedrückt-Status noch
        // nicht übertragen wurde, den Status "LON" (Long on) übertragen.
        longOnSent = true;
        changed = false;
        charsToSend = "LON ";
    } else {
        // Den An-/Aus-Status des Schalters übertragen.
        charsToSend = ((getStatus() == LOW) ? "ON " : "OFF ");
    }
    charsToSend = XPDR_SWITCH + charsToSend + String(row) + " " + String(col);
    #ifdef DEBUG
    Serial.print("Switch::transmitStatus: charsToSend="); Serial.print(charsToSend); Serial.println("|");
    #endif
    ///@todo Switch::transmitStatus ausprogrammieren.
    return;
}

/*********************************************************************************************************//**
 * ab hier die privaten Methoden
*************************************************************************************************************/

/**
 * @brief Differenz zweier Zeitstempel berechnen.
 *
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
    const unsigned long maxLong = 4294967295;  /** @todo besser sizeOf(unsigned long) nehmen */
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
 *
 * Siehe Pina Merkert: Fliegender Frederick, c't 22/2020, S. 152
 * Hier ist erstmal nur der Code aufgeschrieben - Implementierung und Test stehen noch aus.
 *
 * @todo Braucht's das überhaupt?
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
