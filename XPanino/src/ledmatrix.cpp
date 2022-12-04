/**************************************************************************************************
 * @file ledmatrix.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Implementierung der Klasse @em LedMatrix.
 * @version 0.1
 * @date 2017-11-10
 *
 * Copyright © 2017 - 2020. All rights reserved.
 *
 **************************************************************************************************/

#include <ledmatrix.hpp>

/** Konstanten für die Zuordnung der Arduino-Pins zu den MIC5891- und MIC5821-Schieberegister-Leitungen */
const uint8_t CLOCK = PIN4;     ///< Arduino-Pin für CLOCK des MIC5891/5821
const uint8_t DATA_IN = PIN5;   ///< Arduino-Pin für DATA_IN des MIC5891/5821
const uint8_t STRB = PIN3;      ///< Arduino-Pin für STRB des MIC5891/5821
const uint8_t OE = PIN2;        ///< Arduino-Pin für OE des MIC5891/5821

/** Konstanten für's Blinken */
const unsigned int BLINK_VERSATZ = 447;     ///< Versatz für die Startzeiten der Blinkgeschwindigkeiten. Damit
                                            ///< nicht alles so gleich im Takt blinkt



/**************************************************************************************************/
SpeedClass::SpeedClass(const unsigned long int brightTime, const unsigned long int darkTime) {
    this->brightTime = brightTime;
    this->darkTime = darkTime;
};


/**************************************************************************************************/
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
        blinkStartTime[speedClass] = millis() + speedClass * BLINK_VERSATZ;   // Startzeit mit Versatz
    }
    /// Defaultwerte für die Blinkdauern der Speedklassen der nächsten anstehenden Hellphase
    nextBlinkInterval[BLINK_NORMAL] = blinkTimes[BLINK_NORMAL].getBrightTime();    // Dauer der Hellphase als Initialwert
    nextBlinkInterval[BLINK_SLOW] = blinkTimes[BLINK_SLOW].getBrightTime();
}

void LedMatrix::initHardware() {
    /// Die eingebaute LED als Status aktivieren: die LED
    /// vier mal ein- und ausschalten
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


/**************************************************************************************************/
bool LedMatrix::isValidRowCol(const uint8_t row, const uint8_t col) {
    /// Zulässige Werte row: [0..LED_ROWS - 1],
    /// Zulässige Werte col: [0..LED_COLS - 1].
    return ((row < LED_ROWS) && (col < LED_COLS));
};


/**************************************************************************************************/
bool LedMatrix::isValidRowCol(const LedMatrixPos pos) {
    return isValidRowCol(pos.row, pos.col);
}


/**************************************************************************************************/
bool LedMatrix::isValidBlinkSpeed(const uint8_t blinkSpeed) {
    return (blinkSpeed < NO_OF_SPEED_CLASSES);
}

/**************************************************************************************************/
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


/**************************************************************************************************/
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


/*************************************************************************************************
 * @note Diese Funktion muss regelmäßig und sehr häufig innerhalb des loop
 * aufgerufen werden!
 *
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
 *
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
        /// @todo Kommentar (oder Code) korrigieren wg. "statt 'row' der Ausdruck '7 - row'"
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
        //delayMicroseconds(1);     // lt. Datenblatt erforderlich, aber funktioniert auch ohne
    }
}


/**************************************************************************************************/
bool LedMatrix::isLedOn(const uint8_t row, const uint8_t col) {
    if (isValidRowCol(row, col)) {
        return (matrix[row] & (static_cast<uint32_t>(1) << col)) != 0;
    }
    return false;  // unzulässige Row oder Col; muss zwischen 0 und LED_ROWS - 1 bzw. LED_COLS - 1 sein
}

bool LedMatrix::isLedOn(const LedMatrixPos pos) {
    return isLedOn(pos.row, pos.col);
}


/**************************************************************************************************/
int LedMatrix::ledOn(const uint8_t row, const uint8_t col) {
    if (isValidRowCol(row, col)) {
        matrix[row] |= static_cast<uint32_t>(1) << col;    // An der Stelle col soll das Bit gesetzt werden
        return 0;
    }
    return -1;      // unzulässige Row oder Col
}

int LedMatrix::ledOn(const LedMatrixPos pos) {
    return ledOn(pos.row, pos.col);
}


/**************************************************************************************************/
int LedMatrix::ledOff(const uint8_t row, const uint8_t col) {
    if (isValidRowCol(row, col)) {
        matrix[row] &= ~ (static_cast<uint32_t>(1) << col);
        return 0;
    }
    return -1;      // unzulässige Row oder Col
}

int LedMatrix::ledOff(const LedMatrixPos pos) {
    return ledOff(pos.row, pos.col);
}


/**************************************************************************************************/
int LedMatrix::ledToggle(const uint8_t row, const uint8_t col) {
    if ((row >= LED_ROWS) || (col >= LED_COLS)) {
        return -1;      // unzulässige Row oder Col
    }
    if (isLedOn(row, col)) {
        ledOff(row, col);
    } else {
        ledOn(row, col);
    }
    return 0;
}

int LedMatrix::ledToggle(const LedMatrixPos pos) {
    return ledToggle(pos.row, pos.row);
}


/**************************************************************************************************/
int LedMatrix::ledBlinkOn(const uint8_t row, const uint8_t col, const uint8_t blinkSpeed) {
    if (isValidRowCol(row, col) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                // An der Stelle col soll das Bit gesetzt werden
                blinkStatus[speedClass][row] |= static_cast<uint32_t>(1) << col;
                break;
            }
        }
        return 0;
    }
    return -1;
}

