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
 * Konstanten für Devices und Actions
 *
 * @todo: auf numerische Werte umstellen; ggf. commands.hpp nutzen
 ************************************************************************************************************/

/*********************************************************************************************************//**
 * @brief Event - public Methoden
 *
 ************************************************************************************************************/

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


/*********************************************************************************************************//**
 * @brief Eventliste - public Methoden
 *
 ************************************************************************************************************/
EventListClass::EventListClass() {
    head = nullptr;
    tail = nullptr;
};

void EventListClass::addEvent(EventClass* ptrNewEvent) {
    if (ptrNewEvent != nullptr) {
        // #ifdef DEBUG
        // Serial.print("addEvent A: Device="); Serial.print(ptrNewEvent->device); Serial.println("|");
        // Serial.print("addEvent A: Event="); Serial.print(ptrNewEvent->event); Serial.println("|");
        // Serial.print("addEvent A: Parameter1="); Serial.print(ptrNewEvent->parameter1); Serial.println("|");
        // Serial.print("addEvent A: Parameter2="); Serial.print(ptrNewEvent->parameter2); Serial.println("|");
        // #endif

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
        // #ifdef DEBUG
        // Serial.print("addEvent E: Device="); Serial.print(head->device); Serial.println("|");
        // Serial.print("addEvent E: Event="); Serial.print(head->event); Serial.println("|");
        // Serial.print("addEvent E: Parameter1="); Serial.print(head->parameter1); Serial.println("|");
        // Serial.print("addEvent E: Parameter2="); Serial.print(head->parameter2); Serial.println("|");
        // Serial.print("addEvent E: head->next=");
        // if (head->getNext() == nullptr) { Serial.println("<nullptr>");
        // } else { Serial.println("<pointer>"); }
        // #endif
    }
};

EventClass *EventListClass::getNextEvent() {
    EventClass* ptr = head;     // Zeiger auf 1. Element sichern
    head = head->getNext();     // head auf das 2. Element zeigen lassen
    return ptr;                 // Zeiger auf das bisherige 1. Element zurück geben
};

void EventListClass::deleteEvent() {
    if (head != nullptr) {
        // Liste ist nicht leer
        EventClass* ptr = head;     // Zeiger auf 1. Element sichern
        head = head->getNext();     // head auf das 2. Element zeigen lassen
        delete(ptr);                // Das bisherige 1. Element löschen
    }
};

#ifdef DEBUG
void EventListClass::listEvents() {
    EventClass* ptr = head;
    while (ptr != nullptr) {
        Serial.print("Device="); Serial.print(ptr->device); Serial.println("|");
        Serial.print("Event="); Serial.print(ptr->event); Serial.println("|");
        Serial.print("Parameter1="); Serial.print(ptr->parameter1); Serial.println("|");
        Serial.print("Parameter2="); Serial.print(ptr->parameter2); Serial.println("|");
        if (ptr->getNext() == nullptr) { Serial.println("<nullptr>");
        } else { Serial.println("<pointer>"); }
        ptr = ptr->getNext();
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
    #ifdef DEBUG
    Serial.print("parseString: Device="); Serial.print(ptrEvent->device); Serial.println("|");
    Serial.print("parseString: Event="); Serial.print(ptrEvent->event); Serial.println("|");
    Serial.print("parseString: Parameter1="); Serial.print(ptrEvent->parameter1); Serial.println("|");
    Serial.print("parseString: Parameter2="); Serial.print(ptrEvent->parameter2); Serial.println("|");
    Serial.print("parseString: ptrEvent->next=");
    if (ptrEvent->getNext() == nullptr) { Serial.println("<nullptr>");
    } else { Serial.println("<pointer>"); }
    #endif
    return ptrEvent;
}
