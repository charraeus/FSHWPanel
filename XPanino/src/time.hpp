/***************************************************************************************************
 * @file time.hpp
 * @author Christian Harraeus (christian@haraeus.de)
 * @brief
 * @version 0.1
 * @date 2022-12-29
 *
 * @copyright Copyright (c) 2022
 *
 *
 ***************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <UniversalTimer.hpp>

/***************************************************************************************************
 * @brief Class for manipulating and calculating with time
 *
 */
class TimeClass {
public:
    /**
     * @brief Construct a new Time Class object.
     *
     * @param initialTime HHMMSS
     */
    TimeClass(const uint32_t &initialTime);


    /**
     * @brief Set the time.
     *
     * @param newTime Timestamp to be the initial time HHMMSS
     */
    void setTime(const uint32_t &newTime);

    /**
     * @brief Set the new time.
     *
     * @param hours     Hours of the time stamp HH
     * @param minutes   Minutes of the time stamp MM
     * @param seconds   Seconds of the time stamp SS
     */
    void setTime(const uint8_t &hours, const uint8_t &minutes, const uint8_t &seconds);


    /**
     * @brief Get the current time.
     *
     * @return uint32_t Current time HHMMSS
     */
    uint32_t getTime();


    /**
     * @brief Has time changed since the last @em getTime()?
     * @return bool @em true if time has changed since the last @getTime(), @em false else
     *
     */
    bool isChanged();


    /**
     * @brief Get the current time as a well-formated string: The string will always be 6 characters long.
     *
     * @param withSeconds
     * @return String
     */
    String getFormattedTime(const bool &withSeconds);


    /**
     * @brief Update the time: Add 1 second
     * @return bool @em true if updates have been made, else @false
     */
    virtual bool update();


    uint8_t getSeconds();
    uint8_t getMinutes();
    uint8_t getHours();
    uint32_t getNextActionTime();
    unsigned int nextActionTime;


private:
    uint32_t currentTime;           ///< Current timestamp
    unsigned long lastChange;       ///< contains the millis()-timestamp of last time update
    bool changed {true};            ///< @true if time is changed and not yet retrieved
    UniversalTimer update1000msTimer {1000, true}; ///< Create a timer with 1000ms and repeating


    /**
     * @brief Add 1 second to oldTime and calculate the new time.
     *
     * @param oldTime   Time to be added by 1 second.
     * @return uint32_t New time
     */
    virtual uint32_t updateTimeBy1s(const uint32_t &oldTime);

};


/***************************************************************************************************
 * @brief
 *
 */
class FlightTimeClass : public TimeClass {
public:
    using TimeClass::TimeClass;
//     virtual void update();

private:

};


/***************************************************************************************************
 * @brief
 *
 */
class ElapsedTimeClass : public TimeClass {
public:
    using TimeClass::TimeClass;
//     virtual void update();

private:

};
