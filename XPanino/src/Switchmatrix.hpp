/*************************************************************************************************************
 * @file switchmatrix.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der @em SwitchMatrix
 * @version 0.1
 * @date 2023-03-31
 *
 * Copyright © 2017 - 2023. All rights reserved.
 *
 * @todo statt <em>if not defined(NDEBUG)</em> besser auf <em>if defined(DEBUG)</em> umstellen.
 *
 ************************************************************************************************************/

#pragma once

/*************************************************************************************************************
 * Konstanten
 */
const bool TRANSMIT_ONLY_CHANGED_SWITCHES = true; ///< nur veränderte Schalter-Status übertragen
const bool TRANSMIT_ALL_SWITCHES = false;         ///< alle Schalter-Status übertragen, unabhängig ob verändert oder nicht

/*************************************************************************************************************
 * Schaltermatrix-Daten
 ************************************************************************************************************/
const uint8_t MAX_ROWS = 8;
uint8_t switchMatrixState[MAX_ROWS];        ///< Aktueller Zustand des Schalters
uint8_t switchMatrixLastState[MAX_ROWS];    ///< Letzter Zustand des Schalters
uint8_t switchMatrixChanged[MAX_ROWS];      ///< Geänderte Schalterzustände


void initSwitchMatrix();
void scanSwitchPins();
