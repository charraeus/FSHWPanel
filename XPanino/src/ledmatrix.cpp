/*********************************************************************************************************//**
 * @file ledmatrix.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em LedMatrix.
 * @version 0.1
 * @date 2017-11-10
 *
 * Copyright © 2017 - 2022. All rights reserved.
 *
 ************************************************************************************************************/

#include <ledmatrix.hpp>

/** Konstanten für die Zuordnung der Arduino-Pins zu den MIC5891- und MIC5821-Schieberegister-Leitungen */
const uint8_t CLOCK = PIN4;     ///< Arduino-Pin für CLOCK des MIC5891/5821
const uint8_t DATA_IN = PIN5;   ///< Arduino-Pin für DATA_IN des MIC5891/5821
const uint8_t STRB = PIN3;      ///< Arduino-Pin für STRB des MIC5891/5821
const uint8_t OE = PIN2;        ///< Arduino-Pin für OE des MIC5891/5821

/** Konstanten für's Blinken */
const unsigned int BLINK_VERSATZ = 447;     ///< Versatz für die Startzeiten der Blinkgeschwindigkeiten. Damit
                                            ///< nicht alles so gleich im Takt blinkt


/*************************************************************************************************************
 * SpeedClass::SpeedClass Methoden
 ************************************************************************************************************/

/**
 * 
 */
SpeedClass::SpeedClass(const unsigned long int brightTime, const unsigned long int darkTime) {
    this->brightTime = brightTime;
    this->darkTime = darkTime;
};


/*************************************************************************************************************
 * LedMatrix::LedMatrix Methoden
 ************************************************************************************************************/

/**
 *
 */
LedMatrix::LedMatrix() {
    /// Die Matrizen initalisieren
    for (uint32_t row = 0; row != LED_ROWS; ++row) {
        hwMatrix[row] = 0;                  // Alle LEDs ausschalten
        matrix[row] = 0;                    // Alle LEDs sind ausgeschaltet
    }
    /// Defaultmäßig das Blinken deaktivieren
    for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
        for (uint8_t row = 0; row != LED_ROWS; ++row) {
            blinkStatus[speedClass][row] = 0;         // Keine LED blinkt
        }
        isBlinkDarkPhase[speedClass] = true;
        blinkStartTime[speedClass] = millis(); // + speedClass * BLINK_VERSATZ;   // Startzeit mit Versatz
    }
    /// Defaultwerte für die Blinkdauern der Speedklassen der nächsten anstehenden Hellphase
    nextBlinkInterval[BLINK_NORMAL] = blinkTimes[BLINK_NORMAL].getBrightTime();    // Dauer der Hellphase als Initialwert
    nextBlinkInterval[BLINK_SLOW] = blinkTimes[BLINK_SLOW].getBrightTime();
}


/**
 * Erst die Hardware und I/O-Pins des Arduino initialisieren und dann die eingebaute LED
 * als Status-Feedback ein paar mal blinken lassen und die Arduino-Pins initialisieren.
 */
void LedMatrix::initHardware() {
    /// Die eingebaute LED als Status aktivieren: die LED vier mal ein- und ausschalten
    pinMode(LED_BUILTIN, OUTPUT);
    for (unsigned int i = 1; i != 4; ++i) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500); // NOLINT
        digitalWrite(LED_BUILTIN, LOW);
        delay(500); // NOLINT
    }

    /// Die benötigten Pins für die Ansteuerung der Schieberegister initialisieren.
    pinMode(CLOCK, OUTPUT);
    pinMode(DATA_IN, OUTPUT);
    pinMode(STRB, OUTPUT);
    pinMode(OE, OUTPUT);
    delay(1);   // NOLINT: notwendig, da sonst die folgenden Write-Anweisungen nicht funktionieren
    digitalWrite(CLOCK, LOW);
    digitalWrite(DATA_IN, LOW);
    digitalWrite(STRB, LOW);
    delayMicroseconds(500); // NOLINT
    digitalWrite(STRB, HIGH);       // Latches umgehen --> immer auf HIGH setzen
    digitalWrite(OE, LOW);
    delayMicroseconds(500);  // NOLINT
}


