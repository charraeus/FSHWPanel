/**************************************************************************************************
 * @file parser.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em Parser.
 * @version 0.1
 * @date 2017-11-17
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 **************************************************************************************************/

#include <parser.hpp>


/**************************************************************************************************
 * @brief Puffer für die Ein-/Ausgabe von Zeichen von/an der/die serielle Schnittstelle.
 *
 * Aus Ressoureneinsparungsgründen werden die C-Strings verwendet (und nicht <String.h>)
 *
 ***************************************************************************************************/

/** addChar
 * @brief Zeichen an das Ende des Zeichenpuffers anhängen.
 *
 * @param inChar Das anzuhängende Zeichen.
 * @return @em 1: erfolgreich durchgeführt.
 *         @em 0: Fehler aufgetreten. Wahrscheinlich, weil buffer voll ist.
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


    /** wipe
     * @brief Zeichenpuffer löschen.
     *
     */
    void BufferClass::wipe() { actPos = 0; buffer[actPos] = '\0'; }


/**************************************************************************************************
 * @brief Vom Flugsimulator kommenden Kommandstring parsen und zerlegen.
 *
 ***************************************************************************************************/

/** parseString
 * @brief Den vom Flugsimulator erhaltenen String parsen und in 4 Parameter zerlegen.
 *
 * Im vom Flugsimulator enthaltenen String werden folgende Informationen erwartet,
 * jeweils getrennt durch ein Blank. Es müssen immer diese 4 Parameter übertragen werden:
 * - device     - Device, dass ein Ereignis hat
 *                FL   : Flight Level --> Flightlevel in Parameter1
 *                XPDR : Transpondercode --> Code in Parameter1
 *                LT   : Local Time --> Zeit im Format HHMM in Parameter1
 *                OAT  : OAT in °C --> Grad in Parameter1
 *                VOLT : Volts --> Wert in Parameter1
 * - devEvent      - CHANGE
 * - parameter1,
 * - parameter2 - Werte zu dem device/Ereignis
 *
 * @param inBuffer Enthält die eingelesenen Zeichen, die von der seriellen Schnittstelle gelesen wurden.
 *                 Der inBuffer wird dabei zerstört.
 * @return @em true  Parsen wurde fehlerfrei abgeschlossen.
 *         @em false Fehler beim Parsen aufgetreten.
 *
 * @note Vom Flugsimulator eingelesene Zeile parsen
 *       Ergebnis des Parsens ist deviceName, switchName, eventName
 *
 *       Aber ACHTUNG
 *       Das kann durcheinander geben, falls der Ausgangsstatus nicht DSTATE_WAIT_FOR_device ist.
 *       das kann passieren, wenn wir hier rein laufen, aber noch ein einlesen von der seriellen
 *       Schnittstelle läuft.
 *       in der Switchmatrix direkt dispatch(device, event, parameter1, parameter2) aufrufen
 *       das muss aber noch geschaut werden, wie das gehen kann
 */
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


void ParserClass::dispatch() {
    // Events je nach Quelle verteilen
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
