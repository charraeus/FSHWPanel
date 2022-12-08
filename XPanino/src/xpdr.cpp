/*********************************************************************************************************//**
 * @file xpdr.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em ClockDavtronM803
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 ************************************************************************************************************/

#include <xpdr.hpp>

// Konstanten der möglichen Event-Strings
const uint8_t NO_OF_EVENT_STRINGS = 6;
const String eventStrings[NO_OF_EVENT_STRINGS] = {
    "BTN_OAT ON",
    "BTN_SEL ON", "BTN_SEL OFF",
    "BTN_CTL ON", "BTN_CTL LON", "BTN_CTL OFF"
};

/**************************************************************************************************
 * ClockDavtronM803 - public Methoden
 *
 **************************************************************************************************/

void ClockDavtronM803::setTimeMode(TimeModeTyp &timeMode) { this->timeMode = timeMode; };
void ClockDavtronM803::setLocalTime(uint32_t &localTime) { this->localTime = localTime; };
void ClockDavtronM803::setUtc(uint32_t &utc) { this->utc = utc; };
void ClockDavtronM803::setFlightTime(uint32_t &flightTime) { this->flightTime = flightTime; };
void ClockDavtronM803::setElapsedTime(uint32_t &elapsedTime) { this->elapsedTime = elapsedTime; };
void ClockDavtronM803::setOatVoltsMode(OatVoltsModeTyp &oatVoltsMode) {this->oatVoltsMode = oatVoltsMode; };
void ClockDavtronM803::setTemperature(int8_t &temperatureC) { this->temperatureC = temperatureC; };
void ClockDavtronM803::setPowerStatus(PowerStatusTyp &powerStatus) { this->powerStatus = powerStatus; };
void ClockDavtronM803::setAltimeter(float &altimeter) { this->altimeter = altimeter; };

/**************************************************************************************************/
void ClockDavtronM803::toggleOatVoltsMode() {
    int nextMode{static_cast<int>(oatVoltsMode)};

    nextMode++;
    if (nextMode > static_cast<int>(OatVoltsModeTyp::OAT_QNH)) {
        oatVoltsMode = OatVoltsModeTyp::OAT_VOLTAGE;
    } else {
        oatVoltsMode = static_cast<OatVoltsModeTyp>(nextMode);
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
void ClockDavtronM803::process(const EventClass* event, SwitchMatrix &switchMatrix, LedMatrix &ledMatrix) {

};


/** qnh
 * @brief Altimeter in Hg in QNH umrechnen
 *
 * 29,92 in Hg = 1013,25 hPa
 */
float ClockDavtronM803::qnh(float &altimeter) {
    const float STD_QNH_hPa = 1013.25;

    return STD_QNH_hPa / STD_ALTIMETER_inHg * altimeter;
}
