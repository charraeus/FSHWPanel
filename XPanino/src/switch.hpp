/*********************************************************************************************************//**
 * @file switch.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em Switch
 * @version 0.1
 * @date 2020-04-10
 *
 * Copyright © 2017 - 2020 Christian Harraeus. All rights reserved.
 *
 ************************************************************************************************************/

#pragma once

#include <Arduino.h>

/*********************************************************************************************************//**
 * @brief Abbildung eines Schalters
 *
 * @todo Doku an dieser Stelle noch ausführlicher.
 * @todo Softwaremäßiges Entprellen wie im Artikel "Fliegender Frederick" in c't 22/2020 implementieren
 *       Siehe Function debounce. --> ist das notwendig?
 *
 ************************************************************************************************************/
class Switch {
public:
    /**
     * @brief Setzt den Status des Schalters auf "on" und setzt den Zeitstempel, wann der Schalter
     *        eingeschaltet wurde.
     */
    void setOn();


    /**
     * @brief Setzt den Status des Schalters auf "off" und berechnet die Zeit, wie lange der
     *        Schalter eingeschaltet war.
     */
    void setOff();


    /**
     * @brief Prüfen, ob ein Schalter länger als 3 Sekunden eingeschaltet ist.
     */
    void checkLongOn();


    /**
     * @brief Einschaltzeit berechnen und sichern.
     *
     * Schreibt die aktuelle Zeit, die der Schalter eingeschaltet ist, in die Eigenschaft onTime.
     * @note Diese Methode muss regelmäßig aufgerufen werden.
     *
     * @param newOnTime Aktueller Zeitstempel in Millisekunden, z.B. millis()
     */
    void updateOnTime(const unsigned long &newOnTime);


    /**
     * @brief Den Status eines Schalters (@em ON oder @em off) übertragen. Dabei wird die Eigenschaft
     *        @em changed des Schalters auf @false gesetzt.
     *
     * Wenn der Schalter länger als @em LONG_ON Millisekunden an ist, wird zusätzlich -- aber
     * nur einmal -- der Status @em LON (für \"long on\") gesendet.
     *
     * Wenn das Ausgabeformat geändert werden soll, muss eine von der Klasse @em Switch abgeleitete, neue
     * Klasse erstellt werden, und dort die Methode @transmit überschrieben werden.
     *
     * @see Methode @transmit
     */
    void transmitStatus(uint8_t &row, uint8_t &col);


    /**
     * @brief Physical transmitssion of the data. This method has to be overwritten in every derived class.
     *
     * @param row Row of switch in the switch matrix
     * @param col Column of switch in the switch matrix
     * @param switchState C-string with the string data to submit:
     *                      switchState = 0: Switch is off
     *                      switchState = 1: Switch is on
     *                      switchState = 2: Switch is long on
     */
    void transmit(uint8_t &row, uint8_t &col, uint8_t &switchState);


    /**
     * @brief Fragt ab, ob der Schalter eingeschaltet ist.
     *
     * Nach Rückgabe des Status wird die @em changed Eigenschaft auf @em false gesetzt.
     *
     * @return @em true wenn der Schalter eingeschaltet ist.
     */
    inline bool isOn() { return getStatus() == 0; }


    /**
     * @brief Gibt den Änderungstatus des Schalters zurück\. Der Änderungsstatus wird
     *        Ein-/ Ausschalten verändert.
     *
     * @return @em true wenn der Schalterstatus seit der letzten Änderung nicht abgefragt wurde,\n
     *         @em false sonst.
     */
    inline bool isChanged() const { return changed; }


    /**
     * @brief Fragt den Schalterstatus ab und setzt den Änderungsstatus des Schalters zurück.
     *
     * Nach Rückgabe des Schalterstatus, wird der Änderungsstatus des Schalters,
     * d.h. die Eigenschaft @em changed, auf @em false gesetzt.
     *
     * @return @em 0, wenn der Schalter eingeschaltet ist.\n
     *         @em 1, wenn der Schalter ausgeschaltet ist.
     *
     * @see getStatusNoChange()
     */
    uint8_t getStatus() { changed = false; return status; }


    /**
     * @brief Fragt den Schalterstatus ab ohne den Änderungsstatus des Schalters zurückzusetzen.
     * Der Änderungstatus des Schalters, d.h. die Eigenschaft @em changed, bleibt unverändert.
     *
     * @return @em 0, wenn der Schalter eingeschaltet ist.\n
     *         @em 1, wenn der Schalter ausgeschaltet ist.
     *
     * @see getStatus()
     */
    uint8_t getStatusNoChange() const { return status; }

private:
    uint8_t status {HIGH};   ///< Status 0 --> eingeschaltet, Status 1 --> ausgeschaltet
    bool longOn {false};     ///< true => falls Schalter ist länger als 3 Sek\. an
    bool longOnSent {true};  ///< true => Ereignis wurde schon gesetzt und muss nicht nochmal gefeuert werden
    unsigned long switchPressTime {0};  ///< Zeitstempel wann Schalter eingeschaltet wurde
    unsigned long onTime {0};  ///< Dauer wie lange der Schalter eingeschaltet war
    bool changed {false};    ///< true => Schalterstatus wurde seit der letzten Änderung nicht abgefragt
    uint8_t history;         ///< notwendig für debounce(); macht evtl. switchPressTime, onTime überflüssig

    static unsigned long calcTimeDiff(const unsigned long &onTime,
                                      const unsigned long &offTime);
    uint8_t debounce(uint8_t &history, uint8_t &newStatus);
};
