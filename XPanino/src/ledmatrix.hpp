/*********************************************************************************************************//**
 * @file ledmatrix.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em LedMatrix und viele Konstanten zur Anzeige.
 * @version 0.1
 * @date 2017-11-10
 *
 * Copyright © 2017 - 2020. All rights reserved.
 *
 * @todo Müssen die Konstanten global sein?
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <charmap7seg.hpp>

/*********************************************************************************************************//**
 * Konstanten für Größe von LED-Matrix und  DisplayFields
 ************************************************************************************************************/
// Konstanten für die Größe der LED-Matrix
constexpr const uint8_t LED_ROWS = 8;   ///< Anzahl Zeilen in der LED-Matrix
constexpr const uint32_t LED_COLS = sizeof(uint32_t) * 8;  ///< Anzahl Spalten in der LED-Matrix
/// Achtung: durch Verwendung von uint32_t ist die Spaltenzahl immer 32

// Konstanten für die Anzahl und Größe der Display-Felder
const uint8_t MAX_DISPLAY_FIELDS = 4;       ///< Maximal mögliche Anzahl Display-Felder
const uint8_t MAX_7SEGMENT_UNITS = 6;       ///< Maximal mögliche Anzahl 7-Segment-Anzeigen je Display-Feld


/*********************************************************************************************************//**
 * @brief SpeedClass Speichert die Dauer der Hell- und Dunkelphasen für's Blinken
 ************************************************************************************************************/
class SpeedClass {
public:
    /**
     * @brief Construct a new Speed Class object
     *
     * @param brightTime Dauer der Hellphase beim Blinken in Millisekunden
     * @param darkTime Dauer der Dunkelphase beim Blinken in Millisekunden
     */
    SpeedClass(unsigned long int brightTime, unsigned long int darkTime);
    inline unsigned long int getBrightTime() const { return brightTime; };
    inline unsigned long int getDarkTime() const { return darkTime; };

private:
    unsigned long int brightTime;   ///< Zeit in Millisekunden eingeschaltet
    unsigned long int darkTime;     ///< Dauer in Millisekunden ausgeschaltet
};


/*********************************************************************************************************//**
 * Konstanten für Blinkrate und Blinken
 ************************************************************************************************************/
const uint8_t BLINK_NORMAL = 0;  ///< Dient als Index für blinkTimes; Normale Blinkgeschwindigkeit.
const uint8_t BLINK_SLOW = 1;    ///< Dient als Index für blinkTimes; Langsame Blinkgeschwindigkeit.
const uint8_t NO_OF_SPEED_CLASSES = 2;  ///< Anzahl Blinkgeschwindigkeitspaare.
// NOLINTNEXTLINE
const SpeedClass blinkTimes[NO_OF_SPEED_CLASSES] = {    ///< Blinkgeschwindigkeitspaare
        { 500,  500}, ///< BLINK_NORMAL:  500 ms hell,  500 ms dunkel
        {2000, 6000}  ///< BLINK_SLOW:   2000 ms hell, 6000 ms dunkel
      };



/*********************************************************************************************************//**
 * @brief Ein DisplayField fasst mehrere 7-Segment-Anzeigen zusammen.
 ************************************************************************************************************/
class DisplayField {
public:
    uint8_t led7SegmentRows[MAX_7SEGMENT_UNITS];    ///< Rows in der LED-Matrix für die einzelnen 7-Segment-Anzeigen.
    uint8_t led7SegmentCol0s[MAX_7SEGMENT_UNITS];   ///< Cols des Segment a in der LED-Matrix für die einzelnen 7-Segment-Anzeigen.
    uint8_t count7SegmentUnits;                     ///< Anzahl 7-Segment-Anzeigen, aus denen das Display-Feld besteht.
};


/*********************************************************************************************************//**
 * @brief Position der LED in der LedMatrix, bestehend aus Row (y-Wert) und Col (x-Wert)
 *
 ************************************************************************************************************/
class LedMatrixPos {
public:
    uint8_t row;    ///< Row der LED
    uint8_t col;    ///< Col der LED
};


