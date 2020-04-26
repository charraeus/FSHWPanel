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
#include <Arduino.h>

// Zustands-Konstanten für den Dispatcher
const uint8_t DSTATE_COMMAND_AVAILABLE = 0;     ///< Ein fertiges Kommando ist bereit zum Verarbeiten.
const uint8_t DSTATE_WAIT_FOR_SOURCE = 1;       ///< Als nächstes muss die Source eingelesen werden.
const uint8_t DSTATE_WAIT_FOR_EVENT = 2;        ///< Als nächstes muss das Event eingelesen werden.
const uint8_t DSTATE_WAIT_FOR_DEVICE = 3;       ///< Als nächstes muss das Device eingelesen werden.
const uint8_t DSTATE_WAIT_FOR_PARA1 = 4;        ///< Als nächstes muss Parameter 1 eingelesen werden.
const uint8_t DSTATE_WAIT_FOR_PARA2 = 5;        ///< Als nächstes muss Parameter 2 eingelesen werden.

Dispatcher::Dispatcher() {
    status = DSTATE_WAIT_FOR_SOURCE;
}


/**************************************************************************************************/
void Dispatcher::parseSerial(const char inChar) {
    // Die einzelnen Zeichen, die 'reinkommen zusammensetzen
    // Wenn ein Blank, <CR> oder <LF> oder '\0' empfangen wurde, die bisher eingelesenen
    // Zeichen als command, source, event, parameter1 oder parameter2 abspeichern
    if ((inChar != ' ') && (inChar != '\r') && (inChar != '\n') && (inChar != '\0')) {
        // gültiges "normales" Zeichen empfangen => zum command-String hinzufügen
        command.concat(inChar);
    } else {    // "Sonderzeichen" empfangen
        if (inChar == ' ') {
            // Blank empfangen => Blank ist ein Trennzeichen.
            // Die bisher eingelesenen Zeichen speichern und den Zustand ändern
            switch (status) {
                case DSTATE_WAIT_FOR_SOURCE: {
                    commandData.source = command;
                    status = DSTATE_WAIT_FOR_DEVICE;
                    Serial.print("Source="); Serial.print(commandData.source); Serial.println(" ");
                    break;
                }
                case DSTATE_WAIT_FOR_DEVICE: {
                    commandData.device = command;
                    status = DSTATE_WAIT_FOR_EVENT;
                    Serial.print("Device="); Serial.print(commandData.device); Serial.println(" ");
                    break;
                }
                case DSTATE_WAIT_FOR_EVENT: {
                    commandData.devEvent = command;
                    status = DSTATE_WAIT_FOR_PARA1;
                    Serial.print("Event="); Serial.print(commandData.devEvent); Serial.println(" ");
                    break;
                }
                case DSTATE_WAIT_FOR_PARA1: {
                    commandData.parameter1 = command;
                    status = DSTATE_WAIT_FOR_PARA2;
                    Serial.print("Parameter1="); Serial.print(commandData.parameter1); Serial.println(" ");
                    break;
                }
                case DSTATE_WAIT_FOR_PARA2: {
                    commandData.parameter2 = command;
                    status = DSTATE_COMMAND_AVAILABLE;
                    Serial.print("Parameter2="); Serial.print(commandData.parameter2); Serial.println(" ");
                   break;
                }
                case DSTATE_COMMAND_AVAILABLE: {
                    #ifdef DEBUG
                    Serial.println("Vorheriges Kommando wurde noch nicht verarbeitet! Kann noch nix tun."); 
                    /// @todo Behandlung dieses Zustands ergänzen
                    #endif
                    break;
                }
                default: {
                }
            }
            command = "";
        } else {    // Verarbeitungen, falls ein Zeilenende o.Ä. empfangen wurde
            if ((inChar == '\r') || (inChar == '\n') || (inChar == '\0')) {
                // <CR> oder <LF> oder '\0' empfangen => eingelesenes Kommando inkl. Parametern verarbeiten
                Serial.println("<CR> o.Ä. empfangen.");
                switch (status) {
                    case DSTATE_WAIT_FOR_SOURCE:        // wir sind beim Einlesen der source
                    case DSTATE_WAIT_FOR_DEVICE:        // wir sind beim Einlesen des device
                    case DSTATE_WAIT_FOR_EVENT:         // wir sind beim Einlesen des events
                        // wenn wir hier landen, ist es ein Fehler, da noch Daten fehlen
                        // daher <CR> zurück auf Anfang.
                        status = DSTATE_WAIT_FOR_SOURCE;
                        commandData.source = "";
                        commandData.device = "";
                        commandData.devEvent = "";
                        commandData.parameter1 = "";
                        commandData.parameter2 = "";
                        #ifdef DEBUG
                        Serial.println("<CR> zu früh empfangen; zurück auf Anfang.");
                        #endif
                        break;
                    case DSTATE_WAIT_FOR_PARA1: {       // wir sind beim Einlesen des parameter1
                        if (commandData.parameter1.length() == 0) {
                            // es wurde noch nichts in parameter1 eingelesen; also müssen wir das nachholen
                            if (command.length() > 0) {     // Es ist ein Parameter 1 vorhanden
                                commandData.parameter1 = command;
                                status = DSTATE_COMMAND_AVAILABLE; // Es kommt kein parameter2 mehr, da <CR>, <LF> empfangen wurde
                                #ifdef DEBUG
                                Serial.print("<CR>Parameter1="); Serial.println(commandData.parameter1);
                                #endif
                            } else {    /// Kein Parameter 1 vorhanden; daher abbrechen @todo was, wenn kein Parameter notwendig ist?
                                status = DSTATE_WAIT_FOR_PARA2;
                                commandData.parameter1 = "";
                                commandData.source = "";
                                commandData.device = "";
                                commandData.devEvent = "";
                            }
                            commandData.parameter2 = "";
                            command = "";
                        }
                        break;
                    }
                    case DSTATE_WAIT_FOR_PARA2: {
                        if (commandData.parameter2.length() == 0) {
                            // es wurde noch nichts in parameter2 eingelesen; also müssen wir das nachholen
                            if (command.length() > 0) {     // Es ist ein Parameter 2 vorhanden
                                commandData.parameter2 = command;
                                status = DSTATE_COMMAND_AVAILABLE; // Es kommt kein parameter2 mehr, da <CR>, <LF> empfangen wurde
                                #ifdef DEBUG
                                Serial.print("<CR>Parameter2="); Serial.println(commandData.parameter2);
                                #endif
                            } else {    // Kein Parameter 2 vorhanden; daher abbrechen @todo was, wenn kein Parameter notwendig ist?
                                status = DSTATE_WAIT_FOR_SOURCE;
                                commandData.source = "";
                                commandData.device = "";
                                commandData.devEvent = "";
                                commandData.parameter1 = "";
                                commandData.parameter2 = "";
                            }
                        }    
                        break;
                    }
                    default:
                        // wenn wir hier landen, ist etwas total schief gegangen.
                        /// @todo exception oder Fehlermeldung einfügen.
                        #ifdef DEBUG
                        Serial.println("Unerwarteter Fehler. Kann nix tun!");
                        #endif
                        break;
                }
            }
        }
        if (status == DSTATE_COMMAND_AVAILABLE) {
            dispatch();
            status = DSTATE_WAIT_FOR_SOURCE;
        }
    }
}


