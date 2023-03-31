/*********************************************************************************************************//**
 * @file buffer.hpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Interface der Klasse @em BufferClass.
 * @version 0.1
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <event.hpp>

// Einige Konstanten für die Stringlängen
///< Max. Länge des Buffers ergibt sich aus der Länge der Bestandteile
constexpr uint8_t MAX_BUFFER_LENGTH = 3 * MAX_SRC_DEV_LENGTH + 2 * MAX_PARA_LENGTH;


/***********************************************************************************************//**
 * @brief Puffer für die Ein-/Ausgabe von Zeichen von/an der/die serielle Schnittstelle.
 *
 * Aus Ressoureneinsparungsgründen werden die C-Strings verwendet (und nicht <String.h>)
 *
 **************************************************************************************************/
class BufferClass {
public:
    /**
     * @brief Inhalt des Zeichenpuffers ausgeben
     *
     * @return char* Zeichenpuffer
     */
    inline char *get() { return buffer; }


    /// @brief Das von der seriellen Schnittstelle gelesene Zeichen an das Ende des Zeichenpuffers anhängen.
    /// Aus Ressoureneinsparungsgründen werden die C-Strings verwendet (und nicht <String.h>)
    /// @param inChar Das anzuhängende Zeichen.
    /// @return @em 0: erfolgreich durchgeführt.\n
    ///         @em 1: Fehler aufgetreten. Wahrscheinlich, weil buffer voll ist.
    uint8_t addChar(char inChar);


    /// @brief Den Eingabepuffer putzen.
    void wipe();


    /**
     * @brief Prüfung, ob der Zeichenpuffer leer ist.
     *
     * @return @em true     Der Zeichenpuffer ist leer.\n
     * @return @em false    Der Zeichenpuffer ist nicht leer.
     */
    inline bool isEmpty() { return strlen(buffer) == 0; }


    /**
     * @brief Den vom Flugsimulator erhaltenen String parsen und in 4 Parameter zerlegen.
     *
     * Im vom Flugsimulator enthaltenen String werden folgende Informationen erwartet,
     * jeweils getrennt durch ein Blank. Es müssen immer diese 4 Parameter übertragen werden:
     * - device     - Device, dass ein Ereignis hat
     * - devEvent   - Ereignis; vgl. Doku
     * - parameter1 - Wert zu dem device/Ereignis
     * - parameter2 - Wert zu dem device/Ereignis
     *
     * @param inBuffer Enthält die eingelesenen Zeichen, die von der seriellen Schnittstelle gelesen wurden.
     *                 Der inBuffer wird dabei zerstört.
     * @return @em true  Parsen wurde fehlerfrei abgeschlossen.\n
     *         @em false Fehler beim Parsen aufgetreten.
     */
    EventClass* parseString(char *inBuffer);

private:
    char buffer[MAX_BUFFER_LENGTH] = {"\0"};  ///< Zeichenpuffer der Länge _BUFFER_LENGTH
    unsigned int actPos{0};  ///< aktuelles Ende des Buffers; zeigt auf die Position nach dem letzten Zeichen
};