/**
 * Falls Blinken eingeschaltet ist, zuerst den gewünschten LED-Status
 * (d.h. an- oder ausgeschaltet) noch gemäß der aktuellen Blinkphase
 * (hell oder dunkel) anpassen; das wird durch Aufruf von doBlink()
 * erledigt.
 *
 * Je Row werden alle 32 Bits (je Column ein Bit) durch die Schieberegister
 * geschossen und anschließend das entsprechende Row-Bit. Beim
 * ersten Schleifendurchgang werden also 32 Bits angezeigt. Beim
 * nächsten Schleifendurchgang die nächsten 32 Bits usw. Durch
 * die hohe Geschwindigkeit wird eine statische Anzeige erzielt.
 * Wenn zu viele andere Aktivitäten zwischen den display()-Aufrufen
 * stattfinden, wird die Anzeige mehr oder weniger stark flimmern.
 */
void LedMatrix::writeToHardware() {
    // Alle Berechnungen zum Blinken erledigen
    doBlink();
    // Die hwMatrix serialisieren, in die Schieberegister schieben und die Outputs scharf schalten
    for (uint8_t row = 0; row != LED_ROWS; ++row) {
        digitalWrite(STRB, LOW);    // STROBE unbedingt auf LOW setzen damit die Registerinhalte in die Latches übernommen werden

        // die 32 Column-Bits der jeweiligen Row durch/in die Schieberegister schieben
        // NOLINTNEXTLINE
        for (uint32_t bit = sizeof(hwMatrix[row]) * 8; bit != 0; --bit) {
            digitalWrite(DATA_IN, (hwMatrix[row] >> (bit - 1)) & 1);  // NOLINT: es funktioniert...
            digitalWrite(CLOCK, HIGH);                  // DATA_IN in Shift-Register übernehmen
            delayMicroseconds(1);
            digitalWrite(CLOCK, LOW);
        }

        // nachdem alle Column-Bits übertragen sind, muss noch das zugehörige Row-Bit übertragen werden.
        // Da das MSB zuerst übertragen werden muss, muss hier statt "row" der Ausdruck "7 - row"
        // verwendet werden.
        uint8_t activeRow = static_cast<uint8_t>(1) << row;
        // NOLINTNEXTLINE
        for (uint8_t bit = sizeof(activeRow) * 8; bit != 0; --bit) {
            digitalWrite(DATA_IN, (activeRow >> (bit - 1)) & 1); // NOLINT
            digitalWrite(CLOCK, HIGH);                  // DATA_IN in Shift-Register übernehmen
            delayMicroseconds(1);
            digitalWrite(CLOCK, LOW);
            delayMicroseconds(1);
        }

        digitalWrite(STRB, HIGH);   // STROBE wieder auf HIGH setzen, damit die Latch-Inhalte auf die Outputs geschaltet werden
        delayMicroseconds(1);     // lt. Datenblatt erforderlich, aber funktioniert auch ohne
    }
}


/**
 *
 *
 */
bool LedMatrix::isLedOn(const LedMatrixPos pos) {
    if (isValidRowCol(pos)) {
        return (matrix[pos.row] & (static_cast<uint32_t>(1) << pos.col)) != 0;
    }
    return false;  // unzulässige Row oder Col; muss zwischen 0 und LED_ROWS - 1 bzw. LED_COLS - 1 sein
}


/**
 *
 *
 */
int LedMatrix::ledOn(const LedMatrixPos pos) {
    if (isValidRowCol(pos)) {
        matrix[pos.row] |= static_cast<uint32_t>(1) << pos.col;    // An der Stelle col soll das Bit gesetzt werden
        return 0;
    }
    return -1;      // unzulässige Row oder Col
}


/**
 *
 *
 */
int LedMatrix::ledOff(const LedMatrixPos pos) {
    if (isValidRowCol(pos)) {
        matrix[pos.row] &= ~ (static_cast<uint32_t>(1) << pos.col);
        return 0;
    }
    return -1;      // unzulässige Row oder Col
}