/*********************************************************************************************************//**
 * @brief LEDs, die in einer Matrix angeordnet sind.
 *
 * @todo Ausführlichere Doku ergänzen.
 ************************************************************************************************************/
class LedMatrix {
public:
    /** LedMatrix - Konstruktor
     * @brief Die Matrizen etc. initialisieren
     */
    LedMatrix();


    /**
     * @brief Hardware des Arduino initialisieren.
     */
    void initHardware();


    /**
     * @brief Die die LEDs repräsentierenden Bits serialisieren und an die MIC5891/5821-Chips übertragen.
     * @note Diese Funktion muss regelmäßig und sehr häufig innerhalb des loop
     *       aufgerufen werden!
     */
    void writeToHardware();


    /**
     * @brief Prüfen, ob LED an der Position (@em row, @em col) in der LedMatrix angeschaltet ist.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile der LED in der LedMatrix.\n
     *            pos.col: Die Nummer der Spalte der LED in der LedMatrix.
     *
     * @return Status der LED. @em false Die LED ist ausgeschaltet.
     */
    bool isLedOn(LedMatrixPos pos);


    /**
     * @brief LED anschalten, d.h.\ das entsprechende Bit in der LedMatrix an der
     *        Position (@em row, @em col) setzen.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile der LED in der LedMatrix.\n
     *            pos.col: Die Nummer der Spalte der LED in der LedMatrix.
     *
     * @return Erfolg der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledOn(LedMatrixPos pos);


    /**
     * @brief LED ausschalten, d.h.\ das entsprechende Bit in der LedMatrix an der
     *        Position (@em row, @em col) löschen.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile der LED in der LedMatrix.\n
     *            pos.col: Die Nummer der Spalte der LED in der LedMatrix.
     *
     * @return Erfolg der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledOff(LedMatrixPos pos);


    /**
     * @brief LED zwischen den Zuständen Aus und Ein umschalten,
     *        d.h.\ das entsprechende Bit in der LedMatrix an der Position (@em row, @em col) umschalten.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile der LED in der LedMatrix.\n
     *            pos.col: Die Nummer der Spalte der LED in der LedMatrix.
     * @return Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledToggle(LedMatrixPos pos);


    /**
     * @brief Das Blinken einer LED einschalten.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile der LED in der LedMatrix.\n
     *            pos.col: Die Nummer der Spalte der LED in der LedMatrix.
     *
     * @param blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                   Optionaler Parameter. Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return Erfolg der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledBlinkOn(LedMatrixPos pos, uint8_t blinkSpeed = BLINK_NORMAL);


    /**
     * @brief Das Blinken einer LED ausschalten.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile der LED in der LedMatrix.\n
     *            pos.col: Die Nummer der Spalte der LED in der LedMatrix.
     *
     * @param blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                   Optionaler Parameter: Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return Erfolg der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledBlinkOff(LedMatrixPos pos, uint8_t blinkSpeed = BLINK_NORMAL);


    /**
     * @brief Prüfen, ob für eine LED das Blinken eingeschaltet ist.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile der LED in der LedMatrix.\n
     *            pos.col: Die Nummer der Spalte der LED in der LedMatrix.
     * @param blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                   Optionaler Parameter. Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     *
     * @return Erfolg der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int isLedBlinkOn(LedMatrixPos pos, uint8_t blinkSpeed = BLINK_NORMAL);


    /**
     * @brief Auf dem durch @em pos spezifizierten 7-Segment-Display das Zeichen, das durch
     *        @em charBitMap dargestellt wird anzeigen.
     *
     * D.h. die entsprechenden Bits setzen bzw. löschen. Der Dezimalpunkt wird mit
     * dem Parameter @em dpOn gesondert gesetzt.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile in der LedMatrix. Diese entspricht der
     *                     Nummer der 7-Segment-Anzeige.\n
     *            pos.col: Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige. Aka col0.
     * @param charBitMap Das auszugebende Zeichen.
     * @param dpOn @em true wenn der Dezimalpunkt des 7-Segment-Displays eingeschaltet sein soll,
     *             sonst @em false. Optionaler Parameter.
     *
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegValue(LedMatrixPos pos, uint8_t charBitMap, bool dpOn = false);


    /**
     * @brief Für das Zeichen, das auf dem 7-Segment-Display angezeigt wird, blinken aktivieren.\
     *        Optional inkl.\ Dezimalpunkt.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile in der LedMatrix. Diese entspricht der
     *                     Nummer der 7-Segment-Anzeige.\n
     *            pos.col: Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige. Aka col0.
     * @param dpBlink @em true, wenn der Dezimalpunkt ebenfalls blinken soll, sonst @em false.
     * @param blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                   Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     *
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegBlinkOn(LedMatrixPos pos, bool dpBlink = false, uint8_t blinkSpeed = BLINK_NORMAL);


    /**
     * @brief Das Blinken des Zeichens, das auf der 7-Segment-Anzeige angezeigt wird, deaktivieren.\
     *        Optional ebenfalls den Dezimalpunkt.
     *
     * @param pos Position der LED in der LedMatrix.\n
     *            pos.row: Die Nummer der Zeile in der LedMatrix. Diese entspricht der
     *                     Nummer der 7-Segment-Anzeige.\n
     *            pos.col: Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige. Aka col0.
     * @param dpBlink @em false, wenn das Blinken des Dezimalpunkt ebenfalls abgeschaltet werden soll,
     *                 sonst @em true.
     * @param blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                   Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL. Es wird nur das Blinken der
     *                   angegebenen Geschwindigkeit ausgeschaltet.
     *
     * @return Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegBlinkOff(LedMatrixPos pos, bool dpBlink = false, uint8_t blinkSpeed = BLINK_NORMAL);


    /**
     * @brief Mehrere 7-Segment-Anzeigen zu einem Display zusammenfassen, auf dem
     * dann ein Wert angezeigt werden kann.
     *
     * @param fieldId           Id dieses Display-Felds
     * @param led7SegmentId     Id der zugehörigen 7-Segment-Anzeige
     * @param matrixPos         Position der LED in der LedMatrix.\n
     *                          pos.row: Die Nummer der Zeile in der LedMatrix. Diese entspricht der
     *                                   Nummer der 7-Segment-Anzeige.\n
     *                          pos.col: Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige. Aka col0.
     */
    void defineDisplayField(const uint8_t &fieldId, const uint8_t &led7SegmentId,
                            const LedMatrixPos &matrixPos);


