/**************************************************************************************************
 * @file ledmatrix.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em LedMatrix und viele Konstanten zur Anzeige.
 * @version 0.1
 * @date 2017-11-10
 *
 * Copyright © 2017 - 2020. All rights reserved.
 *
 * @todo Müssen die Konstanten global sein?
**************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <charmap7seg.hpp>


/**************************************************************************************************/
/* Konstanten für die Größe der LED-Matrix */
constexpr const uint8_t LED_ROWS = 8;
constexpr const uint32_t LED_COLS = sizeof(uint32_t) * 8;  //Achtung: durch Verwendung von uint32_t ist die Spaltenzahl immer 32

/* Konstanten für Display-Felder */
const uint8_t MAX_DISPLAY_FIELDS = 4;       ///< Maximal mögliche Anzahl Display-Felder.
const uint8_t MAX_7SEGMENT_UNITS = 6;       ///< Maximal mögliche Anzahl 7-Segment-Anzeigen je Display-Feld.

/**************************************************************************************************
 * @brief Speichert die Dauer der Hell- und Dunkelphasen für's Blinken
 *
 */
class SpeedClass {
public:
    SpeedClass(unsigned long int brightTime, unsigned long int darkTime);
    unsigned long int getBrightTime() const {return brightTime;};
    unsigned long int getDarkTime() const {return darkTime;};

private:
    unsigned long int brightTime;   ///< Zeit in Millisekunden eingeschaltet
    unsigned long int darkTime;     ///< Dauer in Millisekunden ausgeschaltet
};

/**************************************************************************************************/
/* Konstanten für Blinkrate und Blinken */
const uint8_t BLINK_NORMAL = 0;  ///< Dient als Index für blinkTimes; Normale Blinkgeschwindigkeit.
const uint8_t BLINK_SLOW = 1;    ///< Dient als Index für blinkTimes; Langsame Blinkgeschwindigkeit.
const uint8_t NO_OF_SPEED_CLASSES = 2;  ///< Anzahl Blinkgeschwindigkeitspaare.
const SpeedClass blinkTimes[NO_OF_SPEED_CLASSES] = {    // NOLINT
        { 500,  500}, //< BLINK_NORMAL:  500 ms hell,  500 ms dunkel
        {2000, 6000}  //< BLINK_SLOW:   2000 ms hell, 6000 ms dunkel
      } ;

/**************************************************************************************************
 * @brief Ein Display-Feld fasst mehrere 7-Segment-Anzeigen zusammen.
 *
 */
class DisplayField {
public:
    uint8_t led7SegmentRows[MAX_7SEGMENT_UNITS];    ///< Rows in der LED-Matrix für die einzelnen 7-Segment-Anzeigen.
    uint8_t led7SegmentCol0s[MAX_7SEGMENT_UNITS];   ///< Cols des Segment a in der LED-Matrix für die einzelnen 7-Segment-Anzeigen.
    uint8_t count7SegmentUnits;                     ///< Anzahl 7-Segment-Anzeigen, aus denen das Display-Feld besteht.
};

/**************************************************************************************************
 * @brief Position der Led in der LedMatrix, bestehend aus Row (y-Wert) und Col (x-Wert)
 *
 */
class LedMatrixPos {
public:
    uint8_t row;    ///< Row der Led.
    uint8_t col;    ///< Col der Led.
};


/**************************************************************************************************
 * LEDs, die in einer Matrix angeordnet sind.
 *
 * @todo Ausführlichere Doku ergänzen.
 **************************************************************************************************/
class LedMatrix {
private:
    uint32_t matrix[LED_ROWS];          ///< Matrix für den logischen Status (ein oder aus) je LED.
    uint32_t hwMatrix[LED_ROWS];        ///< Akt. Status ein/aus je LED. Diese Matrix steuert direkt die Hardware.
    DisplayField displays[MAX_DISPLAY_FIELDS];  ///< Display-Felder (= Zusammenfassung von 7-Segment-Anzeigen).
    Led7SegmentCharMap charMap;                  ///< Zeichentabelle für 7-Segment-Anzeige(n)
    uint32_t blinkStatus[NO_OF_SPEED_CLASSES][LED_ROWS];    ///< Status ob geblinkt werden soll je Geschwindigkeitsklasse und LED.
    unsigned long int blinkStartTime[NO_OF_SPEED_CLASSES];  ///< Gibt den Takt des normal-schnellen Blinkens für alle LEDs vor.
    bool isBlinkDarkPhase[NO_OF_SPEED_CLASSES];  ///< Flag für die Dunkelphase beim normalen Blinken.
    unsigned long int nextBlinkInterval[NO_OF_SPEED_CLASSES];  ///< Dauer des nächsten Blink-Intervalls (abhängig von blinkTimes[].brightTime und ...darkTime.
    const uint8_t ASCII_0 = 48;  // ASCII-Code für '0'
    const uint8_t ASCII_9 = 57;  // ASCII-Code für '9'

