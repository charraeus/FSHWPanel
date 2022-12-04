/**************************************************************************************************
 * @file xpdr.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em ClockDavtronM803
 * @version 0.1
 * @date 2017-11-17
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
 **************************************************************************************************/

#include <xpdr.hpp>


// Konstanten der möglichen Event-Strings
const uint8_t NO_OF_EVENT_STRINGS = 6;
const String eventStrings[NO_OF_EVENT_STRINGS] = {
    "BTN_OAT ON", 
    "BTN_SEL ON", "BTN_SEL OFF", 
    "BTN_CTL ON", "BTN_CTL LON", "BTN_CTL OFF"
};


/**************************************************************************************************/
void ClockDavtronM803::toggleOatVoltMode() {
    int nextStatus{static_cast<int>(oatVoltsStatus)};

    nextStatus++;
    if (nextStatus > static_cast<int>(OatVoltsModeTyp::OAT_QNH)) { 
        oatVoltsStatus = OatVoltsModeTyp::OAT_VOLTAGE; 
    } else {
        oatVoltsStatus = static_cast<OatVoltsModeTyp>(nextStatus);
    }
};


/**************************************************************************************************/
void ClockDavtronM803::toggleTimeMode() {
    int nextStatus{static_cast<int>(timeMode)};

    nextStatus++;
    if (nextStatus > static_cast<int>(TimeModeTyp::TIME_FT)) { 
        timeMode = TimeModeTyp::TIME_LT; 
    } else {
        timeMode = static_cast<TimeModeTyp>(nextStatus);
    }
}


/**************************************************************************************************/
void ClockDavtronM803::process() {
    
};


/**************************************************************************************************/
Event ClockDavtronM803::event(const String &eventString) {
    Serial.print("CLOCK hat ein Event erhalten: "); Serial.println(eventString);
    Event ereignis = BTN_SEL_ON;    
    
//    // Event zurückgeben
//    for (uint8_t es = 0; es != NO_OF_EVENT_STRINGS; ++es) {
//        if (eventName == eventStrings[es]) {
//            return es;
//            break;
//        }
//    }
    return ereignis;
};
