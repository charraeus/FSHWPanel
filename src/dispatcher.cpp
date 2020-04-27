/**************************************************************************************************
 * @file dispatcher.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em Dispatcher.
 * @version 0.1
 * @date 2017-11-17
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
 **************************************************************************************************/

#include "dispatcher.hpp"
#include "error.hpp"

// Zustands-Konstanten für den Dispatcher
const uint8_t DSTATE_WAIT_FOR_SOURCE = 0;       ///< Als nächstes muss die Source eingelesen werden.
const uint8_t DSTATE_SOURCE_AVAILABLE = 1;      ///< Als nächstes muss das Event eingelesen werden.
const uint8_t DSTATE_DEVICE_AVAILABLE = 2;      ///< Als nächstes muss Parameter 1 eingelesen werden.
const uint8_t DSTATE_EVENT_AVAILABLE = 3;       ///< Als nächstes muss das Device eingelesen werden.
const uint8_t DSTATE_PARA1_AVAILABLE = 4;       ///< Als nächstes muss Parameter 2 eingelesen werden.
const uint8_t DSTATE_PARA2_AVAILABLE = 5;       ///< Als nächstes muss Parameter 2 eingelesen werden.
const uint8_t MAX_PARSER_LENGTH = 10;           ///< Maxmimale Zeichenanzahl für die Parserelemente.
const uint8_t MAX_COMMAND_LENGTH = 100;         ///< Maximale Zeichenanzahl der Eingabezeile.


/**************************************************************************************************/
Dispatcher::Dispatcher() {
    status = DSTATE_WAIT_FOR_SOURCE;
    command.reserve(MAX_COMMAND_LENGTH);
    commandData.source.reserve(MAX_PARSER_LENGTH);
    commandData.device.reserve(MAX_PARSER_LENGTH);
    commandData.devEvent.reserve(MAX_PARSER_LENGTH);
    commandData.parameter1.reserve(MAX_PARSER_LENGTH);
    commandData.parameter2.reserve(MAX_PARSER_LENGTH);
}


void Dispatcher::parseSerial(const char inChar) {
    // Die einzelnen Zeichen, die 'reinkommen zusammensetzen
    // Wenn ein Blank, <CR> oder <LF> oder '\0' empfangen wurde, die bisher eingelesenen
    // Zeichen als command, source, event, parameter1 oder parameter2 abspeichern
    #if DEBUG
    Serial.print(inChar);
    #endif
    if ((inChar != ' ') && (inChar != '\r') && (inChar != '\n') && (inChar != '\0')) {
        // gültiges "normales" Zeichen empfangen => zum command-String hinzufügen
        //if (! command.concat(inChar)) {
        //    Serial.println(_ERR_DISPATCHER_001);
        //}
        command += inChar;
    } else {    // "Trennzeichen" empfangen
        if ((inChar == ' ') || (inChar == '\n')) {
            // Blank oder <CR> empfangen => ist ein Trennzeichen.
            // Die bisher eingelesenen Zeichen speichern und den Zustand ändern.
            if ((inChar == '\n') && (status < DSTATE_PARA2_AVAILABLE)) {
                Serial.println(_ERR_DISPATCHER_002);
                commandData.source = "";
                commandData.device = "";
                commandData.devEvent = "";
                commandData.parameter1 = "";
                commandData.parameter2 = "";
            }
            if ((inChar == '\n') && (status == DSTATE_PARA2_AVAILABLE)) {
                dispatch();
                status = DSTATE_WAIT_FOR_SOURCE;
            } else {
                if (command.length() > static_cast<int>(MAX_COMMAND_LENGTH)) {
                    Serial.println(_ERR_DISPATCHER_003);
                } else {                        
                    switch (status) {
                        case DSTATE_WAIT_FOR_SOURCE: {
                            commandData.source = command;
                            status = DSTATE_SOURCE_AVAILABLE;
                            Serial.print("Source="); Serial.print(commandData.source); Serial.println(" ");
                            break;
                        }
                        case DSTATE_SOURCE_AVAILABLE: {
                            commandData.device = command;
                            status = DSTATE_DEVICE_AVAILABLE;
                            Serial.print("Device="); Serial.print(commandData.device); Serial.println(" ");
                            break;
                        }
                        case DSTATE_DEVICE_AVAILABLE: {
                            commandData.devEvent = command;
                            status = DSTATE_EVENT_AVAILABLE;
                            Serial.print("Event="); Serial.print(commandData.devEvent); Serial.println(" ");
                            break;
                        }
                        case DSTATE_EVENT_AVAILABLE: {
                            commandData.parameter1 = command;
                            status = DSTATE_PARA1_AVAILABLE;
                            Serial.print("Parameter1="); Serial.print(commandData.parameter1); Serial.println(" ");
                            break;
                        }
                        case DSTATE_PARA1_AVAILABLE: {
                            commandData.parameter2 = command;
                            status = DSTATE_PARA2_AVAILABLE;
                            Serial.print("Parameter2="); Serial.print(commandData.parameter2); Serial.println(" ");
                            break;
                        }
                        default: {
                        }
                    }
                    command = "";
                } // switch
            } // if command.length()...   
        } // if ((inChar == ' ') || (inChar == '\n'))
    } // if ((inChar != ' ') && ...
}


