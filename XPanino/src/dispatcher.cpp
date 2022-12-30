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
#define PRINT(P) Serial.print(#P);  Serial.print(" ="); Serial.println(P);

void DispatcherClass::dispatch(EventClass *event) const {
    if (strcmp(event->device, DEVICE_M803) == 0) {
        m803.processEvent(event);
    // } else if (strcmp(event->device, DEVICE_XPDR) == 0) {
    //     xpdr.processEvent(event);
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


/**
 * @brief Dispatch the switch events to the right device.
 *
 */
void DispatcherClass::dispatchSwitchEvents(const uint8_t &row, const uint8_t &col, uint8_t switchState) {
    uint8_t id = switchId[row][col];
    PRINT(id);

    switch (id) {
        case CLOCK_BTN_CTL :
        case CLOCK_BTN_OAT :
        case CLOCK_BTN_SEL : { m803.setSwitchEvent(id, switchState); break; }
        default            : ;
    }
}


char *DispatcherClass::getSwitchName(const uint8_t &row, const uint8_t &col) {
    static char buffer[4];
    strcpy(buffer, switchNames[row][col]);     // copy switch name into buffer
    return buffer;                          // return pointer to the beginning of the buffer
}


//     switch (row) {
//         // row 0
//         case 0 : {
//             switch (col) {
//                 // row 0, col 0: Transponder button IDT
//                 case 0 : {
//                     break;
//                 }
//                 // row 0, col 1: Transponder button VFR
//                 case 1 : {
//                     break;
//                 }
//                 // row 0, col 2: Transponder button CLR
//                 case 2 : {
//                     break;
//                 }
//                 // row 0, col 3: Transponder mode switch OFF
//                 case 3 : {
//                     break;
//                 }
//                 // row 0, col 4: Transponder mode switch SBY
//                 case 4 : {
//                     break;
//                 }
//                 // row 0, col 5: Transponder mode switch TST
//                 case 5 : {
//                     break;
//                 }
//                 // row 0, col 6: Transponder mode switch ON
//                 case 6 : {
//                     break;
//                 }
//                 // row 0, col 7: Transponder mode switch ALT
//                 case 7 : {
//                     break;
//                 }
//                 default : {
//                     // Invalid column
//                     // this must not ever happen!
//                 }
//             }
//         }
//         // row 1
//         case 1 : {
//             switch (col) {
//                 // row 1, col 0: Transponder button 0
//                 case 0 : {
//                     break;
//                 }
//                 // row 1, col 1: Transponder button 1
//                 case 1 : {
//                     break;
//                 }
//                 // row 1, col 2: Transponder button 2
//                 case 2 : {
//                     break;
//                 }
//                 // row 1, col 3: Transponder button 3
//                 case 3 : {
//                     break;
//                 }
//                 // row 1, col 4: Transponder button 4
//                 case 4 : {
//                     break;
//                 }
//                 // row 1, col 5: Transponder button 5
//                 case 5 : {
//                     break;
//                 }
//                 // row 1, col 6: Transponder button 6
//                 case 6 : {
//                     break;
//                 }
//                 // row 1, col 7: Transponder button 7
//                 case 7 : {
//                     break;
//                 }
//                 default : {
//                     // Invalid column
//                     // this must not ever happen!
//                 }
//             }
//         }
//         // row 2
//         case 2 : {
//             switch (col) {
//                 // row 2, col 0: Clock button SELECT
//                 case 0 : {
//                     break;
//                 }
//                 // row 2, col 1: Clock button CONTROL
//                 case 1 : {
//                     break;
//                 }
//                 // row 2, col 2: Clock button OAT
//                 case 2 : {
//                     break;
//                 }
//                 // row 2, col 3: currently not connected
//                 case 3 : {
//                     break;
//                 }
//                 // row 2, col 4: currently not connected
//                 case 4 : {
//                     break;
//                 }
//                 // row 2, col 5: currently not connected
//                 case 5 : {
//                     break;
//                 }
//                 // row 2, col 6: currently not connected
//                 case 6 : {
//                     break;
//                 }
//                 // row 2, col 7: currently not connected
//                 case 7 : {
//                     break;
//                 }
//                 default : {
//                     // Invalid column
//                     // this must not ever happen!
//                 }
//             }
//         }
//         // Invalid row
//         default : {
//             // this must not ever happen!
//         }
//     }
// }