/**
 *
 *
 */
int LedMatrix::ledToggle(const LedMatrixPos pos) {
    if ((pos.row >= LED_ROWS) || (pos.col >= LED_COLS)) {
        return -1;      // unzulässige Row oder Col
    }
    if (isLedOn(pos)) {
        ledOff(pos);
    } else {
        ledOn(pos);
    }
    return 0;
}


/**
 *
 *
 */
int LedMatrix::ledBlinkOn(const LedMatrixPos pos, const uint8_t blinkSpeed) {
    if (isValidRowCol(pos) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                // An der Stelle col soll das Bit gesetzt werden
                blinkStatus[speedClass][pos.row] |= static_cast<uint32_t>(0b00000001) << pos.col;
                return 0;
            }
        }
    }
    return -1;
}


/**
 *
 *
 */
int LedMatrix::ledBlinkOff(const LedMatrixPos pos, const uint8_t blinkSpeed) {
    if (isValidRowCol(pos) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                blinkStatus[speedClass][pos.row] &= ~ (static_cast<uint32_t>(1) << pos.col);
                return 0;
            }
        }
    }
    return -1;
}


/**
 *
 *
 */
int LedMatrix::isLedBlinkOn(const LedMatrixPos pos, const uint8_t blinkSpeed) {

    if (isValidRowCol(pos) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                return static_cast<int>(((blinkStatus[speedClass][pos.row]
                            & (static_cast<uint32_t>(1) << pos.col)) != 0));
            }
        }
    }
    return -1;  // unzulässige Row oder Col; muss zwischen 0 und LED_ROWS - 1 bzw. LED_COLS - 1 sein
}


/**
 *
 *
 */
int LedMatrix::set7SegValue(const LedMatrixPos pos, const uint8_t charBitMap, const bool dpOn) {
    // prüfen, ob insbes. alle 8 cols, die für ein 7-Segement-Display benötigt werden, innerhalb
    // des gültigen Berichs liegen
    if (isValidRowCol(pos) && isValidRowCol({pos.row, static_cast<uint8_t>(pos.col + 7)})) {
        // row und col0 passen
        // NOLINTNEXTLINE
        matrix[pos.row] &= ~ (static_cast<uint32_t>(0b11111111) << pos.col);  // alle Bits der 7-Segm.-Anz. löschen
        matrix[pos.row] |= static_cast<uint32_t>(charBitMap) << pos.col;
        if (dpOn) {
            ledOn({pos.row, static_cast<uint8_t>(pos.col + 7)});   // NOLINT: der Dezimalpunkt ist immer das höchstwertigste Bit im Zeichenbyte
        } else {
            ledOff({pos.row, static_cast<uint8_t>(pos.col + 7)});   // NOLINT: der Dezimalpunkt ist immer das höchstwertigste Bit im Zeichenbyte
        }
        return 0;
    }
    // unzulässige Row oder Col
    return -1;
}


/**
 *
 *
 */
int LedMatrix::set7SegBlinkOn(const LedMatrixPos pos, const bool dpBlink, const uint8_t blinkSpeed) {
    // Blinken der 7-Segment-Anzeige und ggf. auch des Dezimalpunkts einschalten
    // Dabei nach Blinkgeschwindigkeiten unterscheiden
    // NOLINTNEXTLINE
    if (isValidRowCol(pos) && isValidRowCol({pos.row, static_cast<uint8_t>(pos.col + 7)})
            && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                if (dpBlink) {
                    // alle Bits inkl. Dezimalpunkt des 7-Segment-Displays zum Blinken einschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][pos.row] |= static_cast<uint32_t>(0b11111111) << pos.col;
                } else {
                    // alle Bits, aber ohne Dezimalpunkt, des 7-Segment-Displays zum Blinken einschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][pos.row] |= static_cast<uint32_t>(0b01111111) << pos.col;  // alle Bits, aber ohne Dezimalpunkt, des 7-Segment-Displays zum Blinken markieren
                }
                break;
            }
        }
        return 0;
    }
    // unzulässige Row oder Col
    return -1;
}


