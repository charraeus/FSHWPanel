/*********************************************************************************************************//**
 * @file m803.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em ClockDavtronM803.
 * @version 0.2
 * @date 2022-12-08
 *
 * Copyright © 2017 - 2022. All rights reserved.
 ************************************************************************************************************/

#include <device.hpp>
#include <m803.hpp>

#ifdef DEBUG
#define PRINT(P) Serial.print(#P);  Serial.print("="); Serial.println(P);
#endif

extern LedMatrix leds;


/**************************************************************************************************
 * ClockDavtronM803 - public Methoden
 *
 **************************************************************************************************/


ClockDavtronM803::ClockDavtronM803()
    : Device() {         //< Call Device-Constructor; @todo was ist der Unterschied zu using Device::Device; ?

    oatVoltsMode = OatVoltsModeState::EMF;  ///< Show EMF voltage in upper display.
    isOatVoltsModeChanged = true;
    clockMode = ClockModeState::LT;         ///< Lokale Zeit im unteren Display anzeigen.
    isClockModeChanged = true;

    ///< Define the upper display and show a default value.
    upperDisplay = 0;   ///< Das Display-Feld upperDisplay definieren. Es besteht aus 4 7-Segment-Anzeigen:
    leds.defineDisplayField(upperDisplay, 0, {0, 16});  ///< Die 1. 7-Segment-Anzeige liegt auf der Row 0 und den Cols 16 bis 23.
    leds.defineDisplayField(upperDisplay, 1, {1, 16});  ///< Die 2. 7-Segment-Anzeige liegt auf der Row 1 und den Cols 16 bis 23.
    leds.defineDisplayField(upperDisplay, 2, {2, 16});  ///< Die 3. 7-Segment-Anzeige liegt auf der Row 2 und den Cols 16 bis 23.
    leds.defineDisplayField(upperDisplay, 3, {3, 16});  ///< Die 4. 7-Segment-Anzeige liegt auf der Row 3 und den Cols 16 bis 23.
    leds.display(upperDisplay, "----");

    ///< Define the lower display and show a default value.
    lowerDisplay = 1;
    leds.defineDisplayField(lowerDisplay, 0, {4, 16});        ///< Die 1. 7-Segment-Anzeige liegt auf der Row 4 und den Cols 16 bis 23: Zehnerstelle der Stunde.
    leds.defineDisplayField(lowerDisplay, 1, {5, 16});        ///< Die 2. 7-Segment-Anzeige liegt auf der Row 5 und den Cols 16 bis 23: Einerstelle der Stunde.
    leds.defineDisplayField(lowerDisplay, 2, {6, 16});        ///< Die 3. 7-Segment-Anzeige liegt auf der Row 6 und den Cols 16 bis 23: Zehnerstelle der Minute.
    leds.defineDisplayField(lowerDisplay, 3, {7, 16});        ///< Die 4. 7-Segment-Anzeige liegt auf der Row 7 und den Cols 16 bis 23: Einerstelle der Minute.
    leds.display(lowerDisplay, "----");

    ///< Define the leds which do not belong to a displayField.
    LED_TRENNER_1 = {0, 4};         ///< Der obere Stunden-Minuten-Trenner liegt auf Row=0 und Col=4.
    leds.ledOff(LED_TRENNER_1);
    LED_TRENNER_2 = {1, 4};         ///< Der untere Stunden-Minuten-Trenner liegt auf Row=1 und Col=4.
    leds.ledOff(LED_TRENNER_2);
    LED_LT = {2, 4};                ///< Die LED "LT" liegt auf Row=2 und Col=4.
    leds.ledOff(LED_LT);
    LED_UT = {3, 4};                ///< Die LED "UT" liegt auf Row=3 und Col=4.
    leds.ledOff(LED_UT);
    LED_ET = {4, 4};                ///< Die LED "ET" liegt auf Row=2 und Col=4.
    leds.ledOff(LED_ET);
    LED_FT = {5, 4};                ///< Die LED "FT" liegt auf Row=3 und Col=4.
    leds.ledOff(LED_FT);
}


ClockModeState ClockDavtronM803::toggleClockMode() {
    int currentMode = static_cast<int>(clockMode);
    if (currentMode < static_cast<int>(ClockModeState::FT)) {
        currentMode++;
        clockMode = static_cast<ClockModeState>(currentMode);
    } else {
        clockMode = ClockModeState::LT;
    }
    isClockModeChanged = true;
    return clockMode;
}


