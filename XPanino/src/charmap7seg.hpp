/*********************************************************************************************************//**
 * @file charmap7seg.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Stellt Bitmuster für die Anzeige von Zeichen auf 7-Segment-Anzeigen bereit.
 * @version 0.1
 * @date 2022-12-03
 *
 * Copyright © 2022 - .... All rights reserved.
 *
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>

/** charMap-Index-Konstanten für die Anzeige von Sonderzeichen auf den 7-Segment-Anzeigen */
const uint8_t CHAR_3_DASH_HORIZ = 36;           ///< Zeichen Drei "-" übereinander
const uint8_t CHAR_2_DASH_VERT = 37;            ///< Zeichen "||"
const uint8_t CHAR_ERROR = CHAR_3_DASH_HORIZ;   ///< Zeichen für Fehler


/*********************************************************************************************************//**
 * @brief Bildet ein 7-Segment-Display mit Dezimalpunkt ab und liefert Bitmuster
 *        für die Anzeige der Werte auf den 7-Segment-Anzeigen.
 *
 *       a
 *    -------
 *    |     |
 *  f |     | b
 *    |  g  |
 *    -------
 *    |     |
 *  e |     | c
 *    |     |
 *    -------
 *       d
 *
 * charMap enthält das Bitmuster, die die Ziffer/den Buchstaben darstellen.
 * a wird dabei durch das niederstwertige Bit repräsentiert, g durch das höchstwertige Bit.
 *
 * Beispiel:
 * - Auf den 7 Digits des XPDR die Ziffernfolge 0..6 anzeigen
 * - Die Dezimalpunkte im Digit 2 und 6 anzeigen
 * - Die LED R anzeigen
 * - Auf den oberen 4 Digits der Uhr folgende Zeichen anzeigen:
 *   -- Drei waagerechte Striche
 *   -- Minus
 *   -- E und Dezimalpunkt
 *   -- r
 * - Auf den unteren 4 Digits der Uhr folgende Zeichen anzeigen:
 *   -- kleines o und Dezimalpunkt
 *   -- F
 *   -- C
 *   -- Zwei senkrechte Striche
 * - Die Minutentrenner-LEDs der Uhr anzeigen
 * - Die LED UT anzeigen
 * vgl. Doku "Belegung der LED-Matrix des 1. IO-Warrior" in Evernote
 *          most sig. Byte                least sig. Byte
 *                |                              |
 *                v                              v
 *                7      07      07      07      0
 *    matrix[0] = 0b00000000010010010011111100010000;
 *    matrix[1] = 0b00000000010000000000011000010000;
 *    matrix[2] = 0b00000000111110011101101100000000;
 *    matrix[3] = 0b00000000010100000100111100010000;
 *    matrix[4] = 0b00000000110111001110011000000000;
 *    matrix[5] = 0b00000000011000110110110100000000;
 *    matrix[6] = 0b00000000001110010111110100000000;
 *    matrix[7] = 0b00000000001101100000000000010000;
 *
 * Die Reihenfolge der Ziffern, Buchstaben und Sonderzeiche entspricht der Reihenfolge
 * in einer ASCII-Tabelle, so dass die Codes aus der ASCII-Tabelle zum Errechnen des
 * Index' in der charMap verwendet werden können.
 *
 * Die Buchstaben V, W, X, Y, Z fehlen, da diese mit 7 Segmenten nicht darstellbar sind.
 * Einige weitere Buchstaben (z.B. K) sind ebenfalls nicht darstellbar, aber um die
 * relative Position innerhalb der charMap entsprechend der ASCII-Tabelle beizubehalten,
 * werden diese Buchstaben durch drei waagerechte Striche dargestellt.
 * Dies betrifft K, M und N.
 *
 ************************************************************************************************************/
class Led7SegmentCharMap {
public:
    /**
     * @brief BitMap zur Darstellung auf der 7-Segment-Anzeige für outChar ermitteln.
     *
     * @param outChar Zeichen, das ausgegeben werden soll.
     * @return uint8_t Bitmap, durch die das Zeichen auf der 7-Seg.-Anzeige dargestellt wird.
     */
    uint8_t get7SegBitMap(char outChar) const;

private:
    static const uint8_t bitMap[];      ///< Bitmap-Tabelle mit den Bitmustern
    static const char* charsAllowed;    ///< Erlaubte Zeichen, die auf 7-Segment-Anzeigen ausgegeben werden können
};
