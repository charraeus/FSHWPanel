/***************************************************************************************************
 * @file time.cpp
 * @author Christian Harraeus (christian@haraeus.de)
 * @brief
 * @version 0.1
 * @date 2022-12-29
 *
 * @copyright Copyright (c) 2022
 *
 *
 ***************************************************************************************************/

#include <time.hpp>

#ifdef DEBUG
#define PRINT(P) Serial.print(#P);  Serial.print("="); Serial.println(P);
#endif


/**************************************************************************************************
 * TimeClass - public Methoden
 *
 **************************************************************************************************/


TimeClass::TimeClass(const uint32_t &initialTime) {
    setTime(initialTime);
    update1000msTimer.start();
}


void TimeClass::setTime(const uint32_t &newTime) {
    currentTime = newTime;
    lastChange = millis();
    changed = true;
}


void TimeClass::setTime(const uint8_t &hours, const uint8_t &minutes, const uint8_t &seconds) {
    setTime(seconds + minutes * 100 + hours * 10000);
}


uint32_t TimeClass::getTime(){
    changed = false;
    return currentTime;
}


bool TimeClass::isChanged() {
    return changed;
}


String TimeClass::getFormattedTime(const bool &withSeconds) {
    String t = static_cast<String>(getTime());
    while (t.length() < 6) {
        t = '0' + t;
    }
    if (! withSeconds) {
        return t.substring(0, 4);
    }
    return t;
}


uint8_t TimeClass::getSeconds() {
    return currentTime % 100;
}


uint8_t TimeClass::getMinutes() {
    return (currentTime / 100) % 100;
}


uint8_t TimeClass::getHours() {
    return currentTime / 10000;
}


uint32_t TimeClass::getNextActionTime() {
    return nextActionTime;
}


bool TimeClass::update() {
    if (update1000msTimer.check()) {
        setTime(updateTimeBy1s(getTime()));
        return true;
    }
    return false;
}


/**************************************************************************************************
 * TimeClass - private Methoden
 *
 **************************************************************************************************/

uint32_t TimeClass::updateTimeBy1s(const uint32_t &oldTime) {
    uint8_t seconds = oldTime % 100;            // e.g. 123456 --> seconds = 56
    uint8_t minutes = (oldTime / 100) % 100;    // e.g. 123456 --> 1234.56 --> 1234 --> minutes = 34
    uint8_t hours = (oldTime / 10000);          // e.g. 123456 --> 12.3456 --> 12 --> hours = 12

    seconds++;
    if (seconds > 59) {
        seconds = 0;
        minutes++;
        if (minutes > 59) {
            minutes = 0;
            hours++;
            if (hours > 23) {
                hours = 0;
            }
        }
    }
    return hours * 10000 + minutes * 100 + seconds;
}


/**************************************************************************************************
 * FlightTimeClass - public Methoden
 *
 **************************************************************************************************/

// void FlightTimeClass::update() {

// }


/**************************************************************************************************
 * ElapsedTimeClass - public Methoden
 *
 **************************************************************************************************/

// void ElapsedTimeClass::update() {

// }