OatVoltsModeState ClockDavtronM803::toggleOatVoltsMode() {
    int currentMode{static_cast<int>(oatVoltsMode)};
    if (currentMode < static_cast<int>(OatVoltsModeState::ALT)) {
        currentMode++;
        oatVoltsMode = static_cast<OatVoltsModeState>(currentMode);
    } else {
        oatVoltsMode = static_cast<OatVoltsModeState>(OatVoltsModeState::EMF);
    }
    isOatVoltsModeChanged = true;
    return oatVoltsMode;
};


void ClockDavtronM803::setSwitchEvent(const uint8_t switchId, const uint8_t switchStatus) {
    if (switchStatus == SWITCH_ON) {
        switch (switchId) {
            case CLOCK_BTN_CTL : { break; }
            case CLOCK_BTN_OAT : { toggleOatVoltsMode(); break; }
            case CLOCK_BTN_SEL : { toggleClockMode(); break; }
            default : ;
        }
    }
}


void ClockDavtronM803::setOatVoltsMode(OatVoltsModeState &oatVoltsMode) {this->oatVoltsMode = oatVoltsMode; };
void ClockDavtronM803::setTemperature(int8_t &temperatureC) { this->temperatureC = temperatureC; };
void ClockDavtronM803::setAltimeter(float &altimeter) { this->altimeter = altimeter; };


void ClockDavtronM803::updateAndProcess() {
    /// update flight and elapsed time objects
    flightTime.update();
    //elapsedTime.update();

    /// show the values and leds
    showUpperDisplay();
    showLowerDisplay();
}


/** qnh
 * @brief Altimeter inHg in QNH umrechnen.
 * @return float Calculated QNH.
 *
 * 29,92 in Hg = 1013,25 hPa
 */
float ClockDavtronM803::qnh() {
    const float STD_QNH_hPa = 1013.25;      ///< Standardluftdruck in Hektopascal

    return STD_QNH_hPa / STD_ALTIMETER_inHg * altimeter;
}


/**
 * @brief Calculate Fahrenheit from Celsius.
 * @return float Calculated temperature in Fahrenheit.
 */
inline float ClockDavtronM803::temperatureF() {
    return temperatureC * (9 / 5)+ 32;
}


/**
 * @brief Format a temperature value to exactly 3 digits. Considers the decimal point if necessary.
 *
 * @param temp      Temperature to be formatted.
 * @return String   Temperature with 3 digits.
 */
String ClockDavtronM803::formatTemperature(const float &temp) const {
    String s = static_cast<String>(temp);
    // cut the decimal if f > 99.9 or f < 0
    if (temp > 99.9) {
        // e. g. 101.23
        s = s.substring(0, 3);
    } else if (temp > 9.9) {
        // e.g. 10.3
        s = s.substring(0, 4);
    } else if (temp >= 0) {
        // e.g. 4.58
        s = ' ' + s.substring(0, 3);
    } else if (temp < 0) {
        // e.g. -6.2
        s = s.substring(0, 4);
    } else if (temp < -9) {
        // e.g. -10.3
        s = s.substring(0, 3);
    }
    return s;
}


void ClockDavtronM803::showUpperDisplay() {
    if (isDevicePowerAvailable()) {
        /// if device power is available show the values on the display
        if (isOatVoltsModeChanged || isTemperatureChanged || isEMFchanged || isAltimeterChanged) {
            switch (oatVoltsMode) {
                case OatVoltsModeState::EMF        : {
                            String s = static_cast<String>(emfVoltage);
                            if (emfVoltage < 10) {
                                // fill at the beginning with blank
                                s = ' ' + s;
                            }
                            s = s.substring(0, 4) + 'E';
                            leds.display(upperDisplay, s);
                            break;
                }
                case OatVoltsModeState::FAHRENHEIT : {
                            String s = formatTemperature(temperatureF());
                            s += 'F';
                            leds.display(upperDisplay, s);
                            break;
                }
                case OatVoltsModeState::CELSIUS    : {
                            String s = formatTemperature(temperatureC);
                            s += 'C';
                            leds.display(upperDisplay, s);
                            break;
                }
                case OatVoltsModeState::QNH        : {
                            float q = qnh();
                            String s = static_cast<String>(q);
                            if (q < 1000) {
                                s = ' ' + s.substring(0, 3);
                            } else {
                                s = s.substring(0, 4);
                            }
                            leds.display(upperDisplay, s);
                            break;
                }
                case OatVoltsModeState::ALT        : {
                            String s = static_cast<String>(altimeter);
                            leds.display(upperDisplay, s.substring(0, min(5, s.length())));
                            break;
                }
                default : {
                    // this must not ever happen!
                    leds.display(upperDisplay, "Err ");
                }
            }
            isOatVoltsModeChanged = false;
            isEMFchanged = false;
            isTemperatureChanged = false;
            isAltimeterChanged = false;
        }
    } else {
        /// if device power is not available => "switch off" the display
        leds.display(upperDisplay, "    ");
    }
}