/**
 *
 *
 */
int LedMatrix::set7SegBlinkOff(const LedMatrixPos pos, const bool dpBlink, const uint8_t blinkSpeed) {
    // Blinken der 7-Segment-Anzeige und ggf. auch des Dezimalpunkts ausschalten
    // Dabei nach Blinkgeschwindigkeiten unterscheiden
    // NOLINTNEXTLINE
    if (isValidRowCol(pos) && isValidRowCol({pos.row, static_cast<uint8_t>(pos.col + 7)})
            && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                if (dpBlink) {
                    // das Blinker aller Bits inkl. Dezimalpunkt des 7-Segment-Displays ausschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][pos.row] &= ~ (static_cast<uint32_t>(0b11111111) << pos.col);
                } else {
                    // das Blinken aller Bits, aber ohne Dezimalpunkt, des 7-Segment-Displays einschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][pos.row] &= ~ (static_cast<uint32_t>(0b01111111) << pos.col);
                }
                break;
            }
        }
        return 0;
    }
    // unzulässige Row oder Col
    return -1;
}


/**
 *
 *
 */
void LedMatrix::defineDisplayField(const uint8_t &fieldId, const uint8_t &led7SegmentId,
                                   const LedMatrixPos &matrixPos) {
    if ((fieldId <= MAX_DISPLAY_FIELDS) && (led7SegmentId <= MAX_7SEGMENT_UNITS)) {
        displays[fieldId].led7SegmentRows[led7SegmentId] = matrixPos.row;
        displays[fieldId].led7SegmentCol0s[led7SegmentId] = matrixPos.col;
        displays[fieldId].count7SegmentUnits = max(led7SegmentId, displays[fieldId].count7SegmentUnits);
    }
};


/**
 *
 *
 */
void LedMatrix::display(const uint8_t &fieldId, const String &outString) {
    bool dpOn = false;         // Flag, ob Dezimalpunkt im akt. 7-Segment-Display angezeigt wird
    uint8_t dpKorrektur = 0;   // Korrektur zum Positionszähler, falls Dezimalpunkt(e) gefunden
    uint8_t led7SegmentIndex = 0;  // Index für die 7-Segm.-Anz., wo das Zeichen ausgegeben wird
                                   // Da je 7-Segm.-Anz. nur ein Zeichen ausgegeben werden kann,
                                   // ist das gleichzeitg die akt. Position im outString.
    uint8_t charBitMap = 0;    // Bitmap des auf der 7-Segment-Anzeige darzustellenden Zeichens

    // Den anzuzeigenden outString Zeichen für Zeichen abklappern...
    for (const auto &outChar : outString) {
        // Konstante zum Ausrechnen des charMapIndex aus dem ASCII-Code
        // Falls das aktuelle Zeichen ein Dezimalpunkt ist, dieses übergehen, da es bereits
        // verarbeitet bzw. anderweitig verarbeitet wird.
        if (outChar == '.') {
            dpKorrektur++;
        } else {
            // Bitmap für das Zeichen holen;
            charBitMap = charMap.get7SegBitMap(outChar);
            // Prüfen, ob das dem aktuellen Zeichen folgende Zeichen ein Dezimalpunkt ist und Flag entsprechend setzen.
            if (outString[led7SegmentIndex + 1] == '.') {
                dpOn = true;    // NOLINT
            } else {
                dpOn = false;
            }
            // outChar auf der richtigen 7-Segment-Anzeige anzeigen lassen
            set7SegValue({displays[fieldId].led7SegmentRows[led7SegmentIndex - dpKorrektur],
                         displays[fieldId].led7SegmentCol0s[led7SegmentIndex - dpKorrektur]},
                        charBitMap, dpOn);
        } // if outChar ist Dezimalpunkt
        led7SegmentIndex++;
    } // for
};


/*********************************************************************************************************//**
 * ab hier die privaten Methoden
*************************************************************************************************************/