int LedMatrix::ledBlinkOn(const LedMatrixPos pos, const uint8_t blinkSpeed) {
    return ledBlinkOn(pos.row, pos.col, blinkSpeed);
}


/**************************************************************************************************/
int LedMatrix::ledBlinkOff(const uint8_t row, const uint8_t col, const uint8_t blinkSpeed) {
    if (isValidRowCol(row, col) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                blinkStatus[speedClass][row] &= ~ (static_cast<uint32_t>(1) << col);
                break;
            }
        }
        return 0;
    }
    return -1;
}

int LedMatrix::ledBlinkOff(const LedMatrixPos pos, const uint8_t blinkSpeed) {
    return ledBlinkOff(pos.row, pos.col, blinkSpeed);
}


/**************************************************************************************************/
int LedMatrix::isLedBlinkOn(const uint8_t row, const uint8_t col, const uint8_t blinkSpeed) {
    int returnvalue = 0;

    if (isValidRowCol(row, col) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                returnvalue = static_cast<int>(((blinkStatus[speedClass][row]
                                 & (static_cast<uint32_t>(1) << col)) != 0));
            }
        }
    } else {
        returnvalue = -1;      // unzulässige Row oder Col; muss zwischen 0 und LED_ROWS - 1 bzw. LED_COLS - 1 sein
    }
    return returnvalue;
}

int LedMatrix::isLedBlinkOn(const LedMatrixPos pos, const uint8_t blinkSpeed) {
    return isLedBlinkOn(pos.row, pos.col, blinkSpeed);
}


/**************************************************************************************************/
int LedMatrix::set7SegValue(const uint8_t row, const uint8_t col0,
                            const uint8_t charBitMap, const bool dpOn) {
    // prüfen, ob insbes. alle 8 cols, die für ein 7-Segement-Display benötigt werden, innerhalb
    // des gültigen Berichs liegen
    // NOLINTNEXTLINE: Es muss nunmal der Wert 7 addiert werden (Bytelogik!)
    if (isValidRowCol(row, col0) && isValidRowCol(row, col0 + 7)) {
        // row und col0 passen
        // NOLINTNEXTLINE
        matrix[row] &= ~ (static_cast<uint32_t>(0b11111111) << col0);  // alle Bits der 7-Segm.-Anz. löschen
        matrix[row] |= static_cast<uint32_t>(charBitMap) << col0;
        if (dpOn) {
            ledOn(row, col0 + 7);   // NOLINT: der Dezimalpunkt ist immer das höchstwertigste Bit im Zeichenbyte
        } else {
            ledOff(row, col0 + 7);   // NOLINT: der Dezimalpunkt ist immer das höchstwertigste Bit im Zeichenbyte
        }
        return 0;
    }
    // unzulässige Row oder Col
    return -1;
}

int LedMatrix::set7SegValue(const LedMatrixPos pos, const unsigned char newValue, const bool dpOn) {
    return set7SegValue(pos.row, pos.col, newValue, dpOn);
}


/**************************************************************************************************/
int LedMatrix::set7SegBlinkOn(const uint8_t row, const uint8_t col0, const bool dpBlink,
                               const uint8_t blinkSpeed) {
    // Blinken der 7-Segment-Anzeige und ggf. auch des Dezimalpunkts einschalten
    // Dabei nach Blinkgeschwindigkeiten unterscheiden
    // NOLINTNEXTLINE
    if (isValidRowCol(row, col0) && isValidRowCol(row, col0 + 7) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                if (dpBlink) {
                    // alle Bits inkl. Dezimalpunkt des 7-Segment-Displays zum Blinken einschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][row] |= static_cast<uint32_t>(0b11111111) << col0;
                } else {
                    // alle Bits, aber ohne Dezimalpunkt, des 7-Segment-Displays zum Blinken einschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][row] |= static_cast<uint32_t>(0b01111111) << col0;  // alle Bits, aber ohne Dezimalpunkt, des 7-Segment-Displays zum Blinken markieren
                }
                break;
            }
        }
        return 0;
    }
    // unzulässige Row oder Col
    return -1;
}

int LedMatrix::set7SegBlinkOn(const LedMatrixPos pos, const bool dpBlink, const uint8_t blinkSpeed) {
    return set7SegBlinkOn(pos.row, pos.col, dpBlink, blinkSpeed);
}


