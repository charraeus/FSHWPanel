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

#ifndef LEDMATRIX_HPP
#define LEDMATRIX_HPP

#include <Arduino.h>

/* Konstanten für den (gewünschten) Status des Dezimalpunkts der 7-Segment-Anzeigen */
const bool DP_ON = true;    ///< Dezimalpunkt wird angezeigt.
const bool DP_OFF = false;  ///< Dezimalpunkt wird nicht angezeigt.

/* Konstanten für die Anzeige von Zeichen auf den 7-Segment-Anzeigen */
const uint8_t CHAR_0 = 0;   ///< Zeichen "0"
const uint8_t CHAR_1 = 1;   ///< Zeichen "1"
const uint8_t CHAR_2 = 2;   ///< Zeichen "2"
const uint8_t CHAR_3 = 3;   ///< Zeichen "3"
const uint8_t CHAR_4 = 4;   ///< Zeichen "4"
const uint8_t CHAR_5 = 5;   ///< Zeichen "5"
const uint8_t CHAR_6 = 6;   ///< Zeichen "6"
const uint8_t CHAR_7 = 7;   ///< Zeichen "7"
const uint8_t CHAR_8 = 8;   ///< Zeichen "8"
const uint8_t CHAR_9 = 9;   ///< Zeichen "9"
const uint8_t CHAR_BANK = 10;   ///< Zeichen "0"
const uint8_t CHAR_3_DASH_HORIZ = 11;   ///< Zeichen Drei "-" übereinander
const uint8_t CHAR_2_DASH_VERT = 12;   ///< Zeichen "||"
const uint8_t CHAR_A = 13;   ///< Zeichen "A"
const uint8_t CHAR_b = 14;   ///< Zeichen "b"
const uint8_t CHAR_C = 15;   ///< Zeichen "C"
const uint8_t CHAR_c = 28;   ///< Zeichen "c"
const uint8_t CHAR_d = 16;   ///< Zeichen "d"
const uint8_t CHAR_E = 17;   ///< Zeichen "D"
const uint8_t CHAR_F = 18;   ///< Zeichen "F"
const uint8_t CHAR_H = 19;   ///< Zeichen "H"
const uint8_t CHAR_L = 20;   ///< Zeichen "L"
const uint8_t CHAR_o = 21;   ///< Zeichen "o"
const uint8_t CHAR_P = 22;   ///< Zeichen "P"
const uint8_t CHAR_r = 23;   ///< Zeichen "r"
const uint8_t CHAR_U = 24;   ///< Zeichen "U"
const uint8_t CHAR_u = 25;   ///< Zeichen "u"
const uint8_t CHAR_MINUS = 26;   ///< Zeichen "-"
const uint8_t CHAR_DEGREE = 27;   ///< Zeichen "°" (Grad)
const uint8_t CHAR_ERROR = CHAR_3_DASH_HORIZ;   ///< Zeichen für Fehler. 

/* Konstanten für Blinkrate und Blinken */
const uint8_t BLINK_NORMAL = 0;     ///< Normale Blinkgeschwindigkeit.
const uint8_t BLINK_SLOW = 1;       ///< Langsame Blinkgeschwindigkeit.
const uint8_t NO_OF_SPEED_CLASSES = 2;  ///< Anzahl Blinkgeschwindigkeiten.

/* Konstanten für die Größe der LED-Matrix */
constexpr const uint8_t LED_ROWS = 8;
constexpr const uint32_t LED_COLS = sizeof(uint32_t) * 8;  //Achtung: durch Verwendung von uint32_t ist die Spaltenzahl immer 32

/**************************************************************************************************
 * LEDs, die in einer Matrix angeordnet sind.
 * 
 * @todo Ausführlichere Doku ergänzen.
 **************************************************************************************************/