    /**
     * @brief Einen Wert (String) auf einem Display, d.h\. ggf\. über mehrere 7-Segment-Anzeigen
     *        hinweg, ausgeben.
     *
     * @param fieldId   Id des Display-Felds, auf dem der outString ausgegeben werden soll
     * @param outString Die auszugebenden Zeichen.
     */
    void display(const uint8_t &fieldId, const String &outString);


private:
    uint32_t matrix[LED_ROWS];    ///< Matrix für den logischen Status (ein oder aus) je LED.
    uint32_t hwMatrix[LED_ROWS];  ///< Akt. Status ein/aus je LED. Diese Matrix steuert direkt die Hardware.
    DisplayField displays[MAX_DISPLAY_FIELDS];  ///< Display-Felder (= Zusammenfassung von 7-Segment-Anzeigen).
    Led7SegmentCharMap charMap;                  ///< Zeichentabelle für 7-Segment-Anzeige(n)
    uint32_t blinkStatus[NO_OF_SPEED_CLASSES][LED_ROWS];    ///< Status ob geblinkt werden soll je Geschwindigkeitsklasse und LED.
    unsigned long int blinkStartTime[NO_OF_SPEED_CLASSES];  ///< Gibt den Takt des normal-schnellen Blinkens für alle LEDs vor.
    bool isBlinkDarkPhase[NO_OF_SPEED_CLASSES];  ///< Flag für die Dunkelphase beim normalen Blinken.
    unsigned long int nextBlinkInterval[NO_OF_SPEED_CLASSES];  ///< Dauer des nächsten Blink-Intervalls (abhängig von blinkTimes[].brightTime und ...darkTime.

    bool isValidRowCol(LedMatrixPos pos);
    bool isValidBlinkSpeed(uint8_t blinkSpeed);
    bool isSomethingToBlink();
    void doBlink();
};
