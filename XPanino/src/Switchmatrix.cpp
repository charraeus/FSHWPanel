/*************************************************************************************************************
 * @file Switchmatrix.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Implementierung der @em SwitchMatrix
 * @version 0.1
 * @date 2023-03-31
 *
 * Copyright © 2017 - 2023. All rights reserved.
 *
 * @todo statt <em>if not defined(NDEBUG)</em> besser auf <em>if defined(DEBUG)</em> umstellen.
 *
 ************************************************************************************************************/

#include <Arduino.h>
#include "switchmatrix.hpp"


/*********************************************************************************************************//**
 * @brief Ports der Schaltermatrix initialisieren
 *
 * Rows: PORTC
 * Cols: PORTD
 *
 * DDRx: Data Direction Register for PORTx
 *
 ************************************************************************************************************/
void initSwitchMatrix() {
    DDRC = 0xff;    // Alle Matrixzeilen-Pins als Output einstellen
    PORTB = 0xff;   // und auf HIGH setzen.
    DDRD = 0;       // Alle Matrixspalten-Pins als Input einstellen
    PORTD = 0xff;   // und deren Pullup-Widerstand einschalten.
    // Jetzt alle Matrixwerte mit 0 initialisieren
    for (uint8_t row = 0; row < 8; ++row) {
        switchMatrixState[row] = 0;
        switchMatrixLastState[row] = 0;
        switchMatrixChanged[row] = 0;
    }
}


/*********************************************************************************************************//**
 * @brief
 *
 ************************************************************************************************************/
void scanSwitchPins() {
    uint8_t row = 0;

    for (row = 1; row <= 8; ++row) {
        PORTB &= ~row;              // Matrixzeile row aktivieren (auf LOW setzen)
        switchMatrixState[row - 1] = PORTD;  // Alle Matrixspalten einlesen
        switchMatrixChanged[row - 1] = switchMatrixLastState[row - 1] ^ switchMatrixState[row - 1];
        PORTB |= row;               // Matrixzeile wieder auf HIGH setzen
    }
}