class LedMatrix {
private:
    uint32_t matrix[LED_ROWS];                              ///< Matrix für den Status (ein oder aus) je LED.
    uint32_t hwMatrix[LED_ROWS];                            ///< akt. Status ein/aus je LED. Diese Matrix steuert direkt die Hardware.
    uint32_t blinkStatus[NO_OF_SPEED_CLASSES][LED_ROWS];    ///< Status ob geblinkt werden soll je Geschwindigkeitsklasse und LED.
    unsigned long int blinkStartTime[NO_OF_SPEED_CLASSES];  ///< Gibt den Takt des normal-schnellen Blinkens für alle LEDs vor.
    bool isBlinkDarkPhase[NO_OF_SPEED_CLASSES];             ///< Flag für die Dunkelphase beim normalen Blinken.
    unsigned long int nextBlinkInterval[NO_OF_SPEED_CLASSES];  ///< Dauer des nächsten Blink-Intervalls (abhängig von BLINK_INTERVAL_NORMAL_BRIGHT und ..._DARK.
//    uint32_t blinkStatusNormal[LED_ROWS];                 ///< Status ob geblinkt werden soll je LED; normale Blinkgeschwindigkeit.
//    uint32_t blinkStatusSlow[LED_ROWS];                   ///< Status ob geblinkt werden soll je LED; langsame Blinkgeschwindigkeit.
//    unsigned long int blinkStartTimeNormal;               ///< Gibt den Takt des normal-schnellen Blinkens für alle LEDs vor.
//    unsigned long int blinkStartTimeSlow;                 ///< Gibt den Takt des langsamen Blinkens für alle LEDs vor.
//    bool isBlinkDarkPhaseNormal;                          ///< Flag für die Dunkelphase beim normalen Blinken.
//    bool isBlinkDarkPhaseSlow;                            ///< Flag für die Dunkelphase beim langsamen Blinken.
//    unsigned long int nextBlinkIntervalNormal;            ///< Dauer des nächsten Blink-Intervalls (abhängig von BLINK_INTERVAL_NORMAL_BRIGHT und ..._DARK.
//    unsigned long int nextBlinkIntervalSlow;              ///< Dauer des nächsten Blink-Intervalls (abhängig von BLINK_INTERVAL_NORMAL_BRIGHT und ..._DARK. 
    
    /** 
     * Char in ein Bitmuster zur Anzeige auf einem 7-Segment-Display umrechnen, d.h.\ das zum
     * @em char passende Bitmuster zur Anzeige ermitteln.
     * @param [in] character Das anzuzeigende Zeichen. Muss eines der @em CHAR_x Konstanten sein.

     */
    uint8_t get7SegBits(const unsigned char character);
    
    /**
     * @brief Prüfen, ob @em row und @em col gültig sind, d.h.\ innerhalb der Arraygrenzen liegen.\ Gültig 
     * heißt, @em row und @em col ist jeweils in [0..LED_ROWS -1 bzw.\ 0..LED_COLS - 1]
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix. 
     * @param [in] col Die Nummer der Spalte in der LedMatrix.
     * @return true Sowohl @em row als auch @em col sind gültig, d.h. innerhalb der Arraygrenzen.
     * @return false @em row oder @em col liegen außerhalb der Arraygrenzen.
     */
    bool isValidRowCol(const uint8_t row, const uint8_t col);
    
    /**
     * @brief Prüfen, ob blinkSpeed gültig ist, d.h.\ eine der @em blinkSpeed Konstanten ist.
     * 
     * @param [in] blinkSpeed Eine der @em blinkSpeed Konstanten.
     * @return true @em blinkSpeed ist gültig.
     * @return false @em blinkSpeed ist nicht gültig.
     */
    bool isValidBlinkSpeed(const uint8_t blinkSpeed);
    
    /**
     * @brief Ein-/Aus-Status für die LEDs gemäß der aktuellen Hell-/Dunkelphase des Blinkens festlegen.
     * 
     */
    void doBlink();
    
public:    
    /**
     * @brief Konstruktor: Hardware des Arduino initialisieren.
     * Erst die Hardware und I/O-Pins des Arduino initialisieren und dann die eingebaute LED 
     * als Status-Feedback ein paar mal blinken lassen und die Arduino-Pins initialisieren.
     */
    LedMatrix();
    void initHardware();
    
    /**
     * @brief Die die LEDs repräsentierenden Bits serialisieren und an die MIC5891/5821-Chips übertragen.
     * 
     */
    void display();

    /**
     * @brief Prüfen, ob LED an der Position (@em row, @em col) in der Led Matrix angeschaltet ist.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix. 
     * @param [in] col Die Nummer der Spalte in der LedMatrix. 
     * @return @em true Die LED ist angeschaltet.
     * @return @em false Die LED ist ausgeschaltet. 
     */
    bool isLedOn(const uint8_t row, const uint8_t col);

