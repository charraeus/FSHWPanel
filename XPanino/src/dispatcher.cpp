/*********************************************************************************************************//**
 * @file dispatcher.cpp
 * @brief Dispatcher-Klasse
 *
 * @author Christian Harraeus (christian@harraeus.de)
 * @date 2022-12-13
 * @version 0.2
 *
 ***********************************************************************************************************/

#include <dispatcher.hpp>

extern EventQueueClass eventQueue;


void DispatcherClass::dispatch(EventClass *event) const {
    if (strcmp(event->device, DEVICE_M803) == 0) {
        m803.process(event);
    } else if (strcmp(event->device, DEVICE_XPDR) == 0) {
        //xpdr.process(event);
    } else {
        // kein passendes Device gefunden.
    }
}


void DispatcherClass::dispatchAll() {
    EventClass* ptr = eventQueue.getHeadEvent();
    while (ptr != nullptr) {
        Serial.println(F("---DispatchAll---"));
        dispatch(ptr);
        eventQueue.deleteHeadEvent(ptr);
        ptr = eventQueue.getHeadEvent();
    };
}