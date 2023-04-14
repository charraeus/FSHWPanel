/*************************************************************************************************************
 * @file switchmatrix.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der @em SwitchMatrixClass
 * Die Zeilen der Schaltermatrix müssen mit @em PORTC verbunden sein. Die Spalten der Schaltermatrix müssen
 * mit @em PORTD verbunden sein. Die Zeilen werden zyklisch aktiviert und die Spaltenwerte aus @em PORTD für
 * jede Zeile ausgelesen und in switchMatrixState gespeichert.
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

/*************************************************************************************************************
 * Konstanten
 */

///< Max. 8 Zeilen für die SwitchMatrix-Arrays verwenden. Muss auf VALID Rows abgestimmt sein!
const uint8_t MAX_ROWS = 3;

// Bitmasken zum Ausblenden der nicht benötigten Bits
const uint8_t VALID_ROWS = 0b00000111;  ///< Nur die unteren 3 Pins
const uint8_t VALID_LOWER_COLS = 0xf0;  ///< Nur die oberen 4 Pins für das untere Halbbyte der Spalten
const uint8_t VALID_UPPER_COLS = 0x0f;  ///< Nur die unteren 4 Pins für das obere Halbbyte der Spalten

// Zeiger auf die DDR-Register
// "... ist ein konstanter Zeiger auf ein uint8_t, das volatile ist."
volatile uint8_t * const ROWS_DDR = &DDRC;          ///< DDR für den Matrixzeilen-Port
volatile uint8_t * const LOWER_COLS_DDR = &DDRD;    ///< DDR für das untere Halbbyte der Matrixzeilen
volatile uint8_t * const UPPER_COLS_DDR = &DDRB;    ///< DDR für das obere Halbbyte der Matrixzeilen

// Zeiger auf die PORT-Register
volatile uint8_t * const ROWS_PORT = &PORTC;        ///< Portfür die Matrixzeilen
volatile uint8_t * const LOWER_COLS_PORT = &PORTD;
volatile uint8_t * const UPPER_COLS_PORT = &PORTB;

// Zeiger auf die PIN-Register
volatile uint8_t * const LOWER_COLS_PIN = &PIND;
volatile uint8_t * const UPPER_COLS_PIN = &PINB;

const bool TRANSMIT_ALL_SWITCHES = true;    ///< alle Schalter-Status übertragen - auch unveränderte


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
    SwitchMatrixClass();

    void initHardware();
    void scanSwitchPins();

private:
    uint8_t switchMatrixState[MAX_ROWS];        ///< Aktueller Zustand der Schalter
    uint8_t switchMatrixLastState[MAX_ROWS];    ///< Letzter Zustand der Schalter
    uint8_t switchMatrixChanged[MAX_ROWS];      ///< Geänderte Schalterzustände
};
