/*************************************************************************************************************
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
#include <buffer.hpp>
#include <Switchmatrix.hpp>

/*************************************************************************************************************
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
                // Ungültige Position, d.h. row oder col außerhalb der Matrix.
                Serial.print(F("Error 001"));
            }
        }   /// weiter geht's mit der nächsten Spalte
        digitalWrite(row, HIGH);    /// Row-Pin wieder auf HIGH setzen und damit deaktivieren.
    }   /// weiter geht's mit der nächsten Row
}


void SwitchMatrix::transmitStatus(const bool changedOnly) {
    for (uint8_t row = 0; row < SWITCH_MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < SWITCH_MATRIX_COLS; col++) {
            if (changedOnly && switchMatrix[row][col].isChanged()) {
                transmit(row, col, switchMatrix[row][col].getStatus());  // Status eines einzelnen Switches
            } else {
                if (! changedOnly) {
                    transmit(row, col, switchMatrix[row][col].getStatus());
                }
            }
        }
    }
}



void SwitchMatrix::transmit(uint8_t &row, uint8_t &col, uint8_t switchState) {
    // switchState = 0: Switch is off
    // switchState = 1: Switch is on
    // switchState = 2: Switch is long on

    // c-string with data to be sent (e.g. via the serial port)
    char charsToSend[MAX_BUFFER_LENGTH] = "S;S;";

    // temp memory for typecast int --> c-string
    char charRowCol[MAX_PARA_LENGTH * 2] = "";

    // set charsToSend according to value of data
    if (switchState == 2) {
        strcat(charsToSend, "LON;");
    } else {
        strcat(charsToSend, ((switchState == 1) ? "ON;" : "OFF;"));
    }
    snprintf (charRowCol, MAX_PARA_LENGTH * 2, "%u;%u", row, col);
    strcat(charsToSend, charRowCol);
    Serial.println(charsToSend);
}


#ifdef DEBUG
/// @brief Schaltermatrix auf Terminal ausgeben.
void SwitchMatrix::printMatrix() {
    Serial.println();
    Serial.print(F("\t"));
    for (uint8_t col = 0; col < SWITCH_MATRIX_COLS; col++) {
        Serial.print(F("Col ")); Serial.print(col); Serial.print(F("\t"));
    }
    Serial.println();
    for (uint8_t col = 0; col <= SWITCH_MATRIX_COLS; col++) {
        Serial.print(F("--------"));
    }
    Serial.println();
    for (uint8_t row = 0; row < SWITCH_MATRIX_ROWS; row++) {
        Serial.print(F("Row ")); Serial.print(row); Serial.print(F(":\t"));
        for (uint8_t col = 0; col < SWITCH_MATRIX_COLS; col++) {
            Serial.print(row); Serial.print(F(" / ")); Serial.print(col);
            Serial.print(F("\t"));
        }
        Serial.println();
    }
}
#endif


/*************************************************************************************************************
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