    /***************************************************************************
     * @brief Prüfen, ob @em row und @em col gültig sind, d.h.\ innerhalb der Arraygrenzen liegen.\ Gültig
     * heißt, @em row und @em col ist jeweils in [0..LED_ROWS -1 bzw.\ 0..LED_COLS - 1]
     *
     * @param [in] row Die Nummer der Zeile in der LedMatrix.
     * @param [in] col Die Nummer der Spalte in der LedMatrix.
     * @return true Sowohl @em row als auch @em col sind gültig, d.h. innerhalb der Arraygrenzen.
     * @return false @em row oder @em col liegen außerhalb der Arraygrenzen.
     */
    bool isValidRowCol(uint8_t row, uint8_t col);
    bool isValidRowCol(LedMatrixPos pos);

    /***************************************************************************
     * @brief Prüfen, ob blinkSpeed gültig ist, d.h.\ eine der @em blinkSpeed Konstanten ist.
     *
     * @param [in] const blinkSpeed Eine der @em blinkSpeed Konstanten.
     * @return true @em blinkSpeed ist gültig.
     * @return false @em blinkSpeed ist nicht gültig.
     */
    bool isValidBlinkSpeed(uint8_t blinkSpeed);

    /***************************************************************************
     * @brief Prüfen, ob geblinkt werden muss. Falls nämlich nicht, spart man sich
     *        einen Haufen Aufwand.
     *        Diese Funktion wird von LedMatrix::doBlink() verwendet.
     */
    bool isSomethingToBlink();

    /***************************************************************************
     * @brief Ein-/Aus-Status für die LEDs gemäß der aktuellen Hell-/Dunkelphase des Blinkens festlegen.
     *
     */
    void doBlink();

public:
    /***************************************************************************
     * @brief Konstruktor: Hardware des Arduino initialisieren.
     * Erst die Hardware und I/O-Pins des Arduino initialisieren und dann die eingebaute LED
     * als Status-Feedback ein paar mal blinken lassen und die Arduino-Pins initialisieren.
     */
    LedMatrix();

    /***************************************************************************
     * @brief Die die LEDs repräsentierenden Bits serialisieren und
     *        an die MIC5891/5821-Chips übertragen.
     *
     */
    void initHardware();

     /***************************************************************************
    * @brief Die die LEDs repräsentierenden Bits serialisieren und an die MIC5891/5821-Chips übertragen.
     *
     */
    void writeToHardware();

    /***************************************************************************
     * @brief Prüfen, ob LED an der Position (@em row, @em col) in der Led Matrix angeschaltet ist.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col Die Nummer der Spalte in der LedMatrix.
     * @return @em true Die LED ist angeschaltet.
     * @return @em false Die LED ist ausgeschaltet.
     */
    bool isLedOn(uint8_t row, uint8_t col);
    bool isLedOn(LedMatrixPos pos);

