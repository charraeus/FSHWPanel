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
     * @brief Set the time. The changed-attribute is set to true.
     *
     * @param newTime Timestamp to be the initial time HHMMSS
     */
    void setTime(const uint32_t &newTime);

    /**
     * @brief Set the new time. The changed-attribute is set to true.
     *
     * @param hours     Hours of the time stamp HH
     * @param minutes   Minutes of the time stamp MM
     * @param seconds   Seconds of the time stamp SS
     */
    void setTime(const uint8_t &hours, const uint8_t &minutes, const uint8_t &seconds);


    /**
     * @brief Get the current time.
     *        The changed-attribute is set to false if @em resetChangedStatus is @em false.
     * @param resetChange (optional) If @true the changed status of the time object is set @em true, else to @em false.
     * @return uint32_t Current time HHMMSS
     */
    uint32_t getTime(const bool resetChangedStatus = true);


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
     * @brief Update the time: Add 1 second.
     * @return bool @em true if updates have been made, else @false.
     */
    virtual bool update();


    /**
     * @brief Get the seconds of a time value.
     *        e.g. 123456 --> seconds = 56.
     *
     * @return uint8_t Seconds.
     */
    uint8_t getSeconds(const uint32_t &timeValue);


    /**
     * @brief Get the minutes of a time value.
     *        e.g. 123456 --> 1234.56 --> 1234 --> minutes = 34.
     *
     * @return uint8_t Minutes.
     */
    uint8_t getMinutes(const uint32_t &timeValue);


    /**
     * @brief Get the hours of a time value.
     *        e.g. 123456 --> 12.3456 --> 12 --> hours = 12.
     *
     * @return uint8_t Hours.
     */
    uint8_t getHours(const uint32_t &timeValue);


    /**
     * @brief Convert separate values for hours, minutes and seconds to a time value.
     *        e.g. hours = 12, minutes = 34, seconds = 56 --> return the value 123456.
     *
     * @param hours
     * @param minutes
     * @param seconds
     * @return uint32_t The time value composed of hours, minutes and seconds.
     */
    uint32_t buildTime(const uint8_t &hours, const uint8_t &minutes, const uint8_t &seconds);


    uint32_t getNextActionTime();
    unsigned int nextActionTime;


private:
    uint32_t currentTime {};           ///< Current timestamp
    unsigned long lastChange {};       ///< contains the millis()-timestamp of last time update
    bool changed {true};               ///< @true if time is changed and not yet retrieved
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
using TimeClass::TimeClass;

public:

    uint32_t updateTimeBy1s(const uint32_t &oldTime) override;
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
