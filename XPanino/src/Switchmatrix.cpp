/*********************************************************************************************************//**
 * @file      Switchmatrix.cpp
 * @brief     Implementierung der Klasse @em SwitchMatrix.
 * @details   Keine Details vorhanden
 *
 * @author    Christian Harraeus
 * @date      2017-10-31
 *
 * Copyright © 2017 - 2020 Christian Harraeus. All rights reserved.
 *
 ************************************************************************************************************/

#include <Arduino.h>
#include <Switchmatrix.hpp>
#include <parser.hpp>


/*********************************************************************************************************//**
 * Methoden für SwitchMatrix
 *
 ************************************************************************************************************/

void SwitchMatrix::initHardware() {
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
    uint8_t pinStatus = 0;
    size_t matrixRow = 0;
    size_t matrixCol = 0;

    for (uint8_t row = HW_MATRIX_ROWS_LSB_PIN; row <= HW_MATRIX_ROWS_MSB_PIN; ++row) {
        digitalWrite(row, LOW);     // Die Matrixzeile aktivieren
        for (uint8_t col = HW_MATRIX_COLS_LSB_PIN; col <= HW_MATRIX_COLS_MSB_PIN; ++col) {
            matrixRow = row - HW_MATRIX_ROWS_LSB_PIN;   // Pin-Nummer auf Matrixzeile umrechnen.
            matrixCol = col - HW_MATRIX_COLS_LSB_PIN;   // Pin-Nummer auf Matrixspalte umrechnen.
            if (isValidMatrixPos(matrixRow, matrixCol)) {
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
            } else {
                Serial.print("Error: Invalid switchMatrix position in "
                    "SwitchMatrix::scanSwitchPins(): row=");
                Serial.print(row); Serial.print(", col="); Serial.println(col); // NOLINT
            }
        }   /// weiter geht's mit der nächsten Spalte
        digitalWrite(row, HIGH);    /// Row-Pin wieder auf HIGH setzen und damit deaktivieren.
    }   /// weiter geht's mit der nächsten Row
}

/** SwitchMatrix::transmitStatus
 * @brief Schalterstatus (ON, OFF, LON) übermitteln
 *
 * @param changedOnly @em true: nur den Status von seit letztem Mal geänderten
 *                              Schaltern übermitteln
 *                    @em false: den Status aller Schalter übermitteln
 */
void SwitchMatrix::transmitStatus(const bool changedOnly, EventListClass eventList) {
    for (uint8_t row = 0; row < SWITCH_MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < SWITCH_MATRIX_COLS; col++) {
            if (changedOnly && switchMatrix[row][col].isChanged()) {
                switchMatrix[row][col].transmitStatus(row, col);        // Methode eines einzelnen Switches
            } else {
                if (! changedOnly) {
                    switchMatrix[row][col].transmitStatus(row, col) + String(row);
                }
            }
        }
    }
}


#ifdef DEBUG
/**
 *
 *
 */
void SwitchMatrix::printMatrix() {
    Serial.println();
    Serial.print("\t");
    for (uint8_t col = 0; col < SWITCH_MATRIX_COLS; col++) {
        Serial.print("Col "); Serial.print(col); Serial.print("\t");
    }
    Serial.println();
    for (uint8_t col = 0; col <= SWITCH_MATRIX_COLS; col++) {
        Serial.print("--------");
    }
    Serial.println();
    for (uint8_t row = 0; row < SWITCH_MATRIX_ROWS; row++) {
        Serial.print("Row "); Serial.print(row); Serial.print(":\t");
        for (uint8_t col = 0; col < SWITCH_MATRIX_COLS; col++) {
            Serial.print(row); Serial.print(" / "); Serial.print(col);
            Serial.print("\t");
        }
        Serial.println();
    }
}
#endif


/*********************************************************************************************************//**
 * ab hier die privaten Methoden
*************************************************************************************************************/

/**
 * @brief Prüfen, ob row eine gültige Zeile in der SwitchMatrix ist.
 *
 * @param row Zeilennummer für die SwitchMatrix.
 *
 * @return true Die Zeilennummer ist gültig.
 * @return false Die Zeilennummer ist ungültig.
 */
bool SwitchMatrix::isValidMatrixRow(const uint8_t row) { return ((row >= 0) && (row <= SWITCH_MATRIX_ROWS)); }


/**
 * @brief Prüfen, ob col eine gültige Spalte in der SwitchMatrix ist.
 *
 * @param col Spaltennummer für die SwitchMatrix.
 *
 * @return true Die Spaltennummer ist gültig.
 * @return false Die Spaltennummer ist ungültig.
 */
bool SwitchMatrix::isValidMatrixCol(const uint8_t col) { return ((col >= 0) && (col <= SWITCH_MATRIX_COLS)); }


/**
 * @brief Prüfen, ob row und col eine gültige Position in der SwitchMatrix sind.
 *
 * @param row Zeilennummer für die SwitchMatrix.
 * @param col Spaltennummer für die SwitchMatrix.
 *
 * @return true Die Position ist gültig.
 * @return false Die Position ist ungültig.
 */
bool SwitchMatrix::isValidMatrixPos(const uint8_t row, const uint8_t col) {
    return isValidMatrixRow(row) && isValidMatrixCol(col);
}
