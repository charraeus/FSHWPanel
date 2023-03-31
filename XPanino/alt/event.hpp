/*********************************************************************************************************//**
 * @file event.hpp
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

// Einige Konstanten für die Stringlängen
const uint8_t MAX_SRC_DEV_LENGTH = 5;   ///< Max. Länge für je Kommando, Source und Device = 4 zzgl. '\0'.
const uint8_t MAX_PARA_LENGTH = 7;      ///< Max. Länge der geparsten Kommandoparameter = 6 zzgl. '\0'.


/*********************************************************************************************************//**
 * @brief Event - Daten zum Event, das noch abgearbeitet werden muss
 *
 ************************************************************************************************************/
class EventClass {
public:
    char device[MAX_SRC_DEV_LENGTH] = "";    ///< device für das das Event bestimmt ist
    char event[MAX_SRC_DEV_LENGTH] = "";     ///< ausgelöstes Event gem. Doku
    char parameter1[MAX_PARA_LENGTH] = "";   ///< Daten für das Event
    char parameter2[MAX_PARA_LENGTH] = "";   ///< Daten für das Event

    EventClass();
    void setNext(EventClass* next);
    EventClass* getNext();
    void printEvent();

private:
    EventClass* next = nullptr;         ///< Zeiger auf das nächste Event in der Liste
};


/*********************************************************************************************************//**
 * @brief Eventliste - sammelt die noch nicht abgearbeiteten Events
 *
 ************************************************************************************************************/
class EventQueueClass {
public:
    /**
     * @brief Konstruktor - Leere Liste anlegen.
     */
    EventQueueClass();

    /**
     * @brief Ein Event an das Ende der Liste anfügen.
     *
     * @param ptrNewEvent Zeiger auf das anzufügende Event
     */
    void addEvent(EventClass* ptrNewEvent);

    /**
     * @brief Das nächste Element aus der Liste holen und das Element aus der Liste löschen.
     * Falls es kein nächstes Element gibt, wird nullptr zurückgegeben.
     *
     * Der Speicher auf das Element wird nicht freigegeben. Es werden lediglich der @em head Pointer und
     * der @em tail Pointer aktualisiert.
     *
     * @return EventClass
     */
    EventClass *getHeadEvent();

    /**
     * @brief Ein Event am Kopf der Liste löschen.
     */
    void deleteHeadEvent(EventClass *event);

    #ifdef DEBUG
    void printQueue();
    #endif

private:
    EventClass* head;     ///< Zeiger auf 1. Event in der Eventliste
    EventClass* tail;     ///< Zeiger auf das letzte Event in der Eventliste
};
