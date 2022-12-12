/*********************************************************************************************************//**
 * @file parser.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em Parser.
 * @version 0.1
 * @date 2022-12-12
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 ************************************************************************************************************/

#include <parser.hpp>

/*********************************************************************************************************//**
 * Konstanten für Devices und Actions
 *
 * @todo: auf numerische Werte umstellen; ggf. commands.hpp nutzen
 ************************************************************************************************************/

/*********************************************************************************************************//**
 * @brief Event - public Methoden
 *
 ************************************************************************************************************/

/// @brief Konstruktor
EventClass::EventClass() {
    next = nullptr;
    #ifdef DEBUG
    printEvent();
    #endif
}

/**
 * @brief Zeiger auf das nächste Element in der Liste (= Nachfolger) eintragen.
 *
 * @param next
 */
void EventClass::setNext(EventClass* next) { this->next = next; };

/**
 * @brief Zeiger auf das nächste Element in der Liste zurückgeben
 *
 * @return EventClass*
 */
EventClass* EventClass::getNext() { return this->next; };

void EventClass::printEvent() {
    #ifdef DEBUG
    Serial.println(F("---EventClass---"));
    Serial.print(F("Device=")); Serial.print(device); Serial.println(F("|"));
    Serial.print(F("Event=")); Serial.print(event); Serial.println(F("|"));
    Serial.print(F("Parameter1=")); Serial.print(parameter1); Serial.println(F("|"));
    Serial.print(F("Parameter2=")); Serial.print(parameter2); Serial.println(F("|"));
    if (next == nullptr) { Serial.println(F("<nullptr>"));
    } else { Serial.println(F("<pointer>")); }
    #endif
}

/*********************************************************************************************************//**
 * @brief EventQueue - public Methoden
 *
 ************************************************************************************************************/
EventQueueClass::EventQueueClass() {
    head = nullptr;
    tail = nullptr;
};

void EventQueueClass::addEvent(EventClass* ptrNewEvent) {
    if (ptrNewEvent != nullptr) {
        if (tail == nullptr) {
            // Liste ist leer --> newEvent ist das einzige Element
            head = ptrNewEvent;
            tail = head;
            head->setNext(nullptr);  // Das letzte Element hat keinen Nachfolger
        } else {
            // Neues Element als letztes Element an die Liste anhängen
            tail->setNext(ptrNewEvent);     // im bisherigen letzten Element den Zeiger auf das
                                            // neue letzte Element eintragen
            tail = ptrNewEvent;             // Jetzt zeigt Tail auf das neue letzte Element
        }
    }
};

EventClass *EventQueueClass::getNextEvent() {
    EventClass* ptr = head;     // Zeiger auf 1. Element sichern
    head = head->getNext();     // head auf das 2. Element zeigen lassen
    return ptr;                 // Zeiger auf das bisherige 1. Element zurück geben
};

void EventQueueClass::deleteEvent() {
    if (head != nullptr) {
        // Liste ist nicht leer
        EventClass* ptr = head;     // Zeiger auf 1. Element sichern
        head = head->getNext();     // head auf das 2. Element zeigen lassen
        delete(ptr);                // Das bisherige 1. Element löschen
    }
};

#ifdef DEBUG
void EventQueueClass::printQueue() {
    Serial.println(F("----Eventqueue----"));
    EventClass* ptr = head;
    while (ptr != nullptr) {
        ptr->printEvent();
        ptr = ptr->getNext();
        Serial.println(F("---"));
    };
}
#endif


/*********************************************************************************************************//**
 * BufferClass - public Methoden
 *
 ************************************************************************************************************/

/**
 * Aus Ressoureneinsparungsgründen werden die C-Strings verwendet (und nicht <String.h>)
 */
uint8_t BufferClass::addChar(const char inChar) {
    if (actPos < MAX_BUFFER_LENGTH) {
        buffer[actPos] = toupper(inChar);
        actPos += 1;
        buffer[actPos] = '\0';
        return 0;
    }
    return 1;
}


/**
 *
 *
 */
void BufferClass::wipe() { actPos = 0; buffer[actPos] = '\0'; }


/*********************************************************************************************************//**
 * ParserClass - public Methoden
 ************************************************************************************************************/

/**************************************************************************************************
 * @brief Vom Flugsimulator kommenden Kommandstring parsen und zerlegen.
 *
 ***************************************************************************************************/
EventClass* ParserClass::parseString(char *inBuffer) {
    const char *TOKEN_DELIMITER = ";";
    char*  ptrParameter = nullptr; // NOLINT
    uint8_t paramCount = 1;                     // Zähler für die richtige Variable der struct
    EventClass* ptrEvent = new EventClass {};
    ptrEvent->setNext(nullptr);

    ptrParameter = strtok(inBuffer, TOKEN_DELIMITER);  // ptrParameter enthält jetzt den Stringabschnitt bis zum ersten Blank.
    if (ptrParameter != nullptr) {  // NOLINT modernize-use-nullptr ;vorsichtshalber testen, ob was gefunden wurde.
        strcpy(ptrEvent->device, ptrParameter);        // Diesen in device kopieren.
        while (ptrParameter != nullptr) {       // NOLINT Das Zerlegen fortsetzen bis nichts mehr da ist.
            paramCount++;   // Zähler hochzählen, für die richtige Variable der struct
                            // In die richtige Variable den Teilstring bis zum
            ptrParameter = strtok(nullptr, TOKEN_DELIMITER);   // NOLINT
            switch (paramCount) {               // jeweils nächsten Blank hineinkopieren.
                case 2: { strcpy(ptrEvent->event, ptrParameter); break; }
                case 3: { strcpy(ptrEvent->parameter1, ptrParameter); break; }
                case 4: { strcpy(ptrEvent->parameter2, ptrParameter); break; }
                default: ;  // mehr als 4 Tokens; das ist ein Fehler; die überzähligen Token ignorieren
            }
        }
    }
    return ptrEvent;
}
