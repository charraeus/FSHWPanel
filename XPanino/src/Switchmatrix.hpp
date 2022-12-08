/*********************************************************************************************************//**
 * @file Switchmatrix.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em SwitchMatrix
 * @version 0.1
 * @date 2017-10-29
 *
 * Copyright © 2017 - 2020. All rights reserved.
 *
 * @todo statt <em>if not defined(NDEBUG)</em> besser auf <em>if defined(DEBUG)</em> umstellen.
 ************************************************************************************************************/

#pragma once

#include <switch.hpp>
#include <parser.hpp>

// Konstanten
const bool TRANSMIT_ONLY_CHANGED_SWITCHES = true; ///< nur veränderte Schalter-Status übertragen
const bool TRANSMIT_ALL_SWITCHES = false;         ///< alle Schalter-Status übertragen, unabhängig ob verändert oder nicht

/*********************************************************************************************************//**
 * Konstanten für die Schaltermatrix.
 ************************************************************************************************************/
const unsigned int HW_MATRIX_ROWS_LSB_PIN = 14;  ///< Pin-Nummer des niederstwertigen Pins der Matrixzeilen Y
const unsigned int HW_MATRIX_ROWS_MSB_PIN = 17;  ///< Pin-Nummer des höchstwertigen Pins der Matrixzeilen Y
const unsigned int HW_MATRIX_COLS_LSB_PIN = 6;   ///< Pin-Nummer des niederstwertigen Pins der Matrixspalten X
const unsigned int HW_MATRIX_COLS_MSB_PIN = 13;  ///< Pin-Nummer des höchstwertigen Pins der Matrixspalten X
constexpr uint8_t SWITCH_MATRIX_ROWS = HW_MATRIX_ROWS_MSB_PIN - HW_MATRIX_ROWS_LSB_PIN + 1;  ///< Anzahl Matrixzeilen
constexpr uint8_t SWITCH_MATRIX_COLS = HW_MATRIX_COLS_MSB_PIN - HW_MATRIX_COLS_LSB_PIN + 1;  ///< Anzahl Matrixspalten


/*********************************************************************************************************//**
 * @brief Schaltermatrix zur Aufnahme von Schaltern der Klasse @em switch.
 *
 * @todo Schaltermatrixklasse hier noch dokumentieren.
 ************************************************************************************************************/
class SwitchMatrix {
public:
     /**
     * @brief Die Hardware, d.h. die Pins, an denen die Schalter angeschlossen sind, initialisieren.
     *
     */
    void initHardware();


    /**
     * Den Status aller Hardware-Schalter in die SwitchMatrix einlesen.
     *
     * Die Matrixzeilen (Y) werden nacheinander auf @em LOW gesetzt und dann die
     * daraus resultierenden Werte der Matrixspalten (X) eingelesen.
     * Die Schalterstatus werden in der switchMatrix gespeichert.
     * @see Externe Doku: Arduino-Doku
     * @todo Statt \@see den richtigen Doxygen-Verweis auf die Arduino-Doku verwenden.
     */
    void scanSwitchPins();


    /**
     * Den Status aller Schalter in der Matrix ablegen.
     *
     * Überträgt den Status der einzelnen Schalter in die Schaltermatrix.
     * Hierzu wird die Methode @em Switch::transmitStatus() des jeweiligen Schalters verwendet.
     *
     * @param [in]  changedOnly
     *              @em true ==>  es wird nur der Status der Schalter, die sich seit der letzten Abfrage geändert haben, übertragen.\n
     *              @em false ==> der Status aller Schalter wird übertragen.
     */
    void transmitStatus(bool changedOnly, EventListClass eventList);


    #ifdef DEBUG
    /**
     * Alle Schalter in der Matrix ausgeben.
     *
     * Gibt Row und Col der Schalter zeilen- und spaltenweise aus.
     * @note Dient eigentlich nur zum debuggen.
     */
    void printMatrix();
    #endif /* ifdef DEBUG */

private:
    Switch switchMatrix[SWITCH_MATRIX_ROWS][SWITCH_MATRIX_COLS];    ///< Switchmatrix anlegen.
    bool changed = false;   ///< Änderungsstatus der gesamten Matrix. Sobald sich ein Schalter ändert, ist @em changed @em true.
    const unsigned int debounceTime = 9;  ///< Zeit in Millisekunden zum Entprellen

    inline bool isValidMatrixRow(const uint8_t row);
    inline bool isValidMatrixCol(const uint8_t col);
    inline bool isValidMatrixPos(const uint8_t row, const uint8_t col);
};
