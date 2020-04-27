/**************************************************************************************************
 * 
 * @file main.cpp
 * @brief Hauptprogramm XPanino.
 * X-Plane-Panel mit Arduino - Proof of concept.
 * 
 * Dieses Programm wird auf den Arduino geladen und stellt das Interface zwischen Flugsimulator
 * und der Schalter- und Anzeige-Hardware dar.
 * @author Christian Harraeus (christian@harraeus.de)
 * @version 0.1
 **************************************************************************************************/

#include "Switchmatrix.hpp"
#include "ledmatrix.hpp"
#include "dispatcher.hpp"
//#include "xpdr.hpp"
//#include "commands.hpp"

LedMatrix leds;                 ///< LedMatrix anlegen
Dispatcher dispatcher;          ///< Dispatcher-Objekt anlegen
SwitchMatrix switches;          ///< Schaltermatrix - SwitchMatrix - anlegen

//ClockDavtronM803 davtron803;    ///< Uhr anlegen (ClockDavtron M803)


/******************************************************************************
 * @brief Zeichen liegt an der seriellen Schnittstelle vor
 * 
 ******************************************************************************/
void serialEvent() {
    while (Serial.available() > 0) { 
        char inChar = char(Serial.read());
        // prüfen auf gültige Zeichen.
        // gültige Zeichen zum Parser senden, ungültige Zeichen ignorieren
        // gültig sind: <CR>, Space, '_' und alle alfanumerischen Zeichen
        if ((inChar == '\r') || isSpace(inChar) || (inChar == '_') || isAlphaNumeric(inChar)) {
            dispatcher.parseSerial(inChar);
            #ifdef DEBUG
            Serial.print(inChar);
            #endif        
        } 
        #ifdef DEBUG
        else {
            Serial.print(inChar);
        }    
        #endif
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
        Serial.begin(115200, SERIAL_8N1);
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
    switches.printMatrix();
     
    /// Initiale Schalterstände abfragen und übertragen
    switches.scanSwitchPins();
    switches.transmitStatus(TRANSMIT_ALL_SWITCHES);

    
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