    /***************************************************************************
     * @brief LED anschalten, d.h.\ das entsprechende Bit in der LedMatrix an der Position (@em row, @em col) setzen.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col Die Nummer der Spalte in der LedMatrix.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledOn(uint8_t row, uint8_t col);
    int ledOn(LedMatrixPos pos);

    /***************************************************************************
     * @brief LED ausschalten, d.h.\ das entsprechende Bit in der LedMatrix an der Position (@em row, @em col) löschen.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col Die Nummer der Spalte in der LedMatrix.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledOff(uint8_t row, uint8_t col);
    int ledOff(LedMatrixPos pos);

    /***************************************************************************
     * @brief LED zwischen den Zuständen Aus und Ein umschalten,
     * d.h.\ das entsprechende Bit in der LedMatrix an der Position (@em row, @em col) umschalten.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col Die Nummer der Spalte in der LedMatrix.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledToggle(uint8_t row, uint8_t col);
    int ledToggle(LedMatrixPos pos);

    /***************************************************************************
     * @brief Das Blinken einer LED einschalten.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col Die Nummer der Spalte in der LedMatrix.
     * @param [in] const blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledBlinkOn(uint8_t row, uint8_t col, uint8_t blinkSpeed = BLINK_NORMAL);
    int ledBlinkOn(LedMatrixPos pos, uint8_t blinkSpeed = BLINK_NORMAL);

    /***************************************************************************
     * @brief Das Blinken einer LED ausschalten.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col Die Nummer der Spalte in der LedMatrix.
     * @param [in] const blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledBlinkOff(uint8_t row, uint8_t col, uint8_t blinkSpeed = BLINK_NORMAL);
    int ledBlinkOff(LedMatrixPos pos, uint8_t blinkSpeed = BLINK_NORMAL);

    /***************************************************************************
     * @brief Prüfen, ob für eine LED das Blinken eingeschaltet ist.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col Die Nummer der Spalte in der LedMatrix.
     * @param [in] const blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int isLedBlinkOn(uint8_t row, uint8_t col, uint8_t blinkSpeed = BLINK_NORMAL);
    int isLedBlinkOn(LedMatrixPos pos, uint8_t blinkSpeed = BLINK_NORMAL);

    /***************************************************************************
     * @brief Auf dem durch @em row und @em col spezifizierten 7-Segment-Display das Zeichen @em NewValue anzeigen,
     * d.h.\ die entsprechenden Bits setzen bzw. löschen. Der Dezimalpunkt wird mit dem Parameter @em dpOn gesondert gesetzt.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix.
     * @param [in] const col0 Die Nummer der Spalte in der LedMatrix, d.h. der 1. LED (= Segment) der 7-Segment-Anzeige.
     * @param [in] const newValue Das auszugebende Zeichen.
     * @param [in] const dpOn @em true wenn der Dezimalpunkt des 7-Segment-Displays eingeschaltet sein soll, sonst @em false.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegValue(uint8_t row, uint8_t col0,
                     uint8_t charBitMap, bool dpOn = false);  // NOLINT
    int set7SegValue(LedMatrixPos pos, uint8_t charBitMap,
                     bool dpOn = false); // NOLINT

    /***************************************************************************
     * @brief Für das Zeichen, das auf dem 7-Segment-Display angezeigt wird, blinken aktivieren.\
     * Optional inkl.\ Dezimalpunkt.
     *
     * @param [in] row Die Nummer der Zeile in der LedMatrix. Diese entspricht der Nummer der 7-Segment-Anzeige.
     * @param [in] const col0 Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige.
     * @param [in] const dpBlink @em true, wenn der Dezimalpunkt ebenfalls blinken soll, sonst @em false.
     * @param [in] const blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegBlinkOn(uint8_t row, uint8_t col0, bool dpBlink = false,
                       uint8_t blinkSpeed = BLINK_NORMAL);
    int set7SegBlinkOn(LedMatrixPos pos, bool dpBlink = false,
                       uint8_t blinkSpeed = BLINK_NORMAL);

    /***************************************************************************
     * @brief Das Blinken des Zeichens, das auf der 7-Segment-Anzeige angezeigt wird, deaktivieren.\
     * Optional ebenfalls den Dezimalpunkt.
     *
     * @param [in] const row Die Nummer der Zeile in der LedMatrix. Diese entspricht der Nummer der 7-Segment-Anzeige
     * @param [in] const col0 Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige.
     * @param [in] const dpBlink @em false, wenn das Blinken des Dezimalpunkt ebenfalls abgeschaltet werden soll,
     * sonst @em true.
     * @param [in] blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     * Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL. Es wird nur das Blinken der
     * angegebenen Geschwindigkeit ausgeschaltet.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegBlinkOff(uint8_t row, uint8_t col0, bool dpBlink = false,
                        uint8_t blinkSpeed = BLINK_NORMAL);
    int set7SegBlinkOff(LedMatrixPos pos, bool dpBlink = false, uint8_t blinkSpeed = BLINK_NORMAL);

    /***************************************************************************
     * @brief Mehrere 7-Segment-Anzeigen zu einem Display zusammenfassen, auf dem
     * dann ein Wert angezeigt werden kann.
     *
     * @param [in] fieldId
     * @param [in] led7SegmentId
     * @param [in] matrixRow
     * @param [in] matrixCol
     */
    void defineDisplayField(const uint8_t &fieldId, const uint8_t &led7SegmentId,
                            const uint8_t &matrixRow, const uint8_t &matrixCol0);

    void defineDisplayField(const uint8_t &fieldId, const uint8_t &led7SegmentId,
                            const LedMatrixPos &pos);

    /***************************************************************************
     *
     */
    void defineDisplayFieldLength(uint8_t &fieldId, uint8_t &count7Segments) {
        displays[fieldId].count7SegmentUnits = count7Segments;
    };

    /***************************************************************************
    * @brief Einen Wert (String) auf einem Display, d.h. ggf. über mehrere 7-Segment-Anzeigen
     * hinweg, ausgeben
     *
     * @param [in] fieldId
     * @param [out] outString
     */
    void display(const uint8_t &fieldId, const String &outString);
};
