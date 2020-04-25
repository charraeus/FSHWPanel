/******************************************************************************
 * @file      Switchmatrix.cpp
 * @brief     Implementierung der Klasse @em SwitchMatrix.
 * @details   Keine Details vorhanden
 *
 * @author    Christian Harraeus
 * @date      2017-10-31
 * 
 * Copyright © 2017 - 2020 Christian Harraeus. All rights reserved.
 * 
 * 
 ******************************************************************************/

#include "Switchmatrix.hpp"
#include "Arduino.h"

/******************************************************************************/
SwitchMatrix::SwitchMatrix() {
    printMatrix();
    /// Alle Matrixzeilen-Pins als Output einstellen und auf HIGH setzen.
    for (uint8_t row = HW_MATRIX_ROWS_LSB_PIN; row <= HW_MATRIX_ROWS_MSB_PIN; ++row) {
        pinMode(row, OUTPUT); // die Pins 2 und 3 auf Output setzen.
        digitalWrite(row, HIGH);
    }
    /// Alle Matrixspalten-Pins als Input mit aktiviertem Pullup-Widerstand einstellen.
    for (uint8_t col = HW_MATRIX_COLS_LSB_PIN; col <= HW_MATRIX_COLS_MSB_PIN; ++col) {
        pinMode(col, INPUT_PULLUP);
    }
}


/******************************************************************************/
void SwitchMatrix::scanSwitchPins() {
    uint8_t pinStatus;
    size_t matrixRow, matrixCol;
    
    for (uint8_t row = HW_MATRIX_ROWS_LSB_PIN; row <= HW_MATRIX_ROWS_MSB_PIN; ++row) {
        digitalWrite(row, LOW);     // Die Matrixzeile aktivieren
        for (uint8_t col = HW_MATRIX_COLS_LSB_PIN; col <= HW_MATRIX_COLS_MSB_PIN; ++col) {
            matrixRow = row - HW_MATRIX_ROWS_LSB_PIN;   // Pin-Nummer auf Matrixzeile umrechnen.
            matrixCol = col - HW_MATRIX_COLS_LSB_PIN;   // Pin-Nummer auf Matrixspalte umrechnen.
            
            /// Pinstatus einlesen und entprellen.
            pinStatus = digitalRead(col);
            
            /// Wenn eine Änderung erkannt wurde, den neuen Schalterstatus in der _SwitchMatrix speichern
            /// und die Einschalt\"zeit\" merken.
            /// Außerdem die neuen Status an PC übertragen.\n
            /// @em 0 enspricht geschlossenem Schalter, da bei geschlossenem Schalter
            /// das Col-Pin auf @em LOW gezogen wird.
            if (pinStatus != (switchMatrix[matrixRow][matrixCol]).getStatusNoChange()) {
                if (pinStatus == LOW) {
                    switchMatrix[matrixRow][matrixCol].setOn();
                }
                else {
                    switchMatrix[matrixRow][matrixCol].setOff();
                };
                changed = true;
            } else {
                /// Bei den nicht veränderten Schaltern die Einschaltzeiten aktualisieren.
                switchMatrix[matrixRow][matrixCol].updateOnTime(millis()); 
                /// Lange Tastendrücke identifizieren und ggf. ein Ereignis auslösen.
                switchMatrix[matrixRow][matrixCol].checkLongOn();
            }
        }
        digitalWrite(row, HIGH);    /// Zeilen-Pin wieder auf HIGH setzen und damit deaktivieren.
    }
}


/******************************************************************************/
void SwitchMatrix::transmitStatus(const bool changedOnly) {
    for (auto &row : switchMatrix) {
        for (auto &col : row) {
            if (changedOnly && col.isChanged()) {
                col.transmitStatus();
            } else {
                if (! changedOnly) {
                    col.transmitStatus();
                }
            }
        }
    }
}


/******************************************************************************/
void SwitchMatrix::findMatrixRowColByName(const String &switchName, size_t &matrixRow, size_t &matrixCol) {
    // Eingabe: Schaltername
    // Ausgabe: Row und Col der Matrix (nicht die Pin-Nummern!)
    // Wenn Row und Col den Wert 9999 haben, wurde der Name nicht gefunden
    matrixRow = 9999;
    matrixCol = 9999;
    for (unsigned int row = 0; row != SWITCH_MATRIX_ROWS; ++row) {
        for (unsigned int col = 0; col != SWITCH_MATRIX_COLS; ++col) {
            if ((switchMatrix[row][col]).getName() == switchName) {
                matrixRow = row;
                matrixCol = col;
                break;
            }
        }
    }
}

#ifdef DEBUG
/******************************************************************************/
void SwitchMatrix::printMatrix() {
    for (auto &row : switchMatrix) {
        for (auto &col : row) {
            Serial.print(col.getName());
            Serial.print("\t");
        }
        Serial.println();
    }
}
#endif //if def DEBUG
