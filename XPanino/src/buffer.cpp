/*********************************************************************************************************//**
 * @file buffer.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em BufferClass.
 * @version 0.1
 * @date 2022-12-12
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 ************************************************************************************************************/

#include <buffer.hpp>
#include <event.hpp>


/*********************************************************************************************************//**
 * BufferClass - public Methoden
 *
 ************************************************************************************************************/

uint8_t BufferClass::addChar(const char inChar) {
    if (actPos < MAX_BUFFER_LENGTH) {
        buffer[actPos] = inChar;
        actPos += 1;
        buffer[actPos] = '\0';
        return 0;
    }
    return 1;
}

void BufferClass::wipe() { actPos = 0; buffer[actPos] = '\0'; }


EventClass* BufferClass::parseString(char *inBuffer) {
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
