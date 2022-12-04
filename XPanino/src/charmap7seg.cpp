/**************************************************************************************************
 * @file charmap7seg.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em LedMatrix und viele Konstanten zur Anzeige.
 * @version 0.1
 * @date 2017-11-10
 *
 * Copyright © 2017 - 2020. All rights reserved.
 *
**************************************************************************************************/

#include <Arduino.h>
#include <charmap7seg.hpp>

const uint8_t Led7SegmentCharMap::bitMap[] =  {    ///< charMap enthält das Bitmuster, die die Ziffer
                            ///< bzw. den Buchstaben darstellen.
        //gfedcba
        // Ziffern
        0b0111111, ///<  "0": Segmente f, e, d, c, b, a    --> bitMap[0]
        0b0000110, ///<  "1": Segmente c, b                --> bitMap[1]
        0b1011011, ///<  "2": Segmente g, e, d, b, a       --> bitMap[2]
        0b1001111, ///<  "3": Segmente g, d, c, b, a       --> bitMap[3]
        0b1100110, ///<  "4": Segmente g, f, c, b          --> bitMap[4]
        0b1101101, ///<  "5": Segmente g, f, d, c, a       --> bitMap[5]
        0b1111101, ///<  "6": Segmente g, f, e, d, c, a    --> bitMap[6]
        0b0000111, ///<  "7": Segmente c, b, a             --> bitMap[7]
        0b1111111, ///<  "8": Segmente g, f, e, d, c, b, a --> bitMap[8]
        0b1101111, ///<  "9": Segmente g, f, d, c, b, a    --> bitMap[9]
        // Großbuchstaben A..U; V, W, X, Y, Z entfallen, da nicht darstellbar
        0b0000000, ///<  " ": alle Segmente aus            --> bitMap[10]
        0b1110111, ///<  "A": Segmente g, f, e, b, c, a .  --> bitMap[11]
        0b1111100, ///<  "b": Segmente g, f, e, d, c       --> bitMap[12]
        0b0111001, ///<  "C": Segmente f, e, d, a          --> bitMap[13]
        0b1011110, ///<  "d": Segmente g, e, d, c, b       --> bitMap[14]
        0b1111001, ///<  "E": Segmente g, f, e, d, a       --> bitMap[15]
        0b1110001, ///<  "F": Segmente g, f, e, a          --> bitMap[16]
        0b0110110, ///<  "G": Segmente g, f, e, c, b       --> bitMap[17]
        0b1110110, ///<  "H": Segmente g, f, e, c, b       --> bitMap[18]
        0b0000110, ///<  "I": Wie "1"                      --> bitMap[19]
        0b0001110, ///<  "J": Segmente d, c, b             --> bitMap[20]
        0b1001001, ///<  "K": Drei waagerechte Striche: Segmente g, d, a  --> bitMap[21]
        0b0111000, ///<  "L": Segmente f, e, d             --> bitMap[22]
        0b1001001, ///<  "M": Drei waagerechte Striche: Segmente g, d, a  --> bitMap[23]
        0b1001001, ///<  "N": Drei waagerechte Striche: Segmente g, d, a  --> bitMap[24]
        0b0111111, ///<  "O": Segmente f, e, d, c, b, a    --> bitMap[25]
        0b1110011, ///<  "P": Segmente g, f, e, b, a       --> bitMap[26]
        0b1100111, ///<  "q": Segmente g, f, c, b, a       --> bitMap[27]
        0b1010000, ///<  "r": Segmente g, e                --> bitMap[28]
        0b1101101, ///<  "S": Wie "5"                      --> bitMap[29]
        0b0110001, ///<  "T": Segmente  f, e, a            --> bitMap[30]
        0b0111110, ///<  "U": Segmente f, e, d, c, b       --> bitMap[31]
        // "Sonderzeichen"
        0b1011000, ///<  "c": Segmente g, e, d, c          --> bitMap[32]
        0b1011100, ///<  "o": Segmente g, e, d, c          --> bitMap[33]
        0b0011100, ///<  "u": Segmente e, d, c             --> bitMap[34]
        0b1000000, ///<  "-": Segment g                    --> bitMap[35]
        0b1100011, ///<  "°": Segment g, f, b, a           --> bitMap[36]
        0b0001000, ///<  "_": Segment d                    --> bitMap[37]
        0b1001001, ///< Drei waagerechte Striche: Segmente g, d, a  --> bitMap[38]
        0b0110110  ///< Zwei senkrechte Striche:  Segmente f, e, c, b --> bitMap[39]
    };


// BitMap zur Darstellung auf der 7-Segment-Anzeige aus Zeichen bzw. Index ermitteln
uint8_t Led7SegmentCharMap::get7SegBitMap(const char outChar) const {
    return bitMap[getCharMapIndex(outChar)];
};


// Indexwert für charMap aus ASCII-Zeichen ermitteln
uint8_t Led7SegmentCharMap::getCharMapIndex(const char outChar) const {
    uint8_t charMapIndex = CHAR_ERROR;  // Index für die Zeichentabelle

    // Wenn das outChar numerisch oder alfanumerisch ist,
    // den Index in der Zeichentabelle ausrechnen
    if (isDigit(outChar)) {
        charMapIndex = static_cast<uint8_t>(outChar)
            - static_cast<uint8_t>(DIFF_CHARMAP_ASCII_NUMBERS);
    }
    if (isSpace(outChar)) {
        charMapIndex = CHAR_BLANK;
    }
    if (isAlpha(outChar)) {
        // erst auf die "Sonderzeichen" prüfen
        switch (outChar) {
            case 'c':               charMapIndex = CHAR_c; break;
            case 'o':               charMapIndex = CHAR_o; break;
            case 'u':               charMapIndex = CHAR_u; break;
            // kein "Sonderzeichen", dann muss es ein Buchstabe sein
            default: {
                charMapIndex = static_cast<uint8_t>(outChar)
                    - static_cast<uint8_t>(DIFF_CHARMAP_ASCII_ALFA);
            }
        }
    };
    if (charMapIndex == CHAR_ERROR) {
        // Bisher nix gefunden, d.h. jetzt können nur
        // noch "echte Sonderzeichen" übrig sein
        // Wird hier auch nix gefunden, bleibt es halt bei CHAR_ERROR
        switch (outChar) {
            case '-':               charMapIndex = CHAR_MINUS; break;
            case '_':               charMapIndex = CHAR_UNDERSCORE; break;
            case CHAR_DEGREE:       charMapIndex = CHAR_DEGREE; break;
            case CHAR_3_DASH_HORIZ: charMapIndex = CHAR_3_DASH_HORIZ; break;
            case CHAR_2_DASH_VERT:  charMapIndex = CHAR_2_DASH_VERT; break;
            default:                charMapIndex = CHAR_ERROR;
        }
    }
    return charMapIndex;
};
