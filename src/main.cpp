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
#include "dispatcher.hpp"
//#include "xpdr.hpp"
//#include "commands.hpp"

/// Makros für serielle Schnittstelle definieren
#define _SERIAL_BAUDRATE 115200     /// Baudrate für den seriellen Port. Nur hier ändern!!

/// Objekte anlegen
LedMatrix leds;                 ///< LedMatrix anlegen
Dispatcher dispatcher;          ///< Dispatcher-Objekt anlegen
SwitchMatrix switches;          ///< Schaltermatrix - SwitchMatrix - anlegen
BufferClass inBuffer;           ///< Eingabepuffer

//ClockDavtronM803 davtron803;    ///< Uhr anlegen (ClockDavtron M803)


/******************************************************************************
 * @brief Zeichen liegt an der seriellen Schnittstelle vor
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
                // Buffer zum Parsen zum Dispatcher senden.
                dispatcher.parseString(inBuffer.get());
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

    leds.initHardware();
    leds.powerOnSelfTest();
    switches.initHardware();
      
    /// Initiale Schalterstände abfragen und übertragen
    switches.scanSwitchPins();
    switches.transmitStatus(TRANSMIT_ALL_SWITCHES);
    switches.printMatrix();
    
}

// loop code

/******************************************************************************
 * @brief Lfd.\ Verarbeitung
 * 
 ******************************************************************************/
void loop() {
    //switches.scanSwitchPins();  ///< Hardware-Schalter abfragen
    //switches.transmitStatus(TRANSMIT_ONLY_CHANGED_SWITCHES);    ///< Geänderte Schalterstände verarbeiten
    //readXplane()  -  Daten vom X-Plane einlesen (besser als Interrupt realisieren)
    //processLocal()  -  hier finden lokale Verarbeitungen statt, z.B. LED als Schalterreaktion direkt einschalten
    leds.display();     ///< LEDs anzeigen bzw. refreshen
}
