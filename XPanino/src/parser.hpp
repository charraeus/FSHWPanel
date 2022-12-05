/**************************************************************************************************
 * @file parser.hpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Interface der Klassen @em ParserClass und @em BufferClass.
 * @version 0.1
 * @date 2017-11-17
 *
 * Copyright © 2017 - 2020. All rights reserved.
 *
 **************************************************************************************************/

#pragma once

#include <Arduino.h>

// Einige Konstanten
const uint8_t MAX_BUFFER_LENGTH = 50;   ///< Der Buffer kann max. 49 Zeichen zzgl. Zeilenende '\0' aufnehmen.
const uint8_t MAX_PARA_LENGTH = 10;     ///< Max. Länge der geparsten Kommandoparameter.


/**************************************************************************************************
 * @brief Puffer für die Ein-/Ausgabe von Zeichen von/an der/die serielle Schnittstelle.
 *
 * Aus Ressoureneinsparungsgründen werden die C-Strings verwendet (und nicht <String.h>)
 *
 ***************************************************************************************************/
class BufferClass {
public:
    /** get
     * @brief Inhalt des Zeichenpuffers ausgeben
     *
     * @return char* Zeichenpuffer
     */
    char *get() { return buffer; }


    /** addChar
     * @brief Zeichen an das Ende des Zeichenpuffers anhängen.
     *
     * @param inChar Das anzuhängende Zeichen.
     * @return @em 1: erfolgreich durchgeführt.
     *         @em 0: Fehler aufgetreten. Wahrscheinlich, weil buffer voll ist.
     */
    uint8_t addChar(char inChar);


    /** wipe
     * @brief Zeichenpuffer löschen.
     *
     */
    void wipe() { actPos = 0; buffer[actPos] = '\0'; }


    /** isEmpty
     * @brief Prüfung, ob der Zeichenpuffer leer ist.
     *
     * @return @em true     Der Zeichenpuffer ist leer.
     * @return @em false    Der Zeichenpuffer ist nicht leer.
     */
    bool isEmpty() { return strlen(buffer) == 0; }

private:
    char buffer[MAX_BUFFER_LENGTH]{'\0'};  ///< Zeichenpuffer der Länge _BUFFER_LENGTH
    unsigned int actPos{0};                ///< aktuelles Ende des Buffers; zeigt auf die Position nach dem letzten Zeichen
};


/**************************************************************************************************
 * @brief Vom Flugsimulator kommenden Kommandstring parsen und zerlegen.
 *
 ***************************************************************************************************/
class ParserClass {
public:
    /** parseString
     * @brief Den vom Flugsimulator erhaltenen String parsen und in 4 Parameter zerlegen.
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
     */
    bool parseString(char *inBuffer);


    #ifdef DEBUG
    void printData();
    #endif

private:
    char device[MAX_PARA_LENGTH] = "device";       ///< Avionic-Gerät, von/zu dem die Aktion stammt/gehört.
    char devEvent[MAX_PARA_LENGTH] = "devEvent";   ///< Ereignis, das aufgetreten ist, z.B. "Schalter angeschaltet".
    char parameter1[MAX_PARA_LENGTH] = "para1";    ///< Parameter zum Ereignis, z.B. die Schalterbezeichnung.
    char parameter2[MAX_PARA_LENGTH] = "para2";    ///< Parameter zum Ereignis, z.B. die Schalterbezeichnung.

    /** dispatch
     * @brief Prüfung, ob der Zeichenpuffer leer ist.
     *
     * @return @em true     Der Zeichenpuffer ist leer.
     * @return @em false    Der Zeichenpuffer ist nicht leer.
     */
    void dispatch();    ///< Dispatcher aufrufen. @todo Dispatcher in eine eigene Klasse auslagern.
};
