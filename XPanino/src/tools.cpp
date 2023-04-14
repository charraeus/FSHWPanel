/*********************************************************************************************************
 * @file tools.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Diverse Tools, u.A. um unabhängig von Arduino.h zu sein.
 *
 * @version 0.1
 * @date 2023-0408
 *
 * Copyright © 2023. All rights reserved.
 *
 *
 ************************************************************************************************************/
#include <tools.hpp>

#if defined DEBUG && !defined NOT_WITH_SERIAL
void printBinValue(const uint8_t value) {
    uint8_t bit = 0b10000000;
    do {
        Serial.print(value & bit ? '1' : '0');
        bit >>= 1;
    } while (bit > 0);
};
#endif
