/*************************************************************************************************************
 * @file Switchmatrix.cpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Implementierung der @em SwitchMatrixClass
 * @version 0.1
 * @date 2023-03-31
 *
 * Copyright © 2017 - 2023. All rights reserved.
 *
 * @todo Schalter entprellen
 *
 ************************************************************************************************************/

#include <Arduino.h>
#include <switchmatrix.hpp>
#include <tools.hpp>

#if defined DEBUG && defined NOT_WITH_SERIAL
#include "avr8-stub.h"
//#include "app_api.h" // only needed with flash breakpoints
#endif


/*********************************************************************************************************//**
 * @brief Ports der Schaltermatrix initialisieren
 *
 ************************************************************************************************************/
SwitchMatrixClass::SwitchMatrixClass() {
    // In den Zustandsarrays alle Spalten mit 1 initialisieren, da
    // eingeschaltete Buttons/Schalter die Portpins auf LOW ziehen.
    // Ausnahme: Changed ist immer auf 1, falls sich ein Schalterzustand geändert hat
    for (uint8_t row = 0; row < MAX_ROWS; ++row) {
        switchMatrixState[row] = 0xff;
        switchMatrixLastState[row] = 0xff;
        switchMatrixChanged[row] = 0;
    }
}


void SwitchMatrixClass::initHardware() {
    // Alle gültigen Matrixzeilen-Pins als Output einstellen --> relevante Bits setzen.
    // und alle Bits des Ports auf HIGH setzen (= alle Zeilen deaktivieren).
    // Gem. Datasheet Kap. 14.2.3 muss ein Zwischenstatus beim Wechsel von Tri-State zu Output-HIGH
    // auftreten. Der Zwischenstatus ist OUTPUT-LOW (oder INPUT mit Pull-Up; hier aber nicht verwendet).
    *ROWS_DDR |= VALID_ROWS;      // Pins auf Output schalten (HIGH).
    *ROWS_PORT &= ~VALID_ROWS;    // Pins auf LOW schalten (= Zwischenstatus).
    _NOP();
    *ROWS_PORT |= VALID_ROWS;     // Pins auf HIGH schalten --> so will ich das haben.
    // Alle Matrixspalten-Pins als Input einstellen --> relevante Bits löschen.
    // und deren Pullup-Widerstand einschalten --> relevante Bits setzen
    *LOWER_COLS_DDR &= ~VALID_LOWER_COLS;
    *LOWER_COLS_PORT |= VALID_LOWER_COLS;
    *UPPER_COLS_DDR &= ~VALID_UPPER_COLS;
    *UPPER_COLS_PORT |= VALID_UPPER_COLS;
}


/*********************************************************************************************************//**
 * @brief Die Schaltermatrix einlesen und den Status speichern
 *
 * Die Matrixzeilen (Y) werden nacheinander auf @em LOW gesetzt und dann die
 * daraus resultierenden Werte der Matrixspalten (X) eingelesen.
 * Die Schalterstatus werden in der switchMatrix gespeichert.
 * @see Externe Doku: Arduino-Doku
 * @todo Statt \@see den richtigen Doxygen-Verweis auf die Arduino-Doku verwenden.
 *
 *  rowBit: Bitmaske zum Aktivieren der einzelnen Matrixzeilen
 * rowPort: Der Port, an dem die Matrixzeilen angeschlossen sind. Siehe initHardware().
 *     row: Logische Zeile der Schaltermatrix, mit der das Array adressiert wird.
 *
 *     switchMatrixState: Array, das den aktuell, eingelesenen Status der Schalter enthält.
 *                        Schalter ein ==> LOW, Schalter aus ==> HIGH
 *   switchMatrixChanged: Array, das überall dort ein Bit gesetzt hat, dessen Schalterstatus sich
 *                        geändert hat.
 * switchMatrixLastState: Array, das den letzten gültigen Status der Schalter enthält.
 *
 * @todo die 4er-Konstanten noch berechnen: Anzahl nicht gesetzter Bits in den valid..Cols
 *
 ************************************************************************************************************/
void SwitchMatrixClass::scanSwitchPins() {
    // #if defined DEBUG && !defined NOT_WITH_SERIAL
    // Serial.print("*UPPER_COLS_PIN & 0b00001111="); printBinValue(*UPPER_COLS_PIN & VALID_UPPER_COLS);
    // Serial.print("   ");
    // Serial.print("*LOWER_COLS_PIN & 0b11110000="); printBinValue(*LOWER_COLS_PIN & VALID_LOWER_COLS);
    // Serial.println();
    // #endif

//    uint8_t lastMatrixState = 0;
    for (uint8_t row = 0; row < MAX_ROWS; ++row) {
        switchMatrixLastState[row] = switchMatrixState[row];   // Akt. Status merken
        *ROWS_PORT &= ~_BV(row);               // "row-te" Matrixzeile aktivieren (auf LOW setzen)
        _NOP();     // warten, bis das Signal an PORT D und B stabil ist
        // Den Port auslesen, der den unteren Teil der Matrixspalten enthält
        // und in das untere Halbbyte des MatrixState übertragen
        switchMatrixState[row] = (*LOWER_COLS_PIN & VALID_LOWER_COLS) >> 4;
        // Den Port auslesen, der den oberen Teil der Matrixspalten enthält
        // und in das obern Halbbyte des MatrixState übertragen
        switchMatrixState[row] |= (*UPPER_COLS_PIN & VALID_UPPER_COLS) << 4;
        //switchMatrixState[row] |= *upperColPort & validUpperCols;
        // Die Änderungen im MatrixChanged-Array markieren (bei Änderung zum vorigen Status: Bit setzen)
        switchMatrixChanged[row] = switchMatrixLastState[row] ^ switchMatrixState[row];
        #if defined DEBUG &&  !defined NOT_WITH_SERIAL
        if (switchMatrixChanged[row] != 0) {
            Serial.print("Row="); printBinValue(*ROWS_PORT); Serial.print("  -->  ");
            Serial.print("Cols="); printBinValue(switchMatrixState[row]); Serial.println();
        }
        #endif
        *ROWS_PORT |= _BV(row);                // Matrixzeile wieder auf HIGH setzen
    }
}
