/**************************************************************************************************
 * @file Switchmatrix.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em SwitchMatrix
 * @version 0.1
 * @date 2017-10-29
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
 * @todo statt <em>if not defined(NDEBUG)</em> besser auf <em>if defined(DEBUG)</em> umstellen.
 **************************************************************************************************/

#ifndef Switchmatrix_hpp
#define Switchmatrix_hpp

#include <Arduino.h>
#include "switch.hpp"

// Konstanten
const bool TRANSMIT_ONLY_CHANGED_SWITCHES = true; ///< nur veränderte Schalter-Status übertragen
const bool TRANSMIT_ALL_SWITCHES = false;         ///< alle Schalter-Status übertragen, unabhängig ob verändert oder nicht

/******************************************************************************
 * Bildet die Schaltermatrix für ein Avionic-gerät ab.
 ******************************************************************************/
const unsigned int HW_MATRIX_ROWS_LSB_PIN = 14;    ///< Pin-Nummer des niederstwertigen Pins der Matrixzeilen Y
const unsigned int HW_MATRIX_ROWS_MSB_PIN = 17;    ///< Pin-Nummer des höchstwertigen Pins der Matrixzeilen Y
const unsigned int HW_MATRIX_COLS_LSB_PIN = 6;     ///< Pin-Nummer des niederstwertigen Pins der Matrixspalten X
const unsigned int HW_MATRIX_COLS_MSB_PIN = 13;    ///< Pin-Nummer des höchstwertigen Pins der Matrixspalten X 
constexpr size_t SWITCH_MATRIX_ROWS = HW_MATRIX_ROWS_MSB_PIN - HW_MATRIX_ROWS_LSB_PIN + 1;  ///< Anzahl Matrixzeilen
constexpr size_t SWITCH_MATRIX_COLS = HW_MATRIX_COLS_MSB_PIN - HW_MATRIX_COLS_LSB_PIN + 1;  ///< Anzahl Matrixspalten

/**************************************************************************************************
 * @brief Schaltermatrix zur Aufnahme von Schaltern der Klasse @em switch.
 * 
 * @todo Schaltermatrixklasse hier noch dokumentieren.
 **************************************************************************************************/
class SwitchMatrix {
private:
    Switch switchMatrix[SWITCH_MATRIX_ROWS][SWITCH_MATRIX_COLS]
        {   
            { Switch{"0"},   Switch{"1"},   Switch{"2"},   Switch{"3"},   Switch{"4"},   Switch{"5"},   Switch{"6"},  Switch{"7"}},
            { Switch{"IDT"}, Switch{"VFR"}, Switch{"CLR"}, Switch{"OFF"}, Switch{"SBY"}, Switch{"TST"}, Switch{"ON"}, Switch{"ALT"}},
            { Switch{"2-0"}, Switch{"2-1"}, Switch{"2-2"}, Switch{"2-3"}, Switch{"2-4"}, Switch{"2-5"}, Switch{"OAT"}, Switch{"2-7"}},
            { Switch{"3-0"}, Switch{"3-1"}, Switch{"3-2"}, Switch{"3-3"}, Switch{"3-4"}, Switch{"3-5"}, Switch{"SEL"}, Switch{"CTL"}}
        };  ///< Schaltermatrix anlegen und mit neu erzeugten Schaltern der Klasse @em Switch füllen. Dabei werden auch die Schalternamen vergeben.
    bool changed = false;   ///< Änderungsstatus der gesamten Matrix. Sobald sich ein Schalter ändert, ist @em changed @em true.
    unsigned int debounceTime = 2;  ///< Zeit in Millisekunden zum Entprellen
    
public:
    /**
     * @brief Konstruktor. Die Hardware des Arduino initialisieren.
     * 
     * Initialisiert die Ports/Pins, an denen die Schalter angeschlossen sind.
     */
    SwitchMatrix();

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
    void transmitStatus(const bool changedOnly);
    
    /**
     * Schalter per Name finden und die zugehörige Row und Col der Matrix zurückgeben.
     *
     * Gibt die Zeile und Spalte in der Matrix zurück, die zu dem Schalter mit dem entsprechenden Namen gehören. 
     * Das sind nicht die Pin-Nummern des Arduino, sondern die Zeile und Spalte im Array @em switchMatrix.
     *
     * @param [in]   switchName Name des Schalters, dessen Zeile und Spalte benötigt werden.
     * @param [out]  matrixRow  Nummer der Zeile des Schalters [0..n]
     * @param [out]  matrixCol  Nummer der Spalte des Schalters [0..n]
     * @note        Wenn Row oder Col den Wert 9999 haben, wurde der Name nicht gefunden.
     */
    void findMatrixRowColByName(const String &switchName, size_t &matrixRow, size_t &matrixCol);

    #ifdef DEBUG    
    /**
     * Die Namen aller Schalter ausgeben.
     *
     * Gibt alle Namen der Schalter zeilen- und spaltenweise aus. 
     * @note Dient eigentlich nur zum debuggen.
     */
    void printMatrix();
    #endif /* ifdef DEBUG */
};

#endif /* ifndef Switchmatrix_hpp */