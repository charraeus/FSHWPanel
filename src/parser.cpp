/**************************************************************************************************
 * @file parser.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em Parser.
 * @version 0.1
 * @date 2017-11-17
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
 **************************************************************************************************/

#include "parser.hpp"
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
bool ParserClass::parseString(char *inBuffer) {
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
    char *ptrParameter;
    uint8_t paramCount = 1;                 // Zähler für die richtige Variable der struct 
    
    ptrParameter = strtok (inBuffer, " ");       // ptrChar enthält jetzt den Stringabschnitt bis zum ersten Blank.
    if (ptrParameter != nullptr) {               // Vorsichtshalber testen, ob was gefunden wurde.
        strcpy(source, ptrParameter);    // Diesen in source kopieren.
        while (ptrParameter != nullptr) {            // Das Zerlegen fortsetzen bis nichts mehr da ist.
            #ifdef DEBUG
            Serial.println (ptrParameter);
            #endif
            paramCount++;                       // Zähler hochzählen, für die richtige Variable der struct 
            ptrParameter = strtok (nullptr, " ");    // In die richtige Variable den Teilstring bis zum jeweils
            switch (paramCount) {               // nächsten Blank hineinkopieren. 
                case 2: { strcpy(device, ptrParameter); break; }
                case 3: { strcpy(devEvent, ptrParameter); break; }
                case 4: { strcpy(parameter1, ptrParameter); break; }
                case 5: { strcpy(parameter2, ptrParameter); break; }
            }
        }
    }
    #ifdef DEBUG
    Serial.print("Source="); Serial.print(source); Serial.println("|");
    Serial.print("Device="); Serial.print(device); Serial.println("|");
    Serial.print("Event="); Serial.print(devEvent); Serial.println("|");
    Serial.print("Parameter1="); Serial.print(parameter1); Serial.println("|");
    Serial.print("Parameter2="); Serial.print(parameter2); Serial.println("|");
    #endif
    return (paramCount == 5);   // Falls paramCount nicht 5 ist, wurden nicht ausreichend viele Werte eingelesen.
}


void ParserClass::dispatch() {
    // Events je nach Quelle verteilen
    // if (source == "CLOCK") {
        // rufe eine Callback-Funktion von ClockDavtronM802 auf. Die Adresse dieser Funktion muss 
        // aber zuerst von ClockDavtronM803 gesetzt worden sein.
        // Hierzu muss wahrscheinlich noch eine SetCallbackFunction implementiert werden.
        // callbackEventClock(switchName, eventName); --> Sollte die function ClockDavtronM803.event() aufrufen
    // } else {
    //     if (source == "XPDR") {
            // analog zu "CLOCK"
            // callbackEventClock(switchName, eventName);
    //     } else {
    //         if (source == "XP") {
                // Daten vom X-Plane sind angekommen
    //         }
    //     }
    // }
    Serial.println("Hier wird jetzt dispatscht...");
    
    // Kommandovariablen wieder putzen
    // source = "";
    // device = "";
    // devEvent = "";
    // parameter1 = "";
    // parameter2 = "";
}


#ifdef DEBUG
void ParserClass::printData() {
    Serial.println(source);
    Serial.println(device);
    Serial.println(devEvent);
    Serial.println(parameter1);
    Serial.println(parameter2);
}
#endif
