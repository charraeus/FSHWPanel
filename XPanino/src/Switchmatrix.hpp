/*************************************************************************************************************
 * @file switchmatrix.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der @em SwitchMatrixClass
 *
 * Die Zeilen der Schaltermatrix müssen mit @em PORTC verbunden sein. Die Spalten der Schaltermatrix müssen
 * mit @em PORTD verbunden sein. Die Zeilen werden zyklisch aktiviert und die Spaltenwerte aus @em PORTD und
 * @em PORTC für jede Zeile ausgelesen und in switchMatrixState gespeichert.
 *
 *
 * +----------+
 * | Port D   |
 * |  L       |
 * |  o     7 +-------------------------------------------------------------------+
 * |  w       |                                                                   |
 * |  e     6 +------------------------------------------------------------+      |
 * |  r       |                                                            |      |
 * |        5 +-----------------------------------------------------+      |      |
 * |  C       |                                                     |      |      |
 * |  o     4 +----------------------------------------------+      |      |      |
 * |  l     3 +-----nv                                       |      |      |      |
 * |  u     2 +-----nv                                       |      |      |      |
 * |  m     1 +-----nv                                       |      |      |      |
 * |  n     0 +-----nv                                       |      |      |      |
 * |  s       |                                              |      |      |      |
 * +----------+                                              |      |      |      |
 * | Port B   |                                              |      |      |      |
 * |  U       |                                              |      |      |      |
 * |  p     7 +-----nv                                       |      |      |      |
 * |  p     6 +-----nv                                       |      |      |      |
 * |  e     5 +-----nv                                       |      |      |      |
 * |  r     4 +-----nv                                       |      |      |      |
 * |        3 +---------------------------------------+      |      |      |      |
 * |  C       |                                       |      |      |      |      |
 * |  o     2 +--------------------------------+      |      |      |      |      |
 * |  l       |                                |      |      |      |      |      |
 * |  u     1 +-------------------------+      |      |      |      |      |      |
 * |  m       |                         |      |      |      |      |      |      |
 * |  n     0 +------------------+      |      |      |      |      |      |      |
 * |  s       |                  |      |      |      |      |      |      |      |
 * +----------+                  |      |      |      |      |      |      |      |
 * | Port C   |                  |      |      |      |      |      |      |      |
 * |        2 +------------------*------*------*------*------*------*------*------*-
 * | Rows     |                  |      |      |      |      |      |      |      |
 * |        1 +------------------*------*------|------*------*------*------*------*-
 * |          |                  |      |      |      |      |      |      |      |
 * |        0 +------------------*------*------*------*------*------*------*------*-
 * |          |                  |      |      |      |      |      |      |      |
 * +----------+                  |      |      |      |      |      |      |      |
 *                               |      |      |      |      |      |      |      |
 *                               V      V      V      V      V      V      V      V
 *                            +------------------------------------------------------+
 *                            |  7      6      5      4      3      2      1      0  |
 *                            |              resultierendes Column-Byte              |
 *                            +------------------------------------------------------+
 *
 *     *: an diesen Stellen sitzen die Schalter/Taster
 *
 *
 * @version 0.1
 * @date 2023-03-31
 *
 * Copyright © 2017 - 2023. All rights reserved.
 *
 *
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>
#include <button_debounce.hpp>

/*************************************************************************************************************
 * Konstanten
 */

///< Max. 8 Zeilen für die SwitchMatrix-Arrays verwenden. Muss auf VALID Rows abgestimmt sein!
const uint8_t MAX_ROWS = 3;

// Bitmasken zum Ausblenden der nicht benötigten Bits
const uint8_t VALID_ROWS = 0b00000111;  ///< Nur die unteren 3 Pins
const uint8_t VALID_LOWER_COLS = 0xf0;  ///< Nur die oberen 4 Pins für das untere Halbbyte der Spalten
const uint8_t VALID_UPPER_COLS = 0x0f;  ///< Nur die unteren 4 Pins für das obere Halbbyte der Spalten
const uint8_t PULLUP_ON = 0xff;         ///< Alle Pullup-Widerstände am Inputport aktivieren

// Zeiger auf die DDR-Register
// "... ist ein konstanter Zeiger auf ein uint8_t, das volatile ist."
volatile uint8_t * const ROWS_DDR = &DDRC;          ///< DDR für den Matrixzeilen-Port
volatile uint8_t * const LOWER_COLS_DDR = &DDRD;    ///< DDR für das untere Halbbyte der Matrixspalten
volatile uint8_t * const UPPER_COLS_DDR = &DDRB;    ///< DDR für das obere Halbbyte der Matrixspalten

// Zeiger auf die PORT-Register
volatile uint8_t * const ROWS_PORT = &PORTC;        ///< Portfür die Matrixzeilen
volatile uint8_t * const LOWER_COLS_PORT = &PORTD;  ///< Port für die unteren 4 Matrixspalten
volatile uint8_t * const UPPER_COLS_PORT = &PORTB;  ///< Port für die oberen 4 Matrxispalten

// Zeiger auf die PIN-Register
volatile uint8_t * const LOWER_COLS_PIN = &PIND;
volatile uint8_t * const UPPER_COLS_PIN = &PINB;

/*************************************************************************************************************
 * @brief Schaltermatrix
 *
 * PORTx: Port x des Atmega/Arduino
 *  DDRx: Data Direction Register for PORTx
 *
 * @todo Schaltermatrixklasse hier noch dokumentieren.
 *
 ************************************************************************************************************/
class SwitchMatrixClass {
public:
    // SwitchMatrixClass();

    void initHardware();
    void scanSwitchPins();
    Debouncer matrixRows[MAX_ROWS] = { Debouncer(PULLUP_ON), Debouncer(PULLUP_ON), Debouncer(PULLUP_ON) };
};
