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


/**************************************************************************************************/
uint8_t BufferClass::addChar(const char inChar) {
    if (actPos < _MAX_BUFFER_LENGTH - 1) {
        buffer[actPos] = inChar;
        actPos += 1;
        buffer[actPos] = '\0';
        return 1;
    } else {
        return 0;
    }
}


/**************************************************************************************************/
bool Dispatcher::parseString(char *inBuffer) {
    /// @todo Kommentar anpassen.
    // Vom Flugsimulator eingelesene Zeile parsen
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
    Serial.print("Dispatcher::parseString:");
    Serial.println(inBuffer);

    char *ptrParameter;
    uint8_t paramCount = 1;                 // Zähler für die richtige Variable der struct commandData.
    
    ptrParameter = strtok (inBuffer, " ");       // ptrChar enthält jetzt den Stringabschnitt bis zum ersten Blank.
    if (ptrParameter != nullptr) {               // Vorsichtshalber testen, ob was gefunden wurde.
        strcpy(commandData.source, ptrParameter);    // Diesen in commandData.source kopieren.
        while (ptrParameter != nullptr) {            // Das Zerlegen fortsetzen bis nichts mehr da ist.
            #ifdef DEBUG
            Serial.println (ptrParameter);
            #endif
            paramCount++;                       // Zähler hochzählen, für die richtige Variable der struct commandData.
            ptrParameter = strtok (nullptr, " ");    // In die richtige Variable den Teilstring bis zum jeweils
            switch (paramCount) {               // nächsten Blank hineinkopieren. 
                case 2: { strcpy(commandData.device, ptrParameter); break; }
                case 3: { strcpy(commandData.devEvent, ptrParameter); break; }
                case 4: { strcpy(commandData.parameter1, ptrParameter); break; }
                case 5: { strcpy(commandData.parameter2, ptrParameter); break; }
            }
        }
    }
    #ifdef DEBUG
    Serial.print("Source="); Serial.print(commandData.source); Serial.println("|");
    Serial.print("Device="); Serial.print(commandData.device); Serial.println("|");
    Serial.print("Event="); Serial.print(commandData.devEvent); Serial.println("|");
    Serial.print("Parameter1="); Serial.print(commandData.parameter1); Serial.println("|");
    Serial.print("Parameter2="); Serial.print(commandData.parameter2); Serial.println("|");
    #endif
    return (paramCount == 5);   // Falls paramCount nicht 5 ist, wurden nicht ausreichend viele Werte eingelesen.
}


void Dispatcher::dispatch() {
    // Events je nach Quelle verteilen
    // if (commandData.source == "CLOCK") {
        // rufe eine Callback-Funktion von ClockDavtronM802 auf. Die Adresse dieser Funktion muss 
        // aber zuerst von ClockDavtronM803 gesetzt worden sein.
        // Hierzu muss wahrscheinlich noch eine SetCallbackFunction implementiert werden.
        // callbackEventClock(switchName, eventName); --> Sollte die function ClockDavtronM803.event() aufrufen
    // } else {
    //     if (commandData.source == "XPDR") {
            // analog zu "CLOCK"
            // callbackEventClock(switchName, eventName);
    //     } else {
    //         if (commandData.source == "XP") {
                // Daten vom X-Plane sind angekommen
    //         }
    //     }
    // }
    Serial.println("Hier wird jetzt dispatscht...");
    
    // Kommandovariablen wieder putzen
    // commandData.source = "";
    // commandData.device = "";
    // commandData.devEvent = "";
    // commandData.parameter1 = "";
    // commandData.parameter2 = "";
}


void Dispatcher::dispatch(const String &inSource, const String &inDevice, const String &inEvent,
                          const String &inParameter1, const String &inParameter2) {
    // commandData.source = inSource;
    // commandData.device = inDevice;
    // commandData.devEvent = inEvent;
    // commandData.parameter1 = inParameter1;
    // commandData.parameter2 = inParameter2;
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
