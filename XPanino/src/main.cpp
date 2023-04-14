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
 * avr-gdp-Debugger auf Hex-zahlen umschalten: set output-radix 16
 *
 ************************************************************************************************************/

#if defined DEBUG && defined NOT_WITH_SERIAL
   #include "avr8-stub.h"
   //#include "app_api.h" // only needed with flash breakpoints
#endif

// Headerdateien der Objekte includen
#include <Arduino.h>
#include <switchmatrix.hpp>
#include <tools.hpp>

const long SERIAL_BAUDRATE = 57600;     ///< Baudrate für den seriellen Port. Nur hier ändern!!

SwitchMatrixClass switchMatrix;

/*********************************************************************************************************//**
 * @brief Event: Zeichen liegt an der seriellen Schnittstelle vor.
 *
 ************************************************************************************************************/
#ifndef NOT_WITH_SERIAL
void serialEvent() {
    while (Serial.available() > 0) {
        char inChar = char(Serial.read());
        if (inChar == '\n') {
            inChar = '\r';  // dummy-anweisung damuit der Compiler nicht meckert
        }
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
#endif


/*********************************************************************************************************//**
 * @brief Initialisierungen des Arduino durchführen.
 *
 * Hier werden die einzelnen Displays, 7-Segment-Anzeigen und LEDs angelegt und deren Position definiert
 * sowie die Hardware initialisiert und initial abgefragt bzw. gesetzt.
 *
 ************************************************************************************************************/
void setup() {
    #if defined DEBUG && defined NOT_WITH_SERIAL
    // initialize GDB stub
    debug_init();
    #endif

    // Mit aktiviertem gdb stub zum debuggen funktioniert die serielle Schnittstelle nicht
    // Siehe auch https://github.com/jdolinay/avr_debug/blob/master/avr8-stub/avr8-stub.h
    #ifndef NOT_WITH_SERIAL
    // Serielle Schnittstelle initialisieren
    if (Serial) {
        Serial.begin(SERIAL_BAUDRATE, SERIAL_8N1);
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
    #endif

    // Erst die eingebaute Status-LED als Status-Feedback ein paar mal blinken lassen, dann
    // die Hardware und I/O-Pins des Arduino initialisieren.
    // Status-Led hängt an Bit 5 (= PORTB5)
    DDRB |= _BV(DDB5);       // Port B5 als Output schalten (LOW)
    PORTB &= ~_BV(PORTB5);   // Pin auf LOW schalten -> Led aus
    _NOP();
    for (uint8_t i = 1; i < 9; ++i) {
        PINB |= _BV(PORTB5);    // Led umschalten (vgl. Datasheet Kap. 14.1)
        delay(300);
    }

    // Die Hardware (Ports) der Schaltermatrix initialisieren
    switchMatrix.initHardware();
    #if defined DEBUG && defined NOT_WITH_SERIAL
    breakpoint();
    #endif
}


/*********************************************************************************************************//**
 * @brief Lfd. Verarbeitung in
 *
 ************************************************************************************************************/
void loop(){
    switchMatrix.scanSwitchPins();
    delay(100);
}
