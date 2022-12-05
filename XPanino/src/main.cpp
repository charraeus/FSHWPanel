/**************************************************************************************************
 * @file main.cpp
 * @brief Hauptprogramm XPanino.
 * X-Plane-Panel mit Arduino - Proof of concept.
 *
 * Dieses Programm wird auf den Arduino geladen und stellt das Interface zwischen Flugsimulator
 * und der Schalter- und Anzeige-Hardware dar.
 * @author Christian Harraeus (christian@harraeus.de)
 * @version 0.2
 **************************************************************************************************/

/// Headerdateien der Objekte includen
#include <Switchmatrix.hpp>
#include <ledmatrix.hpp>
#include <parser.hpp>
#include <xpdr.hpp>
//#include <commands.hpp>

/// Makros für serielle Schnittstelle definieren
#define _SERIAL_BAUDRATE 115200     /// NOLINT Baudrate für den seriellen Port. Nur hier ändern!!

/// Objekte anlegen
LedMatrix leds;             ///< LedMatrix anlegen
ParserClass parser;         ///< Parser-Objekt anlegen
SwitchMatrix switches;      ///< Schaltermatrix - SwitchMatrix - anlegen
BufferClass inBuffer;       ///< Eingabepuffer anlegen

ClockDavtronM803 davtron803;    ///< Uhr anlegen (ClockDavtron M803)


/******************************************************************************
 * @brief Zeichen liegt an der seriellen Schnittstelle vor.
 *
 ******************************************************************************/
void serialEvent() {
    while (Serial.available() > 0) {
        char inChar = char(Serial.read());
        // prüfen auf gültige Zeichen.
        // gültige Zeichen in den Buffer aufnehmen, ungültige Zeichen ignorieren
        // gültig sind: Space, '_' und alle alfanumerischen Zeichen
        if (isAlphaNumeric(inChar) || (isPunct(inChar)) || (inChar == '_') || (inChar == ' ')) {
            inBuffer.addChar(inChar);
            #ifdef DEBUG
            Serial.println(inChar);
            Serial.println(inBuffer.get());
            #endif
        } else {
            if (((inChar == '\n') || (inChar == '\r')) && (! inBuffer.isEmpty())) {
                // Zeilenende erkannt und der inBuffer ist nicht leer. D.h., vorher wurde kein '\r' bzw. '\n' gelesen,
                // was dann den inBuffer geleert hätte.
                // Buffer zum Parsen zum Parser senden.
                parser.parseString(inBuffer.get());
                inBuffer.wipe();
            }
            // alle anderen Zeichen werden ignoriert.
        }
    }
}


/******************************************************************************
 * @brief Initialisierungen des Arduino durchführen.
 *
 * @todo Dokumentation hier ergänzen und Doxygen-Test-Doku 'rausnehmen.
 ******************************************************************************/
