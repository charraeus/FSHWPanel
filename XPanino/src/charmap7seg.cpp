/***************************************************************************************************
 * @file charmap7seg.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em Led7SegmentCharMap und viele Konstanten zur Anzeige.
 * @version 0.1
 * @date 2017-11-10
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 **************************************************************************************************/

#include <Arduino.h>
#include <charmap7seg.hpp>


/***************************************************************************************************
 * @brief Array mit den erlaubten - weil auf 7-Segment-Anzeigen darstellbaren - Zeichen.
 *
 * Aus Ressourceneinsparungsgründen wird keine String-Klasse verwendet.
 *
 * @todo: Die Anzeige des letzten Zeichens funktioniert nicht. Das Zeichen wird nicht erkannt. Siehe Methode 'get7SegBitMap'
 */
const char *Led7SegmentCharMap::charsAllowed = "0123456789 AbCdEFGHIJLnOPqrSTUcou-°_";

/***************************************************************************************************
 * @brief bitMap enthalten die Bitmuster um die einzelnen Segemente der 7-Segment-Anzeigen für die
 *        Anzeige der Ziffern und Buchstaben anzusteuern. bitMap bildet ein 7-Segment-Display mit
 *        Dezimalpunkt ab.
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
 * bitMap enthält das Bitmuster, die die Ziffer/den Buchstaben darstellen.
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
 *  ```
 *                most sig. Byte   least sig. Byte
 *                7      07      07      07      0
 *    matrix[0] = 0b00000000010010010011111100010000;
 *    matrix[1] = 0b00000000010000000000011000010000;
 *    matrix[2] = 0b00000000111110011101101100000000;
 *    matrix[3] = 0b00000000010100000100111100010000;
 *    matrix[4] = 0b00000000110111001110011000000000;
 *    matrix[5] = 0b00000000011000110110110100000000;
 *    matrix[6] = 0b00000000001110010111110100000000;
 *    matrix[7] = 0b00000000001101100000000000010000;
 * ```
 ************************************************************************************************************/
const uint8_t Led7SegmentCharMap::bitMap[] =  {
        //gfedcba
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
        0b0111000, ///<  "L": Segmente f, e, d             --> bitMap[21]
        0b1001001, ///<  "n": Segmente g, e, c .           --> bitMap[22]
        0b0111111, ///<  "O": Segmente f, e, d, c, b, a    --> bitMap[23]
        0b1110011, ///<  "P": Segmente g, f, e, b, a       --> bitMap[24]
        0b1100111, ///<  "q": Segmente g, f, c, b, a       --> bitMap[25]
        0b1010000, ///<  "r": Segmente g, e                --> bitMap[26]
        0b1101101, ///<  "S": Wie "5"                      --> bitMap[27]
        0b0110001, ///<  "T": Segmente  f, e, a            --> bitMap[28]
        0b0111110, ///<  "U": Segmente f, e, d, c, b       --> bitMap[29]
        0b1011000, ///<  "c": Segmente g, e, d, c          --> bitMap[30]
        0b1011100, ///<  "o": Segmente g, e, d, c          --> bitMap[31]
        0b0011100, ///<  "u": Segmente e, d, c             --> bitMap[32]
        0b1000000, ///<  "-": Segment g                    --> bitMap[33]
        0b1100011, ///<  "°": Segment g, f, b, a           --> bitMap[34]
        0b0001000, ///<  "_": Segment d                    --> bitMap[35]
        0b1001001, ///< Drei waagerechte Striche: Segmente g, d, a    --> bitMap[36]
        0b0110110  ///< Zwei senkrechte Striche:  Segmente f, e, c, b --> bitMap[37]
    };


/**
 * @brief (in cpp-Datei) Bitmap zur Anzeige auf der 7-Segment-Anzeige zurückgeben.
 *
 * @todo statt Schleife die Standardfunktion strchr() verwenden.
 * @todo Fehler: statt "_" wird die Fehlerbitmap zurückgegeben.
 *
 */
uint8_t Led7SegmentCharMap::get7SegBitMap(const char outChar) const {
    for (size_t i = 0; i < strlen(charsAllowed); i++) {
        if (outChar == charsAllowed[i]) {
            return bitMap[i];
        }
    }
    // wenn man hier landet, wurde kein erlaubtes Zeichen gefunden --> Fehlerbitmap zurück geben
    // Serial.println(F("get7SegBitMap: Nix gefunden. Fehlerausgang."));
    return bitMap[CHAR_ERROR];
}
