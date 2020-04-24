/**************************************************************************************************
 * @file switch.hpp
 * @author Christian Harraeus (christian@harraeus.de)
 * @brief Interface der Klasse @em Switch
 * @version 0.1
 * @date 2020-04-10
 * 
 * Copyright © 2017 - 2020 Christian Harraeus. All rights reserved.
 * 
 **************************************************************************************************/

#ifndef switch_hpp
#define switch_hpp

#include <Arduino.h>

/**************************************************************************************************
 * @brief Abbildung eines Schalters
 * @todo Doku an dieser Stelle noch ausführlicher.
 * 
 **************************************************************************************************/
class Switch {
private:
    uint8_t status {HIGH};          ///< Status 0 --> eingeschaltet, Status 1 --> ausgeschaltet
    String switchName{""};          ///< Name des Schalters; wird durch Konstruktor gesetzt
    bool longOn {false};            ///< true => falls Schalter ist länger als 3 Sek\. an
    bool longOnSent {true};         ///< true => Ereignis wurde schon gesetzt und muss nicht nochmal gefeuert werden
    unsigned long onTimeStart {0};  ///< Zeitstempel wann Schalter eingeschaltet wurde
    unsigned long onTime {0};       ///< Dauer wie lange der Schalter eingeschaltet war
    bool changed {false};           ///< true => Schalterstatus wurde seit der letzten Änderung nicht abgefragt
    
    /**
     * @brief Differenz zweier Zeitstempel berechnen
     * 
     * Berechnet die Differenz zweier Zeitstempel und gibt diese als Ergebnis 
     * zurück. Ein eventueller Überlauf (passiert ca. nach 50 Tagen) wird berücksichtigt.
     * 
     * @param [in] onTime Einschaltzeitstempel in Millisekunden, z.B. millis(). Dieser Parameter ist i.d.R. der ältere Zeitstempel.
     * @param [in] offTime Ausschaltzeitstempel in Millisekunden, z.B. millis(). Dieser Parameter ist i.d.R. der neuere Zeitstempel.
     * @return unsigned long Zeitdifferenz in Millisekunden
     */
    unsigned long calcTimeDiff(const unsigned long onTime, const unsigned long offTime);    
   
public:
    /**  
     * @brief Constructor zum Anlegen eines Schalterobjekts.
     *  Initialisiert den Schalter.
     * @param [in] newName Ein String mit dem Namen des Schalters. 
     * @note       newName sollte nicht allzu lang sein, um den Arduino-Speicher nicht so schnell zu füllen. 
     * */
    Switch(const String &newName) : switchName {newName} {}
    
    /**
     * @brief Setzt den Status des Schalters auf "on" und setzt den Zeitstempel, wann der Schalter eingeschaltet wurde.
     * 
     */
    void setOn();
    
    /**
     * @brief Setzt den Status des Schalters auf "off" und berechnet die Zeit, wie lange der Schalter eingeschaltet war.
     * 
     */
    void setOff();
    
    /** 
     * Prüfen, ob ein Schalter länger als 3 Sekunden eingeschaltet ist.
     * Falls ein Schalter länger als 3 Sekunden eingeschaltet ist, wird die @em changed Eigenschaft des Schalters
     * auf @em true gesetzt.
     * Die Zeitdauer ist in der Konstanten @em LONG_ON hinterlegt.
     */
    void checkLongOn();
    
    /**
     * Einschaltzeit berechnen und sichern.
     *
     * Schreibt die aktuelle Zeit, die der Schalter eingeschaltet ist, in die Eigenschaft onTime.
     * @note Diese Methode muss regelmäßig aufgerufen werden.
     * 
     * @param [in] newOnTime: Aktueller Zeitstempel in Millisekunden, z.B. millis()
     */
    void updateOnTime(const unsigned long newOnTime); 
    
    /**
     * Den Status eines Schalters (@em ON oder @em off) übertragen.
     *  Wenn der Schalter länger als @em LONG_ON Millisekunden an ist, wird zusätzlch - aber
     * nur einmal - der Status @em LON (für \"long on\") gesendet. 
     * @todo hier muss noch der Schalterstatus irgendwie an X-Plane übergeben werden.
     */
    void transmitStatus();
    
    /** 
     * Fragt ab, ob der Schalter eingeschaltet ist.
     * Nach Rückgabe des Status wird die @em changed Eigenschaft auf @em false gesetzt.
     * @return true wenn der Schalter eingeschaltet ist. 
     */ 
     bool isOn() { return getStatus() == 0; }
    
    /** 
     * Gibt den Änderungstatus des Schalters zurück.
     * Der Änderungsstatus wird durch Ein-/Ausschalten verändert.
     * @return true wenn der Schalterstatus seit der letzten Änderung nicht abgefragt wurde. 
     */ 
    bool isChanged() { return changed; }    
    
    /** 
     * Gibt den Namen des Schalters zurück.
     * @return Name des Schalters.
     */ 
    String getName() { return switchName; }
    
    /** 
     * Fragt den Schalterstatus ab und setzt den Änderungsstatus des Schalters zurück. 
     * Nach Rückgabe des Schalterstatus, wird der Änderungsstatus des Schalters, 
     * d.h. die Eigenschaft @em changed, auf @em false gesetzt.
     * @return @em 0, wenn der Schalter eingeschaltet ist.
     * @return @em 1, wenn der Schalter ausgeschaltet ist.
     * @see getStatusNoChange()
     */
    uint8_t getStatus() { changed = false; return status; } 
    
    /**
     * Fragt den Schalterstatus ab ohne den Änderungsstatus des Schalters zurückzusetzen. 
     * Der Änderungstatus des Schalters, d.h. die Eigenschaft @em changed, bleibt unverändert.
     * @return @em 0, wenn der Schalter eingeschaltet ist.
     * @return @em 1, wenn der Schalter ausgeschaltet ist.
     * @see getStatus()
     */
    uint8_t getStatusNoChange() { return status; }  
};

#endif /* #ifdef switch_hpp */