/*********************************************************************************************************//**
 * @file dispatcher.hpp
 * @brief Dispatcher-Klasse
 *
 * @author Christian Harraeus (christian@harraeus.de)
 * @date 2022-12-13
 * @version 0.2
 *
 ***********************************************************************************************************/

#pragma once

#include <event.hpp>
#include <m803.hpp>
#include <Switchmatrix.hpp>
#include <xpdr.hpp>

extern ClockDavtronM803 m803;
extern TransponderKT76C xpdr;
extern EventQueueClass eventQueue;


/***************************************************************************************************
 * @brief dispatchen
 *
 */
class DispatcherClass {
public:
    /**
     * @brief Dispatch a specific event.
     *
     * @param event Event to dispatch.
     */
    void dispatch(EventClass *event) const;


    /**
     * @brief dispatch all events in the event queue.
     *
     */
    void dispatchAll();


    /**
     * @brief Dispatch a switch status change event.
     *
     * @param row
     * @param col
     * @param switchState
     */
    void dispatchSwitchEvents(const uint8_t &row, const uint8_t &col, uint8_t switchState);


    /**
     * @brief Get the Switch Name object
     *
     * @param row Row in switch matrix.
     * @param col Column in switch matrix.
     * @return char* Pointer to buffer with switch name.
     */
    char *getSwitchName(const uint8_t &row, const uint8_t &col);

private:
    /**
     *  @brief Array of switch names according to their position in the switch matrix
     *
     * ## Anordnung der Schalter für Uhr und Transponder in der Schaltermatrix
     *
     * |              |**Col 0** |**Col 1** |**Col 2** |**Col 3** |**Col 4** |**Col 5** |**Col 6** |**Col 7** |
     * |--------------|----------|----------|----------|----------|----------|----------|----------|----------|
     * |**Row 0**     |0.0<br />XPDR<br />*IDT*       |0.1<br />XPDR<br />*VFR*       |0.2<br />XPDR<br />*CLR*       |0.3<br />XPDR<br />*OFF*       |0.4<br />XPDR<br />*SBY*       |0.5<br />XPDR<br />*TST*       |0.6<br />XPDR<br />*ON*       |0.7<br />XPDR<br />*ALT*       |
     * |**Row 1**     |1.0<br />XPDR<br />*0*       |1.1<br />XPDR<br />*1*       |1.2<br />XPDR<br />*2*       |1.3<br />XPDR<br />*3*       |1.4<br />XPDR<br />*4*       |1.5<br />XPDR<br />*5*       |1.6<br />XPDR<br />*6*       |1.7<br />XPDR<br />*7*       |
     * |**Row 2**     |2.0<br />Uhr<br />*SELECT*       |2.1<br />Uhr<br />*CONTROL*       |2.2<br />Uhr<br />*O.A.T*       |2.3       |2.4       |2.5       |2.6       |2.7       |
     *
     * **ACHTUNG:** Die Col-Nummer muss noch auf die Gegebenheiten des Anschlusses an den Arduino anstatt an den IOW angepasst werden!
     *
     * @todo Matrix umlöten wie im Dokument @em DavtronM803.md beschrieben.
     *
     */
    const char *switchNames[SWITCH_MATRIX_ROWS][SWITCH_MATRIX_COLS] = {
                    { "0", "1", "2", "3", "4", "5", "6", "7" },
                    { "IDT", "11", "12", "OFF", "SBY", "TST", "ON", "ALT" },
                    { "SEL", "CTL", "OAT", "23", "24", "25", "OAT", "27" },
                    { "30", "31", "32", "33", "34", "35", "SEL", "CTL" }
    };

    const uint8_t switchId[SWITCH_MATRIX_ROWS][SWITCH_MATRIX_COLS] = {
                    { 00, 01, 02, 03, 04, 05, 06, 07 },
                    { 10, 11, 12, 13, 14, 15, 16, 17 },
                    { 20, 21, 22, 23, 24, 25, CLOCK_BTN_OAT, 27 },
                    { 30, 31, 32, 33, 34, 35, CLOCK_BTN_SEL, CLOCK_BTN_CTL }
    };
};