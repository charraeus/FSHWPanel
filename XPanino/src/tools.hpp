/*********************************************************************************************************
 * @file tools.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Diverse Tools, u.A. um unabhängig von Arduino.h zu sein.
 *
 * @version 0.1
 * @date 2023-04-08
 *
 * Copyright © 2023. All rights reserved.
 *
 *
 ************************************************************************************************************/
#pragma once

#include <Arduino.h>

#if defined DEBUG && ! defined NOT_WITH_SERIAL

    #ifndef PRINT
    #define PRINT(P) Serial.print(#P); Serial.print("="); Serial.println(P);
    #endif

    #ifndef PRINTBIN
    #define PRINTBIN(P) Serial.print(#P); Serial.print("="); PRINT_BIN_VALUE(P); Serial.println();
    #endif

    #ifndef PRINT_BIN_VALUE
    #define PRINT_BIN_VALUE(V) for (uint8_t bit = 0b10000000; bit > 0; \
                                    Serial.print(V & bit ? '1' : '0'), bit>>= 1);
    #endif

#endif
