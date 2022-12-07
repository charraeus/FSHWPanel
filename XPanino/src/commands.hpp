/*********************************************************************************************************//**
 * @file commands.hpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Kommandocodes für die Kommunikation zwischen Arduino und PC.
 * @version 0.1
 * @date 2020-04-11
 *
 * Copyright © 2017 - 2020. All rights reserved.
 *
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>

/*********************************************************************************************************//**
 * @brief Kommoandocodes
 ************************************************************************************************************/
const uint16_t ACK               = 0xFFFF;    ///< Acknowledge - Angeforderte Daten für Parameter Code folgen

const uint16_t RESET_ARDUINO     = 0xFF01;    ///< Arduino neu booten
const uint16_t RESEND_SWITCHES   = 0xFF02;    ///< Den Status aller Schalter senden

const uint16_t XPDR_CODE         = 0xF101;    ///< Den übergebenen XPDR-Code anzeigen
const uint16_t XPDR_FLIGHTLEVEL  = 0xF102;    ///< Flightlevel für Transponder

const uint16_t M803_LT           = 0xF104;    ///< Aktuelle Uhrzeit (Local)
const uint16_t M803_UT           = 0xF103;    ///< Aktuelle Uhrzeit (UTC)
const uint16_t M803_ET           = 0xF105;    ///< Elapsed Time
const uint16_t M803_FT           = 0xF106;    ///< Flight Time
const uint16_t M803_OATF         = 0xF100;    ///< O.A.T. in Fahrenheit
const uint16_t M803_OATC         = 0xF100;    ///< O.A.T. in Grad Celsius
const uint16_t M803_VOLTS        = 0xF107;    ///< Spannung in V
const uint16_t M803_QNH          = 0xF201;    ///< Aktuelles QNH des X-Plane-Wetters
const uint16_t M802_ALT          = 0xF202;    ///< Aktueller Druck in inHg des X-Plane-Wetters