    /**
     * @brief LED anschalten, d.h.\ das entsprechende Bit in der LedMatrix an der Position (@em row, @em col) setzen.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix.
     * @param [in] col Die Nummer der Spalte in der LedMatrix. 
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledOn(const uint8_t row, const uint8_t col);
    
    /**
     * @brief LED ausschalten, d.h.\ das entsprechende Bit in der LedMatrix an der Position (@em row, @em col) löschen.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix. 
     * @param [in] col Die Nummer der Spalte in der LedMatrix.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledOff(const uint8_t row, const uint8_t col);

    /**
     * @brief LED zwischen den Zuständen Aus und Ein umschalten,
     * d.h.\ das entsprechende Bit in der LedMatrix an der Position (@em row, @em col) umschalten.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix.  
     * @param [in] col Die Nummer der Spalte in der LedMatrix. 
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledToggle(const uint8_t row, const uint8_t col);

    /**
     * @brief Das Blinken einer LED einschalten.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix.   
     * @param [in] col Die Nummer der Spalte in der LedMatrix.  
     * @param [in] blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledBlinkOn(const uint8_t row, const uint8_t col, const uint8_t blinkSpeed = BLINK_NORMAL);

    /**
     * @brief Das Blinken einer LED ausschalten.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix.    
     * @param [in] col Die Nummer der Spalte in der LedMatrix.   
     * @param [in] blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n 
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int ledBlinkOff(const uint8_t row, const uint8_t col, const uint8_t blinkSpeed = BLINK_NORMAL);

    /**
     * @brief Prüfen, ob für eine LED das Blinken eingeschaltet ist.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix.     
     * @param [in] col Die Nummer der Spalte in der LedMatrix.    
     * @param [in] blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n 
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL.
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int isLedBlinkOn(const uint8_t row, const uint8_t col, const uint8_t blinkSpeed = BLINK_NORMAL);

    /**
     * @brief Auf dem durch @em row und @em col spezifizierten 7-Segment-Display das Zeichen @em NewValue anzeigen, 
     * d.h.\ die entsprechenden Bits setzen bzw. löschen. Der Dezimalpunkt wird mit dem Parameter @em dpOn gesondert gesetzt.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix.  
     * @param [in] col0 Die Nummer der Spalte in der LedMatrix, d.h. der 1. LED (= Segment) der 7-Segment-Anzeige.
     * @param [in] newValue Das auszugebende Zeichen. 
     * @param [in] dpOn @em true wenn der Dezimalpunkt des 7-Segment-Displays eingeschaltet sein soll, sonst @em false.  
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegValue(const uint8_t row, const uint8_t col0, const unsigned char newValue, const bool dpOn = false);

    /**
     * @brief Für das Zeichen, das auf dem 7-Segment-Display angezeigt wird, blinken aktivieren.\ 
     * Optional inkl.\ Dezimalpunkt.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix. Diese entspricht der Nummer der 7-Segment-Anzeige.
     * @param [in] col0 Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige.
     * @param [in] dpBlink @em true, wenn der Dezimalpunkt ebenfalls blinken soll, sonst @em false.
     * @param [in] blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n 
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL. 
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegBlinkOn(const uint8_t row, const uint8_t col0, const bool dpBlink = false,
                       const uint8_t blinkSpeed = BLINK_NORMAL);
    
    /**
     * @brief Das Blinken des Zeichens das auf der 7-Segment-Anzeige angezeigt wird, deaktivieren.\ 
     * Optional ebenfalls den Dezimalpunkt.
     * 
     * @param [in] row Die Nummer der Zeile in der LedMatrix. Diese entspricht der Nummer der 7-Segment-Anzeige
     * @param [in] col0 Die Nummer der Spalte der ersten LED der 7-Segment-Anzeige.
     * @param [in] dpBlink @em false, wenn das Blinken des Dezimalpunkt ebenfalls abgeschaltet werden soll, 
     * sonst @em true.
     * @param [in] blinkSpeed Die Geschwindigkeit, mit der die LED blinken soll. Optionaler Parameter.\n 
     *                          Mögliche Angaben: BLINK_SLOW, BLINK_NORMAL. Es wird nur das Blinken der 
     *                          angegebenen Geschwindigkeit ausgeschaltet.  
     * @return int Status der Aktion: 0 oder -1 falls ungültige Row/Col.
     */
    int set7SegBlinkOff(const uint8_t row, const uint8_t col0, const bool dpBlink = false,
                        const uint8_t blinkSpeed = BLINK_NORMAL);

    /**
     * @brief Ein wenig mit den LEDs spielen... --> simulierter Power-On-Self-Test
     * 
     */
    void powerOnSelfTest();

    #ifdef DEBUG
    void printMatrix();
    #endif /* ifdef DEBUG */
};


#endif /* #ifndef */