void Dispatcher::parseString(const String inCommandString) {
    // eventString parsen
    // Ergebnis des Parsens ist sourceName, switchName, eventName
    // z.B.                       XP FL CHANGE MAX_PARSER_LENGTH1    oder
    //                            XPDR BTN ON
    //
    // Aber ACHTUNG
    // Das kann durcheinander geben, falls der Ausgangsstatus nicht DSTATE_WAIT_FOR_SOURCE ist.
    // das kann passieren, wenn wir hier rein laufen, aber noch ein einlesen von der seriellen
    // Schnittstelle läuft.
    // in der Switchmatrix direkt dispatch(source, device, event, parameter1, parameter2) aufrufen
    // das muss aber noch geschaut werden, wie das gehen kann
    int blankPos = inCommandString.indexOf(" ");
    int startPos = 0;
    while (blankPos != -1) {
        startPos = blankPos + 1;
        blankPos = inCommandString.indexOf(" ", startPos);
    }
}


void Dispatcher::dispatch() {
    // Events je nach Quelle verteilen
    if (commandData.source == "CLOCK") {
        // rufe eine Callback-Funktion von ClockDavtronM802 auf. Die Adresse dieser Funktion muss 
        // aber zuerst von ClockDavtronM803 gesetzt worden sein.
        // Hierzu muss wahrscheinlich noch eine SetCallbackFunction implementiert werden.
        // callbackEventClock(switchName, eventName); --> Sollte die function ClockDavtronM803.event() aufrufen
    } else {
        if (commandData.source == "XPDR") {
            // analog zu "CLOCK"
            // callbackEventClock(switchName, eventName);
        } else {
            if (commandData.source == "XP") {
                // Daten vom X-Plane sind angekommen
            }
        }
    }
    Serial.println("Hier wird jetzt dispatscht...");
    
    // Kommandovariablen wieder putzen
    commandData.source = "";
    commandData.device = "";
    commandData.devEvent = "";
    commandData.parameter1 = "";
    commandData.parameter2 = "";
}


void Dispatcher::dispatch(const String &inSource, const String &inDevice, const String &inEvent,
                          const String &inParameter1, const String &inParameter2) {
    commandData.source = inSource;
    commandData.device = inDevice;
    commandData.devEvent = inEvent;
    commandData.parameter1 = inParameter1;
    commandData.parameter2 = inParameter2;
    //dispatch();
}


#ifdef DEBUG
void Dispatcher::printData() {
    Serial.println(commandData.source);
    Serial.println(commandData.device);
    Serial.println(commandData.devEvent);
    Serial.println(commandData.parameter1);
    Serial.println(commandData.parameter2);
    Serial.println(status);
    Serial.println(command);
}
#endif