/**************************************************************************************************/
int LedMatrix::set7SegBlinkOff(const uint8_t row, const uint8_t col0, const bool dpBlink,
                               const uint8_t blinkSpeed) {
    // Blinken der 7-Segment-Anzeige und ggf. auch des Dezimalpunkts ausschalten
    // Dabei nach Blinkgeschwindigkeiten unterscheiden
    // NOLINTNEXTLINE
    if (isValidRowCol(row, col0) && isValidRowCol(row, col0 + 7) && isValidBlinkSpeed(blinkSpeed)) {
        for (uint8_t speedClass = 0; speedClass != NO_OF_SPEED_CLASSES; ++speedClass) {
            if (blinkSpeed == speedClass) {
                if (dpBlink) {
                    // das Blinker aller Bits inkl. Dezimalpunkt des 7-Segment-Displays ausschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][row] &= ~ (static_cast<uint32_t>(0b11111111) << col0);
                } else {
                    // das Blinken aller Bits, aber ohne Dezimalpunkt, des 7-Segment-Displays einschalten
                    // NOLINTNEXTLINE
                    blinkStatus[speedClass][row] &= ~ (static_cast<uint32_t>(0b01111111) << col0);
                }
                break;
            }
        }
        return 0;
    }
    // unzulässige Row oder Col
    return -1;
}

int LedMatrix::set7SegBlinkOff(const LedMatrixPos pos, const bool dpBlink, const uint8_t blinkSpeed) {
    return set7SegBlinkOff(pos.row, pos.col, dpBlink, blinkSpeed);
}


/**************************************************************************************************/
void LedMatrix::defineDisplayField(const uint8_t &fieldId, const uint8_t &led7SegmentId,
                                   const uint8_t &matrixRow, const uint8_t &matrixCol0) {
    if ((fieldId <= MAX_DISPLAY_FIELDS) && (led7SegmentId <= MAX_7SEGMENT_UNITS)) {
        displays[fieldId].led7SegmentRows[led7SegmentId] = matrixRow;
        displays[fieldId].led7SegmentCol0s[led7SegmentId] = matrixCol0;
        displays[fieldId].count7SegmentUnits = max(led7SegmentId, displays[fieldId].count7SegmentUnits);
    }
};

void LedMatrix::defineDisplayField(const uint8_t &fieldId, const uint8_t &led7SegmentId,
                                   const LedMatrixPos &matrixPos) {
    defineDisplayField(fieldId, led7SegmentId, matrixPos.row, matrixPos.col);
}


/**************************************************************************************************/
// void LedMatrix::display(const uint8_t &fieldId, const char* outString) {
//     char ch = 0;
//     bool dpOn = false;          // Flag, ob Dezimalpunkt im aktuellen 7-Segment-Display angezeigt werden soll.
//     uint8_t dpKorrektur = 0;    // Korrektur zum Positionszähler, falls Dezimalpunkt(e) gefunden.

//     for (uint8_t c = 0; c != strlen(outString); ++c) {
//         if (outString[c] != '.') {      // Falls das aktuelle Zeichen ein Dezimalpunkt ist, dieses übergehen, da es bereits verarbeitet wurde.
//             if (((outString[c] - 48) >= 0) && ((outString[c] - 48) <= 9)) {     // Prüfen auf Ziffer 0...9
//                 ch = outString[c] - 48;
//             } else {
//                 if (((outString[c] - 54) >= 11) && ((outString[c] - 54) <= 31)) {   // Prüfen auf Buchstabe A..U
//                     ch = outString[c] - 54;
//                 } else {                                        // weder Ziffer, noch Buchstabe => auf Sonderzeichen prüfen
//                     switch (outString[c]) {
//                         case ' ':               ch = CHAR_BLANK; break;
//                         case 'c':               ch = CHAR_c; break;
//                         case 'o':               ch = CHAR_o; break;
//                         case 'u':               ch = CHAR_u; break;
//                         case '-':               ch = CHAR_MINUS; break;
//                         case '_':               ch = CHAR_UNDERSCORE; break;
//                         case CHAR_DEGREE:       ch = CHAR_DEGREE; break;
//                         case CHAR_3_DASH_HORIZ: ch = CHAR_3_DASH_HORIZ; break;
//                         case CHAR_2_DASH_VERT:  ch = CHAR_2_DASH_VERT; break;
//                         default:                ch = CHAR_ERROR;
//                     }
//                 }
//             }


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
                dpOn = true;
            }
            // outChar auf der richtigen 7-Segment-Anzeige anzeigen lassen
            set7SegValue(displays[fieldId].led7SegmentRows[led7SegmentIndex - dpKorrektur],
                         displays[fieldId].led7SegmentCol0s[led7SegmentIndex - dpKorrektur],
                        charBitMap, dpOn);
        } // if outChar ist Dezimalpunkt
        led7SegmentIndex++;
    } // for
};
