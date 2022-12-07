/*********************************************************************************************************//**
 * @file parser.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em Parser.
 * @version 0.1
 * @date 2017-11-17
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 ************************************************************************************************************/

#include <parser.hpp>


/*********************************************************************************************************//**
 * BufferClass Methoden
 *
 ************************************************************************************************************/

/**
 * Aus Ressoureneinsparungsgründen werden die C-Strings verwendet (und nicht <String.h>)
 */
uint8_t BufferClass::addChar(const char inChar) {
    if (actPos < MAX_BUFFER_LENGTH - 1) {
        buffer[actPos] = toupper(inChar);
        actPos += 1;
        buffer[actPos] = '\0';
        return 1;
    }
    return 0;
}


/**
 *
 *
 */
void BufferClass::wipe() { actPos = 0; buffer[actPos] = '\0'; }


/*********************************************************************************************************//**
 * ParserClass Methoden
 *
 ************************************************************************************************************/

/**************************************************************************************************
 * @brief Vom Flugsimulator kommenden Kommandstring parsen und zerlegen.
 *
 ***************************************************************************************************/
bool ParserClass::parseString(char *inBuffer) {
    const char* TOKEN_DELIMITER = ";";
    char* ptrParameter = NULL; // NOLINT
    uint8_t paramCount = 1;                     // Zähler für die richtige Variable der struct

    ptrParameter = strtok(inBuffer, TOKEN_DELIMITER);  // ptrParameter enthält jetzt den Stringabschnitt bis zum ersten Blank.
    if (ptrParameter != NULL) {  // NOLINT modernize-use-nullptr ;vorsichtshalber testen, ob was gefunden wurde.
        strcpy(device, ptrParameter);        // Diesen in device kopieren.
        while (ptrParameter != NULL) {       // NOLINT Das Zerlegen fortsetzen bis nichts mehr da ist.
            paramCount++;   // Zähler hochzählen, für die richtige Variable der struct
                            // In die richtige Variable den Teilstring bis zum
            ptrParameter = strtok(NULL, TOKEN_DELIMITER);   // NOLINT
            switch (paramCount) {               // jeweils nächsten Blank hineinkopieren.
                case 2: { strcpy(devEvent, ptrParameter); break; }
                case 3: { strcpy(parameter1, ptrParameter); break; }
                case 4: { strcpy(parameter2, ptrParameter); break; }
                default: ;  // mehr als 4 Tokens; das ist ein Fehler; die überzähligen Token ignorieren
            }
        }
    }
    #ifdef DEBUG
    Serial.print("Device="); Serial.print(device); Serial.println("|");
    Serial.print("Event="); Serial.print(devEvent); Serial.println("|");
    Serial.print("Parameter1="); Serial.print(parameter1); Serial.println("|");
    Serial.print("Parameter2="); Serial.print(parameter2); Serial.println("|");
    #endif
    return (paramCount == 4);   // Falls paramCount nicht 4 ist, wurden nicht ausreichend viele Werte eingelesen.
}


/*********************************************************************************************************//**
 * ab hier die privaten Methoden
*************************************************************************************************************/

/**
 * @brief Events je nach Quelle verteilen
 *
 *
 * @return nix
 *
 */
void ParserClass::dispatch() {
    // if (device == "CLOCK") {
        // rufe eine Callback-Funktion von ClockDavtronM802 auf. Die Adresse dieser Funktion muss
        // aber zuerst von ClockDavtronM803 gesetzt worden sein.
        // Hierzu muss wahrscheinlich noch eine SetCallbackFunction implementiert werden.
        // callbackEventClock(switchName, eventName); --> Sollte die function ClockDavtronM803.event() aufrufen
    // }
    // if (device == "XPDR") {
    //     analog zu "CLOCK"
    //     callbackEventClock(switchName, eventName);
    // }
    // if (device == "XP") {
        // Daten vom X-Plane sind angekommen
    // }
    Serial.println("Hier wird jetzt dispatscht...");

    // Kommandovariablen wieder putzen
    // device = "";
    // devEvent = "";
    // parameter1 = "";
    // parameter2 = "";
}


#ifdef DEBUG
void ParserClass::printData() {
    Serial.println(device);
    Serial.println(devEvent);
    Serial.println(parameter1);
    Serial.println(parameter2);
}
#endif
