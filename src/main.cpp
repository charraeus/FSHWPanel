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

#if defined(ARDUINO) // Arduino 1.8 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

// Include user and local libraries
#include "ledmatrix.hpp"
#include "Switchmatrix.hpp"
#include "xpdr.hpp"
#include "dispatcher.hpp"
#include "commands.hpp"


//callbackEventClock = &ClockDavtronM803.event();  ///< geht das so? Welcher Typ?
Dispatcher dispatcher;          ///< Dispatcher-Objekt anlegen
LedMatrix leds;                 ///< LedMatrix anlegen
SwitchMatrix switches;          ///< Schaltermatrix - SwitchMatrix - anlegen
ClockDavtronM803 davtron803;    ///< Uhr anlegen (ClockDavtron M803)


/******************************************************************************
 * @brief Zeichen liegt an der seriellen Schnittstelle vor
 * 
 ******************************************************************************/
void serialEvent() {
    while (Serial.available() > 0) { 
        char inChar = Serial.read();
        // prüfen auf gültige Zeichen.
        // gültige Zeichen zum Parser senden, ungültige Zeichen ignorieren
        // gültig sind: <CR>, Space, '_' und alle alfanumerischen Zeichen
        if ((inChar == '\r') || isSpace(inChar) || (inChar == '_') || isAlphaNumeric(inChar)) {
            dispatcher.parseSerial(inChar);
        }
        #ifdef DEBUG
        Serial.print(inChar);
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
        /// Lesepuffer leeren
        while (Serial.available() > 0) {
            Serial.read();
        }
        Serial.println("Los gehts.");
    }
    switches.printMatrix();
    leds.initHardware(); ///< LED-Hardware initialisieren
    //switches.setCallback;
     
    /// Initiale Schalterstände abfragen und übertragen
    switches.scanSwitchPins();
    switches.transmitStatus(TRANSMIT_ALL_SWITCHES);

    leds.powerOnSelfTest();
    
    davtron803.init();
    //davtron803.event("XPDR_BTN OAT ON");
    
}

// loop code

/******************************************************************************
 * @brief Lfd.\ Verarbeitung
 * 
 ******************************************************************************/
void loop() {
    switches.scanSwitchPins();  ///< Hardware-Schalter abfragen
    switches.transmitStatus(TRANSMIT_ONLY_CHANGED_SWITCHES);    ///< Geänderte Schalterstände verarbeiten
    //readXplane()  -  Daten vom X-Plane einlesen (besser als Interrupt realisieren)
    //processLocal()  -  hier finden lokale Verarbeitungen statt, z.B. LED als Schalterreaktion direkt einschalten
    leds.display();     ///< LEDs anzeigen bzw. refreshen
}
