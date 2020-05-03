/**************************************************************************************************
 * @file main.cpp
 * @brief Hauptprogramm XPanino.
 * X-Plane-Panel mit Arduino - Proof of concept.
 * 
 * Dieses Programm wird auf den Arduino geladen und stellt das Interface zwischen Flugsimulator
 * und der Schalter- und Anzeige-Hardware dar.
 * @author Christian Harraeus (christian@harraeus.de)
 * @version 0.1
 **************************************************************************************************/

/// Headerdateien der Objekte includen
#include "Switchmatrix.hpp"
#include "ledmatrix.hpp"
#include "parser.hpp"
#include "xpdr.hpp"
//#include "commands.hpp"

/// Makros für serielle Schnittstelle definieren
#define _SERIAL_BAUDRATE 115200     /// Baudrate für den seriellen Port. Nur hier ändern!!

/// Objekte anlegen
LedMatrix leds;             ///< LedMatrix anlegen
ParserClass parser;         ///< Parser-Objekt anlegen
SwitchMatrix switches;      ///< Schaltermatrix - SwitchMatrix - anlegen
BufferClass inBuffer;       ///< Eingabepuffer anlegen

ClockDavtronM803 davtron803;    ///< Uhr anlegen (ClockDavtron M803)


/******************************************************************************
 * @brief Die Schalter benennen.
 * 
 ******************************************************************************/
void setSwitchNames() {
    /** row 0 */ switches.setSwitchName(0, 0, "0"); switches.setSwitchName(0, 1, "1"); switches.setSwitchName(0, 2, "2"); switches.setSwitchName(0, 3, "3");
                 switches.setSwitchName(0, 4, "4"); switches.setSwitchName(0, 5, "5"); switches.setSwitchName(0, 6, "6"); switches.setSwitchName(0, 7, "7");
    /** row 1 */ switches.setSwitchName(1, 0, "IDT"); switches.setSwitchName(1, 1, "VFR"); switches.setSwitchName(1, 2, "CLR"); switches.setSwitchName(1, 3, "OFF");
                 switches.setSwitchName(1, 4, "SBY"); switches.setSwitchName(1, 5, "TST"); switches.setSwitchName(1, 6, "ON"); switches.setSwitchName(1, 7, "ALT");
    /** row 2 */ switches.setSwitchName(2, 0, "2-0"); switches.setSwitchName(2, 1, "2-1"); switches.setSwitchName(2, 2, "2-2"); switches.setSwitchName(2, 3, "2-3");
                 switches.setSwitchName(2, 4, "2-4"); switches.setSwitchName(2, 5, "2-5"); switches.setSwitchName(2, 6, "OAT"); switches.setSwitchName(2, 7, "2-7");
    /** row 3 */ switches.setSwitchName(3, 0, "0"); switches.setSwitchName(3, 1, "1"); switches.setSwitchName(3, 2, "2"); switches.setSwitchName(3, 3, "3");
                 switches.setSwitchName(3, 4, "4"); switches.setSwitchName(3, 5, "5"); switches.setSwitchName(3, 6, "SEL"); switches.setSwitchName(3, 7, "CTL");
}


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
        Serial.println("Los gehts.");
    }

    leds.initHardware();                                ///< Arduino-Hardware der LED-Matrix initialisieren.
    leds.powerOnSelfTest();                             ///< Startmuster von LEDs anzeigen.
    #define _OATVOLTS 0
    leds.defineDisplayField(_OATVOLTS, 0, 0, 16);
    leds.defineDisplayField(_OATVOLTS, 1, 1, 16);
    leds.defineDisplayField(_OATVOLTS, 2, 2, 16);
    leds.defineDisplayField(_OATVOLTS, 3, 3, 16);
    leds.display(_OATVOLTS, "OAT.U");
    setSwitchNames();                                   ///< Die Schalter der Schaltermatrix benennen.
    switches.initHardware();                            ///< Die Arduino-Hardware der Schaltermatrix initialisieren.
    switches.scanSwitchPins();                          ///< Initiale Schalterstände abfragen und übertragen.
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
    leds.display();     ///< LEDs anzeigen bzw. refreshen
}