void setup() {
    /// Serielle Schnittstelle initialisieren
    if (Serial) {
        Serial.begin(_SERIAL_BAUDRATE, SERIAL_8N1);
        // wait for serial port to connect. Needed for native USB
        while (!Serial) {
            ;
        }
        // Schreibpuffer leeren
        Serial.flush();
        // Lesepuffer leeren
        while (Serial.available() > 0) {
            Serial.read();
        }
        Serial.println("XPanino V0.2 (c) Christian Harraeus, 2020 - 2022.\nLos geht's.");
    }

    leds.initHardware();                      ///< Arduino-Hardware der LED-Matrix initialisieren.

    const uint8_t OATVOLTS = 0;                         ///< Das Display-Feld OATVOLTS definieren. Es besteht aus 4 7-Segment-Anzeigen:
    leds.defineDisplayField(OATVOLTS, 0, {0, 16});       ///< Die 1. 7-Segment-Anzeige liegt auf der Row 0 und den Cols 16 bis 23.
    leds.defineDisplayField(OATVOLTS, 1, {1, 16});        ///< Die 2. 7-Segment-Anzeige liegt auf der Row 1 und den Cols 16 bis 23.
    leds.defineDisplayField(OATVOLTS, 2, {2, 16});        ///< Die 3. 7-Segment-Anzeige liegt auf der Row 2 und den Cols 16 bis 23.
    leds.defineDisplayField(OATVOLTS, 3, {3, 16});        ///< Die 4. 7-Segment-Anzeige liegt auf der Row 3 und den Cols 16 bis 23.
    leds.display(OATVOLTS, "OAT.U");

    const uint8_t UHR = 1;
    leds.defineDisplayField(UHR, 0, {4, 16});        ///< Die 1. 7-Segment-Anzeige liegt auf der Row 4 und den Cols 16 bis 23: Zehnerstelle der Stunde.
    leds.defineDisplayField(UHR, 1, {5, 16});        ///< Die 2. 7-Segment-Anzeige liegt auf der Row 5 und den Cols 16 bis 23: Einerstelle der Stunde.
    leds.defineDisplayField(UHR, 2, {6, 16});        ///< Die 3. 7-Segment-Anzeige liegt auf der Row 6 und den Cols 16 bis 23: Zehnerstelle der Minute.
    leds.defineDisplayField(UHR, 3, {7, 16});        ///< Die 4. 7-Segment-Anzeige liegt auf der Row 7 und den Cols 16 bis 23: Einerstelle der Minute.
    leds.display(UHR, "HH99");

    const uint8_t FL = 2;
    leds.defineDisplayField(FL, 0, {0, 8});         ///< Die 1. 7-Segment-Anzeige liegt auf der Row 0 und den Cols 8 bis 15: Hunderterstelle.
    leds.defineDisplayField(FL, 1, {1, 8});         ///< Die 2. 7-Segment-Anzeige liegt auf der Row 1 und den Cols 8 bis 15: Zehnerstelle .
    leds.defineDisplayField(FL, 2, {2, 8});         ///< Die 3. 7-Segment-Anzeige liegt auf der Row 2 und den Cols 8 bis 15: Einerstelle.
    leds.display(FL, "020");

    const uint8_t SQUAWK = 3;
    leds.defineDisplayField(SQUAWK, 0, {3, 8});        ///< Die 1. 7-Segment-Anzeige liegt auf der Row 3 und den Cols 8 bis 15: Tausenderstelle.
    leds.defineDisplayField(SQUAWK, 1, {4, 8});        ///< Die 2. 7-Segment-Anzeige liegt auf der Row 4 und den Cols 8 bis 15: Hunderterstelle.
    leds.defineDisplayField(SQUAWK, 2, {5, 8});        ///< Die 3. 7-Segment-Anzeige liegt auf der Row 5 und den Cols 8 bis 15: Zehnerstelle.
    leds.defineDisplayField(SQUAWK, 3, {6, 8});        ///< Die 4. 7-Segment-Anzeige liegt auf der Row 6 und den Cols 8 bis 15: Einerstelle.
    leds.display(SQUAWK, "7.000");

    const LedMatrixPos LED_TRENNER_1{0, 4};         ///< Der obere Stunden-Minuten-Trenner liegt auf Row=0 und Col=4.
    leds.ledOn(LED_TRENNER_1);
    leds.ledBlinkOn(LED_TRENNER_1, BLINK_NORMAL);
    const LedMatrixPos LED_TRENNER_2{1, 4};         ///< Der untere Stunden-Minuten-Trenner liegt auf Row=1 und Col=4.
    leds.ledOn(LED_TRENNER_2);
    leds.ledBlinkOn(LED_TRENNER_2, BLINK_NORMAL);
    const LedMatrixPos LED_LT{2, 4};                ///< Die LED "LT" liegt auf Row=2 und Col=4.
    leds.ledOn(LED_LT);
    const LedMatrixPos LED_UT{3, 4};                ///< Die LED "UT" liegt auf Row=3 und Col=4.
    leds.ledOff(LED_UT);
    const LedMatrixPos LED_ET{4, 4};                ///< Die LED "ET" liegt auf Row=2 und Col=4.
    leds.ledOff(LED_ET);
    const LedMatrixPos LED_FT{5, 4};                ///< Die LED "FT" liegt auf Row=3 und Col=4.
    leds.ledOff(LED_FT);
    const LedMatrixPos LED_ALT{6, 4};               ///< Die LED "ALT" liegt auf Row=2 und Col=4.
    leds.ledOn(LED_ALT);
    const LedMatrixPos LED_R{7, 4};                 ///< Die LED "R" liegt auf Row=3 und Col=4.
    leds.ledOn(LED_R);
    leds.ledBlinkOn(LED_R, BLINK_SLOW);

    switches.initHardware();            ///< Die Arduino-Hardware der Schaltermatrix initialisieren.
    switches.scanSwitchPins();          ///< Initiale Schalterstände abfragen und übertragen.
    switches.transmitStatus(TRANSMIT_ALL_SWITCHES);     ///< Den aktuellen ein-/aus-Status der Schalter an den PC senden.
    #ifdef DEBUG
    switches.printMatrix();
    #endif
}

/******************************************************************************
 * @brief Lfd.\ Verarbeitung im loop().
 *
 ******************************************************************************/
void loop() {
    switches.scanSwitchPins();  ///< Hardware-Schalter abfragen
    switches.transmitStatus(TRANSMIT_ONLY_CHANGED_SWITCHES);    ///< Geänderte Schalterstände verarbeiten
    //readXplane()  -  Daten vom X-Plane einlesen (besser als Interrupt realisieren)
    //processLocal()  -  hier finden lokale Verarbeitungen statt, z.B. LED als Schalterreaktion direkt einschalten
    leds.writeToHardware();     ///< LEDs anzeigen bzw. refreshen
}