void ClockDavtronM803::showLowerDisplay() {
    if (isDevicePowerAvailable()) {
        /// if device power is available set the leds
        if (isClockModeChanged || flightTime.isChanged() || elapsedTime.isChanged()) {
            switch (clockMode) {
                case ClockModeState::LT : {
                        if (isClockModeChanged) {
                            leds.ledOff(LED_FT);
                            leds.ledOn(LED_LT);
                            leds.ledOn(LED_TRENNER_1);
                            leds.ledBlinkOn(LED_TRENNER_1, BLINK_NORMAL);
                            leds.ledOn(LED_TRENNER_2);
                            leds.ledBlinkOn(LED_TRENNER_2, BLINK_NORMAL);
                        }
                        if (localTime.getHours(localTime.getTime(false)) != 99) {
                            leds.display(lowerDisplay, localTime.getFormattedTime(false));
                        } else {
                            leds.display(lowerDisplay, "nA-L");
                        }

                        break;
                }
                case ClockModeState::UT : {
                        if (isClockModeChanged) {
                            leds.ledOff(LED_LT);
                            leds.ledOn(LED_UT);
                            leds.ledOn(LED_TRENNER_1);
                            leds.ledBlinkOn(LED_TRENNER_1, BLINK_NORMAL);
                            leds.ledOn(LED_TRENNER_2);
                            leds.ledBlinkOn(LED_TRENNER_2, BLINK_NORMAL);
                        }
                        if (utc.getHours(utc.getTime(false)) != 99) {
                            leds.display(lowerDisplay, utc.getFormattedTime(false));
                        } else {
                            leds.display(lowerDisplay, "nA-U");
                        }
                        break;
                }
                case ClockModeState::ET : {
                        if (isClockModeChanged) {
                            leds.ledOff(LED_UT);
                            leds.ledOn(LED_ET);
                            leds.ledOff(LED_TRENNER_1);
                            leds.ledOff(LED_TRENNER_2);
                        }
                        leds.display(lowerDisplay, elapsedTime.getFormattedTime(false));
                        break;
                }
                case ClockModeState::FT : {
                        if (isClockModeChanged) {
                            leds.ledOff(LED_ET);
                            leds.ledOn(LED_FT);
                            if (flightTime.getHours(flightTime.getTime(false)) == 0) {
                                leds.ledOn(LED_TRENNER_1);
                                leds.ledBlinkOff(LED_TRENNER_1, BLINK_NORMAL);
                                leds.ledOn(LED_TRENNER_2);
                                leds.ledBlinkOff(LED_TRENNER_2, BLINK_NORMAL);
                            } else {
                                leds.ledOn(LED_TRENNER_1);
                                leds.ledBlinkOn(LED_TRENNER_1, BLINK_NORMAL);
                                leds.ledOn(LED_TRENNER_2);
                                leds.ledBlinkOn(LED_TRENNER_2, BLINK_NORMAL);
                            }
                        }
                        String timeString = flightTime.getFormattedTime(true);
                        if (flightTime.getHours(flightTime.getTime(false)) == 0) {
                            // display minutes and seconds: MMSS
                            leds.display(lowerDisplay, timeString.substring(2, 6));
                        } else {
                            // display hours and minutes: HHMM
                            leds.display(lowerDisplay, timeString.substring(0, 4));
                        }
                    break;
                }
                default : {
                    // this must not ever happen!
                    leds.display(lowerDisplay, "Err ");
                    leds.ledOff(LED_TRENNER_1);
                    leds.ledOff(LED_TRENNER_2);
                }
            }
        }
    } else {
        /// if device power is not available "switch off" the display and switch off all leds
        leds.display(lowerDisplay, "    ");
        leds.ledOff(LED_TRENNER_1);
        leds.ledOff(LED_TRENNER_2);
        leds.ledOff(LED_LT);
        leds.ledOff(LED_UT);
        leds.ledOff(LED_FT);
        leds.ledOff(LED_ET);
    }
}
