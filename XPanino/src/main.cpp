/*********************************************************************************************************//**
 * @file main.cpp
 * @brief Hauptprogramm XPanino.
 * X-Plane-Panel mit Arduino - Proof of concept.
 *
 * Dieses Programm wird auf den Arduino geladen und stellt das Interface zwischen Flugsimulator
 * und der Schalter- und Anzeige-Hardware dar.
 * @author Christian Harraeus (christian@harraeus.de)
 * @version 0.2
 *
 * @todo Doxygen-Dokumentation anschauen und aktualieren, vereinheitlichen usw.
 *
 ************************************************************************************************************/

// Headerdateien der Objekte includen
#include <dispatcher.hpp>
#include <Switchmatrix.hpp>
#include <ledmatrix.hpp>
#include <buffer.hpp>
#include <m803.hpp>
#include <xpdr.hpp>
//#include <commands.hpp>

// Makros für serielle Schnittstelle definieren
#define _SERIAL_BAUDRATE 115200     /// NOLINT Baudrate für den seriellen Port. Nur hier ändern!!

// Objekte anlegen
DispatcherClass dispatcher; ///< Dispatcher
EventQueueClass eventQueue; ///< Event
BufferClass inBuffer;       ///< Eingabepuffer anlegen
LedMatrix leds;             ///< LedMatrix anlegen
SwitchMatrix switches;      ///< Schaltermatrix - SwitchMatrix - anlegen

ClockDavtronM803 m803;      ///< Uhr anlegen (ClockDavtron M803)
TransponderKT76C xpdr;      ///< Transponder anlegen


/*********************************************************************************************************//**
 * @brief Event: Zeichen liegt an der seriellen Schnittstelle vor.
 *
 ************************************************************************************************************/
void serialEvent() {
    while (Serial.available() > 0) {
        char inChar = char(Serial.read());
        // prüfen auf gültige Zeichen.
        // gültige Zeichen in den Buffer aufnehmen, ungültige Zeichen ignorieren
        // gültig sind: Space, '_' und alle alfanumerischen Zeichen
        if (isAlphaNumeric(inChar) || (isPunct(inChar)) || (inChar == ' ') || (inChar == '_')) {
             inBuffer.addChar(inChar);
        } else {
            if (((inChar == '\n') || (inChar == '\r')) && (! inBuffer.isEmpty())) {
                // Zeilenende erkannt und der inBuffer ist nicht leer. D.h., vorher wurde
                // kein '\r' bzw. '\n' gelesen, was dann den inBuffer geleert hätte.
                // Also den Buffer jetzt zum Parsen zum Parser senden.
                eventQueue.addEvent(inBuffer.parseString(inBuffer.get()));
                inBuffer.wipe();
                #ifdef DEBUG
                eventQueue.printQueue();
                #endif
            }
            // alle anderen Zeichen werden ignoriert.
        }
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

    leds.initHardware();                      ///< Arduino-Hardware der LED-Matrix initialisieren.

    const uint8_t FL = 2;
    leds.defineDisplayField(FL, 0, {0, 8});         ///< Die 1. 7-Segment-Anzeige liegt auf der Row 0 und den Cols 8 bis 15: Hunderterstelle.
    leds.defineDisplayField(FL, 1, {1, 8});         ///< Die 2. 7-Segment-Anzeige liegt auf der Row 1 und den Cols 8 bis 15: Zehnerstelle .
    leds.defineDisplayField(FL, 2, {2, 8});         ///< Die 3. 7-Segment-Anzeige liegt auf der Row 2 und den Cols 8 bis 15: Einerstelle.
    leds.display(FL, "0.20");
    leds.set7SegBlinkOn({0, 8}, true, BLINK_NORMAL);

    const uint8_t SQUAWK = 3;
    leds.defineDisplayField(SQUAWK, 0, {3, 8});        ///< Die 1. 7-Segment-Anzeige liegt auf der Row 3 und den Cols 8 bis 15: Tausenderstelle.
    leds.defineDisplayField(SQUAWK, 1, {4, 8});        ///< Die 2. 7-Segment-Anzeige liegt auf der Row 4 und den Cols 8 bis 15: Hunderterstelle.
    leds.defineDisplayField(SQUAWK, 2, {5, 8});        ///< Die 3. 7-Segment-Anzeige liegt auf der Row 5 und den Cols 8 bis 15: Zehnerstelle.
    leds.defineDisplayField(SQUAWK, 3, {6, 8});        ///< Die 4. 7-Segment-Anzeige liegt auf der Row 6 und den Cols 8 bis 15: Einerstelle.
    leds.display(SQUAWK, "7000");

    const LedMatrixPos LED_ALT{6, 4};       ///< Die LED "ALT" liegt auf Row=2 und Col=4.leds.ledOn(LED_ALT);
    leds.ledOn(LED_ALT);
    const LedMatrixPos LED_R = {7, 4};      ///< Die LED "R" liegt auf Row=3 und Col=4.
    leds.ledOn(LED_R);
    leds.ledBlinkOn(LED_R, BLINK_SLOW);

    switches.initHardware();            ///< Die Arduino-Hardware der Schaltermatrix initialisieren.
    switches.scanSwitchPins();          ///< Initiale Schalterstände abfragen und übertragen.
    switches.transmitStatus(TRANSMIT_ALL_SWITCHES);     ///< Den aktuellen ein-/aus-Status der Schalter an den PC senden.
    #ifdef DEBUG
    switches.printMatrix();
    //eventQueue.printQueue();
    #endif
}

/*********************************************************************************************************//**
 * @brief Lfd. Verarbeitung im loop().
 *
 ************************************************************************************************************/
void loop() {
    switches.scanSwitchPins();  ///< Hardware-Schalter abfragen
    switches.transmitStatus(TRANSMIT_ONLY_CHANGED_SWITCHES);    ///< Geänderte Schalterstände verarbeiten
    //readXplane()  -  Daten vom X-Plane einlesen (besser als Interrupt realisieren)
    dispatcher.dispatchAll();   ///< Eventqueue abarbeiten
    m803.show();
    //xpdr.show();
    leds.writeToHardware();     ///< LEDs anzeigen bzw. refreshen
}