/**
 * @brief Prüfen, ob @em row und @em col gültig sind, d.h.\ innerhalb der Arraygrenzen liegen.\ Gültig
 * heißt, @em row und @em col ist jeweils in [0..LED_ROWS -1 bzw.\ 0..LED_COLS - 1]
 *
 * @param pos row und col Die Nummer der Zeile und Spalte in der LedMatrix.
 *
 * @return @em true Sowohl @em row als auch @em col sind gültig, d.h. innerhalb der Arraygrenzen.
 * @return @em false @em row oder @em col liegen außerhalb der Arraygrenzen.
 */
bool LedMatrix::isValidRowCol(const LedMatrixPos pos) {
    /// Zulässige Werte row: [0..LED_ROWS - 1],
    /// Zulässige Werte col: [0..LED_COLS - 1].
    return ((pos.row < LED_ROWS) && (pos.col < LED_COLS));
};


/**
 * @brief Prüfen, ob blinkSpeed gültig ist, d.h.\ eine der @em blinkSpeed Konstanten ist.
 *
 * @param blinkSpeed Eine der @em blinkSpeed Konstanten.
 * @return @em true @em blinkSpeed ist gültig.
 * @return @em false blinkSpeed ist nicht gültig.
 */
bool LedMatrix::isValidBlinkSpeed(const uint8_t blinkSpeed) {
    return (blinkSpeed < NO_OF_SPEED_CLASSES);
}


/**
 * @brief Prüfen, ob geblinkt werden muss. Falls nämlich nicht, spart man sich
 *        einen Haufen Aufwand.
 *        Diese Funktion wird von LedMatrix::doBlink() verwendet.
 */
bool LedMatrix::isSomethingToBlink() {
    /// checken, ob überhaupt (normal) geblinkt werden muss:
    /// Wenn alle Werte in blinkStatus Null sind, ist für alle LEDs das Blinken ausgeschaltet.
    /// Sollte für mindestens eine LED das Blinken eingeschaltet sein, wird blinkOn auf true gesetzt.
    bool blinkOn = false;
    for (auto &bs : blinkStatus) {
        for (auto &col : bs) {
            blinkOn = (col != 0);
            if (blinkOn) {
                break;
            }
        }
        if (blinkOn) {
            break;
        }
    }
    return blinkOn;
}


/**
 * @brief Ein-/Aus-Status für die LEDs gemäß der aktuellen Hell-/Dunkelphase des Blinkens festlegen.
 */
void LedMatrix::doBlink() {
    /// Wenn was zu blinken ist und das Blinkintervall abgelaufen ist, die Blinkphase umschalten.
    /// isBlinkDarkPhaseXXX wurde mit Anfangs mit false initialisiert, d.h. das Blinken startet
    /// immer mit einer Hellphase.
    bool blinkOn = isSomethingToBlink();

    if (blinkOn) {
        /// Check auf Zeitablauf, so dass die Hell- und Dunkelphasen umgeschaltet werden müssen
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (millis() - blinkStartTime[speedClass] > nextBlinkInterval[speedClass]) {
                if (isBlinkDarkPhase[speedClass]) {
                    nextBlinkInterval[speedClass] = blinkTimes[speedClass].getBrightTime();
                } else {
                    nextBlinkInterval[speedClass] = blinkTimes[speedClass].getDarkTime();
                }
                isBlinkDarkPhase[speedClass] = ! isBlinkDarkPhase[speedClass];
                blinkStartTime[speedClass] = millis(); // + speedClass * BLINK_VERSATZ;
            }
        }
    }

    /// Die matrix in die hwMatrix kopieren, die die LEDs steuert. Während der Dunkelphase müssen die entsprechenden
    /// blinkenden LEDs ausgeschaltet werden.
    for (uint32_t row = 0; row != LED_ROWS; ++row) {
        hwMatrix[row] = matrix[row];
        /// Wenn was zu blinken ist, die hwMatrix entsprechen korrigieren
        if (blinkOn) {
            for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
                if (isBlinkDarkPhase[speedClass]) {
                    hwMatrix[row] &= ~ blinkStatus[speedClass][row];      // LEDs, die blinken sollen, dunkel schalten
                }
            }
        }
    }
}
