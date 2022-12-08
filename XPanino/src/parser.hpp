/*********************************************************************************************************//**
 * @file parser.hpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Interface der Klassen @em ParserClass und @em BufferClass.
 * @version 0.1
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>

// Einige Konstanten
const uint8_t MAX_BUFFER_LENGTH = 50;   ///< Der Buffer kann max. 49 Zeichen zzgl. Zeilenende '\0' aufnehmen.
const uint8_t MAX_PARA_LENGTH = 15;     ///< Max. Länge der geparsten Kommandoparameter.


/*********************************************************************************************************//**
 * @brief Event - Daten zum Event, das noch abgearbeitet werden muss
 *
 ************************************************************************************************************/
class EventClass {
public:
    char device[MAX_PARA_LENGTH];       ///< device für das das Event bestimmt ist
    char event[MAX_PARA_LENGTH];        ///< ausgelöstes Event gem. Doku
    char parameter1[MAX_PARA_LENGTH];   ///< Daten für das Event
    char parameter2[MAX_PARA_LENGTH];   ///< Daten für das Event

    void setNext(EventClass* next);
    EventClass* getNext();

private:
    EventClass* next;         ///< Zeiger auf das nächste Event in der Liste
};


/*********************************************************************************************************//**
 * @brief Eventliste - sammelt die noch nicht abgearbeiteten Events
 *
 ************************************************************************************************************/
class EventListClass {
public:
    /**
     * @brief Konstruktor - Leere Liste anlegen.
     *
     */
    EventListClass();

    /**
     * @brief Ein Event an das Ende der Liste anfügen.
     *
     * @param newEvent Zeiger auf das anzufügende Event
     */
    void addEvent(EventClass* ptrNewEvent);

    /**
     * @brief Das nächste Element aus der Liste holen und das Element aus der Liste löschen.
     * Falls es kein nächstes Element gibt, wird nullptr zurückgegeben.
     *
     * @return EventClass
     */
    EventClass *getNextEvent();

    /**
     * @brief Ein Event am Kopf der Liste löschen.
     *
     */
    void deleteEvent();

    #ifdef DEBUG
    void listEvents();
    #endif

private:
    EventClass* head;     ///< Zeiger auf 1. Event in der Eventliste
    EventClass* tail;     ///< Zeiger auf das letzte Event in der Eventliste
};


/*********************************************************************************************************//**
 * @brief Puffer für die Ein-/Ausgabe von Zeichen von/an der/die serielle Schnittstelle.
 *
 * Aus Ressoureneinsparungsgründen werden die C-Strings verwendet (und nicht <String.h>)
 *
 ************************************************************************************************************/
class BufferClass {
public:
    /**
     * @brief Inhalt des Zeichenpuffers ausgeben
     *
     * @return char* Zeichenpuffer
     */
    inline char *get() { return buffer; }


    /**
     * @brief Zeichen an das Ende des Zeichenpuffers anhängen.
     *
     * @param inChar Das anzuhängende Zeichen.
     * @return @em 1: erfolgreich durchgeführt.\n
     *         @em 0: Fehler aufgetreten. Wahrscheinlich, weil buffer voll ist.
     */
    uint8_t addChar(char inChar);


    /**
     * @brief Zeichenpuffer löschen.
     *
     */
    void wipe();


    /**
     * @brief Prüfung, ob der Zeichenpuffer leer ist.
     *
     * @return @em true     Der Zeichenpuffer ist leer.\n
     * @return @em false    Der Zeichenpuffer ist nicht leer.
     */
    inline bool isEmpty() { return strlen(buffer) == 0; }

private:
    char buffer[MAX_BUFFER_LENGTH]{'\0'};  ///< Zeichenpuffer der Länge _BUFFER_LENGTH
    unsigned int actPos{0};  ///< aktuelles Ende des Buffers; zeigt auf die Position nach dem letzten Zeichen
};


/*********************************************************************************************************//**
 * @brief Vom Flugsimulator kommenden Kommandstring parsen und zerlegen.
 *
 ************************************************************************************************************/
class ParserClass {
public:
    /**
     * @brief Den vom Flugsimulator erhaltenen String parsen und in 4 Parameter zerlegen.
     *
     * Im vom Flugsimulator enthaltenen String werden folgende Informationen erwartet,
     * jeweils getrennt durch ein Blank. Es müssen immer diese 4 Parameter übertragen werden:
     * - device     - Device, dass ein Ereignis hat
     * - devEvent      - CHANGE
     * - parameter1,
     * - parameter2 - Werte zu dem device/Ereignis
     *
     * @param inBuffer Enthält die eingelesenen Zeichen, die von der seriellen Schnittstelle gelesen wurden.
     *                 Der inBuffer wird dabei zerstört.
     * @return @em true  Parsen wurde fehlerfrei abgeschlossen.\n
     *         @em false Fehler beim Parsen aufgetreten.
     */
    EventClass* parseString(char *inBuffer);
};
