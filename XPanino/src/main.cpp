/*********************************************************************************************************//**
 * @file main.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Hauptprogramm XPanino.
 *        X-Plane-Panel mit Arduino.
 * @version 0.2
 * @date 2023-03-31
 *
 * Dieses Programm wird auf den Arduino geladen und stellt das Interface zwischen Flugsimulator
 * und der Schalter- und Anzeige-Hardware dar.
 *
 * Dieser Branch ist ein komplett alternativer Ansatz, der hoffentlich deutlich schlanker im
 * RAM-Verbrauch ist.
 *
 *
 * @todo Doxygen-Dokumentation anschauen und aktualieren, vereinheitlichen usw.
 *
 ************************************************************************************************************/

// Headerdateien der Objekte includen
#include <Arduino.h>
#include "switchmatrix.hpp"
//#include <avr/io.h>

// Makros für serielle Schnittstelle definieren
#define _SERIAL_BAUDRATE 115200     /// NOLINT Baudrate für den seriellen Port. Nur hier ändern!!


/*********************************************************************************************************//**
 * @brief Event: Zeichen liegt an der seriellen Schnittstelle vor.
 *
 ************************************************************************************************************/
void serialEvent() {
    while (Serial.available() > 0) {
        // char inChar = char(Serial.read());
        // Check for valid characters.
        // gültige Zeichen in den Buffer aufnehmen, ungültige Zeichen ignorieren
        // gültig sind: Space, '_' und alle alfanumerischen Zeichen
        // if (isAlphaNumeric(inChar) || (isPunct(inChar)) || (inChar == ' ') || (inChar == '_')) {
        //      inBuffer.addChar(inChar);
        // } else {
        //     if (((inChar == '\n') || (inChar == '\r')) && (! inBuffer.isEmpty())) {
        //         // Zeilenende erkannt und der inBuffer ist nicht leer. D.h., vorher wurde
        //         // kein '\r' bzw. '\n' gelesen, was dann den inBuffer geleert hätte.
        //         // Also den Buffer jetzt zum Parsen zum Parser senden.
        //         eventQueue.addEvent(inBuffer.parseString(inBuffer.get()));
        //         inBuffer.wipe();
        //         #ifdef DEBUG
        //         eventQueue.printQueue();
        //         #endif
        //     }
        //     // All other characters are ignored.
        // }
    }
}


/*********************************************************************************************************//**
 * @brief Initialisierungen des Arduino durchführen.
 *
 * Hier werden die einzelnen Displays, 7-Segment-Anzeigen und LEDs angelegt und deren Position definiert
 * sowie die Hardware initialisiert und initial abgefragt bzw. gesetzt.
 *
 ************************************************************************************************************/
void setup() {
    // Serielle Schnittstelle initialisieren
    if (Serial) {
        Serial.begin(_SERIAL_BAUDRATE, SERIAL_8N1);
        // wait for serial port to connect. Needed for native USB
        while (!Serial);
        // Schreibpuffer leeren
        Serial.flush();
        // Lesepuffer leeren
        while (Serial.available() > 0) {
            Serial.read();
        }
        Serial.println(F("XPanino"));
    }

    // Schaltermatrix initialisieren
    initSwitchMatrix();
}

/*********************************************************************************************************//**
 * @brief Lfd. Verarbeitung in
 *
 ************************************************************************************************************/
void loop(){
    scanSwitchPins();
}