/*********************************************************************************************************
 * @file tools.hpp
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
#pragma once

#include <Arduino.h>

#if defined DEBUG && ! defined NOT_WITH_SERIAL

    #ifndef PRINT
    #define PRINT(P) Serial.print(#P);  Serial.print("="); Serial.println(P);
    #endif

    void printBinValue(const uint8_t value);

#endif
