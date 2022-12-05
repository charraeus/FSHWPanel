# Kommunikation zwischen Hardware und X-Plane auf dem PC {#Kommunikation}

## Übertragung der Daten zwischen PC und Arduino

### Allgemeiner Aufbau

Die Kommunikation erfolgt durch wechselseitiges Senden/Empfangen von Kommandostrings. Ein Kommandostring besteht immer aus 5 verschiedenen Teilen, die im Ganzen als zusammengesetzter String verschickt werden. Die einzelnen Teile sind jeweils durch ein Blank getrennt. Am Ende steht immer ein \<CR> und/oder ein \<LF>.

1. Source: Quelle des Kommandos,
1. Device: Gerät, das angesprochen werden soll, oder von dem Daten kommen,
1. Action: Aktion, die vom Device ausgeführt wurde, oder ausgeführt werden soll,
1. Para 1: 1. Parameter zur Aktion, d.h. die Nutzdaten (optional),
1. Para 2: 2. Parameter zur Aktion, d.h. die Nutzdaten (optional).

Kommandoteil | Datentyp
-------------|--------------------------
Source       | string
Device       | char / uint8_t
Action       | uint8_t (1 Byte)
Para 1       | @todo ergänzen
Para 2       | @todo ergänzen

**Beispiele**
Kommandostring | Bedeutung
---------------|------------------------
`XP D X7000`   | Im Transponderfeld den Wert '7000' anzeigen.
`XP D C 20.3`   | Im O.A.T.-Feld den Wert ' 20.3' anzeigen.<br/>Hier wird es ggf. knifflig beim parsen, da das Blank hier keinen Parameter-Trenner darstellt.

### Source

Source | Beschreibung
-------|----------------------------------------------------------------------
XP     | Die Daten kommen vom X-Plane
ARD1   | Id des ext. Geräts, hier Arduino 1 (es können ja noch mehr kommen...)

### Devices

const-Name | Device | Beschreibung
-----------|:------:|-----------------------
DATA       | D      | Keine Device; es kommen Daten
SWITCH     | S      | Schalter oder Taster
LED        | L      | Eine einzelne LED
DISP       | 7      | Ein 7-Segment-Display

Für die Entwicklungs- und Testphase werde Buchstaben statt roher Bytes verwendet, da diese im Terminal direkt gelesen werden können.

Vom PC zum Arduino gesendete Actions
---------------------------------------------------------------

Siehe auch @ref commands.hpp für die aktuelle Version der Codes.
@todo noch weiter ergänzen und updaten!!

### Steuerkommandos für den Arduino

const-Name        | Action | Beschreibung                                               | Parameter-Typ | Parameter-Beschreibung
------------------|--------|------------------------------------------------------------|---------------|-----------------------
ACK               | 0xFFFF | Acknowledge - Angeforderte Daten für Parameter Code folgen |               | |
RESET_ARDUINO     | 0xFF01 | Arduino neu booten                                         | -             | |
RESEND_SWITCHES   | 0xFF02 | Den Status aller Schalter senden                           | -             | |

### Transponder KT 76C und Uhr Davtron M803

#### Definierte Actions

const-Name        | Action | Beschreibung                                               | Parameter-Typ | Parameter-Beschreibung
------------------|--------|------------------------------------------------------------|---------------|-----------------------
XPDR_CODE         | X      | Den übergebenen XPDR-Code anzeigen                         | ?             | 4-stelliger Transpondercode
XPDR_FLIGHTLEVEL  | L      | Flightlevel für Transponder                                | ?             | 3 Stellen
M803_LT           |        | Aktuelle Uhrzeit (Local)                                   | ?             | Uhrzeit HHMMSS
M803_UT           | U      | Aktuelle Uhrzeit (UTC)                                     | ?             | Uhrzeit HHMMSS
M803_ET           |        | Elapsed Time                                               | ?             | Vergangene Zeit HHMMSS
M803_FT           |        | Flight Time                                                | ?             | Flight Time in HHMMSS
M803_VOLTS        | V      | Spannung in Volt - eigentlich EMF, aber hat der Flusi nicht| ?             | Spannung in Volt
M803_QNH          | Q      | Aktuelles QNH des X-Plane-Wetters                          | ?             | 4 Stellen
M803_ALT          | A      | Aktuelles Altimeter-Setting in inHg                        | ?             |
M803_OATC         | C      | OAT in °C                                                  | ?             | OAT in Celsius
M803_OATF         | F      | OAT in Fahrenheit                                          |               | OAT in Fahrenheit

Für die Entwicklungs- und Testphase werde Buchstaben statt roher Bytes verwendet, da diese im Terminal direkt gelesen werden können.

#### Actions und X-Plane-Datarefs

Action            | X-Plane-Dataref                                                                                 | X-Plane-Typ | r/w
------------------|-------------------------------------------------------------------------------------------------|-------------|----
M803_LT           | sim/cockpit2/clock_timer/local_time_hours, .../local_time_minutes, .../local_time_seconds       | int         |  r
M803_UT           | sim/cockpit2/clock_timer/zulu_time_hours, .../zulu_time_minutes, .../zulu_time_seconds          | int         |  r
M803_ET           | sim/cockpit2/clock_timer/elapsed_time_hours, .../elapsed_time_minutes, .../elapsed_time_seconds | int         |  r
M803_FT           | sim/cockpit2/clock_timer/timer_elapsed_time_sec (=total time elapsed in seconds)                | float       | r/w
M803_VOLTS        | sim/cockpit2/electrical/battery_voltage_actual_volts                                            | float[8]    |  r
M803_QNH          | = M803_ALT * 33.8637526
M803_ALT          | sim/weather/barometer_current_inhg                                                              | float       | r/w
M803_OATC         | sim/cockpit2/temperature/outside_air_temp_degc, .../outside_air_temp_is_metric (int 1=C, 0=F)   | float       |  r
M803_OATF         | sim/cockpit2/temperature/outside_air_temp_degf, .../outside_air_temp_is_metric (int 1=C, 0=F)   | float       |  r

Vom Arduino zum PC gesendete Actions
------------------------------------------------------------------

@todo noch weiter ergänzen und updaten!!

const-Name       | Action | Beschreibung                          | Parameter-Typ            | Parameter-Beschreibung
-----------------|--------|---------------------------------------|--------------------------|-----------------------
SWITCH_ON        | 0x1101 | Schalter/Taster eingeschaltet         | uint8_t row, uint8_t col | Row und Col in der Schaltermatrix
SWITCH_LON       | 0x1102 | Schalter/Taster lange eingeschaltet   | uint8_t row, uint8_t col | Row und Col in der Schaltermatrix
SWITCH_OFF       | 0x1103 | Schalter/Taster ausgeschaltet         | uint8_t row, uint8_t col | Row und Col in der Schaltermatrix
REQUEST_DATA     | 0x1F01 | Daten vom PC anfordern                | uint16_t Arduino-Action  | -