void Dispatcher::parseString(const String inCommandString) {
    // eventString parsen
    // Ergebnis des Parsens ist sourceName, switchName, eventName
    // z.B.                       XP FL CHANGE 101    oder
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
        switch (status) {
            case DSTATE_WAIT_FOR_SOURCE: {
                commandData.source = inCommandString.substring(startPos, blankPos); // Den Namen der Quelle einlesen
                status = DSTATE_WAIT_FOR_DEVICE;
                break;
            }
            case DSTATE_WAIT_FOR_DEVICE: {
                commandData.device = inCommandString.substring(startPos, blankPos); // Das Device einlesen
                status = DSTATE_WAIT_FOR_EVENT;
                break;
            }
            case DSTATE_WAIT_FOR_EVENT: {
                commandData.devEvent = inCommandString.substring(startPos, blankPos);  // Das Event einlesen
                status = DSTATE_WAIT_FOR_PARA1;
                break;
            }
            case DSTATE_WAIT_FOR_PARA1: {
                commandData.parameter1 = inCommandString.substring(startPos, blankPos); // Parameter1 einlesen
                status = DSTATE_WAIT_FOR_PARA2;
                break;
            }
            case DSTATE_WAIT_FOR_PARA2: {
                commandData.parameter1 = inCommandString.substring(startPos, blankPos); // Parameter1 einlesen
                status = DSTATE_COMMAND_AVAILABLE;
                break;
            }
        }
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
    dispatch();
}



