/**************************************************************************************************
 * @file dispatcher.hpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Interface der Klasse @em Dispatcher
 * @version 0.1
 * @date 2017-11-17
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
 **************************************************************************************************/

#ifndef dispatcher_hpp
#define dispatcher_hpp

#include <Arduino.h>
#define _BUFFER_LENGTH 100  ///< Der Buffer kann max. 99 Zeichen zzgl. Zeilenende '\0' aufnehmen.

/**
 * @brief Puffer für die Ein-/Ausgabe von Zeichen von/an der/die serielle Schnittstelle
 * 
 */
class BufferClass {
public:
    char *get() { return buffer; }
    uint8_t addChar(const char inChar);
    void wipe() { actPos = 0; buffer[actPos] = '\0'; }
    bool isEmpty() { return strlen(buffer) == 0; }
private:
    char buffer[_BUFFER_LENGTH]{'\0'};      ///< Zeichenpuffer der Länge _BUFFER_LENGTH
    unsigned int actPos{0};                ///< aktuelles Ende des Buffers; zeigt auf die Position nach dem letzten Zeichen
};

/**
 * @brief Daten vom Flugsimulator entgegennehmen und verarbeiten.
 * @todo Doku ergänzen
 */
class Dispatcher {
public:
    Dispatcher();

    /**
     * @brief Den vom Flugsimulator erhaltenen String parsen und in 5 Parameter zerlegen.
     * Im vom Flugsimulator enthaltenen String werden folgende Informationen erwartet, 
     * jeweils getrennt durch ein Blank. Es müssen immer diese 5 Parameter übertragen werden:
     * - source     - Quelle. wird wohl in der Regel "XP" für X-Plane sein.
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
     * @param [in] inInt Byte (Zeichen), das von der seriellen Schnittstelle gelesen wurde. 
     */
    void parseSerial(const char inChar);

    /**
     * @brief 
     * @todo Doku ergänzen
     * @param [in] inCommandString 
     */
    void parseString(const char *inBuffer);

    #ifdef DEBUG
    void printData();
    #endif

private:
    uint8_t status = 0;
    String command;
    struct Parser {
        String source;         ///< ??
        String device;         ///< Avionic-Gerät, von/zu dem die Aktion stammt/gehört.
        String devEvent;       ///< Ereignis, das aufgetreten ist, z.B. "Schalter angeschaltet".
        String parameter1;     ///< Parameter zum Ereignis, z.B. die Schalterbezeichnung.
        String parameter2;     ///< Parameter zum Ereignis, z.B. die Schalterbezeichnung.
    } commandData {"source", "device", "devevent", "para1", "para2"};
    
    void dispatch();
    void dispatch(const String &inSource, const String &inDevice, const String &inEvent,
                  const String &inParameter1, const String &inParameter2);
};

#endif /* dispatcher_hpp */
