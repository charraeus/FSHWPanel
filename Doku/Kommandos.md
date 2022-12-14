# Kommunikation zwischen Hardware und X-Plane auf dem PC {#Kommunikation}

## Allgemeiner Aufbau
Die Kommunikation erfolgt durch wechselseitiges Senden/Empfangen von Kommandostrings. Ein Kommandostring besteht immer aus 5 verschiedenen Teilen, die im Ganzen als zusammengesetzter String verschickt werden. Die einzelnen Teile sind jeweils durch ein Blank getrennt. Am Ende steht immer ein \<CR> ("\\r") und/oder ein \<LF> ("\\n").

1. Device: Gerät, das angesprochen werden soll, oder von dem Daten kommen,
1. Action: Aktion, die vom Device ausgeführt wurde, oder ausgeführt werden soll,
1. Para 1: 1. Parameter zur Aktion, d.h. die Nutzdaten (optional),
1. Para 2: 2. Parameter zur Aktion, d.h. die Nutzdaten (optional).

|Kommandoteil | Datentyp       |
|-------------|----------------|
|Device       | char / uint8_t |
|Event        | char*          |
|Para 1       |                |
|Para 2       |                |

@todo noch anpassen und ergänzen

[![Syntaxdiagramm des Kommandos.][bild-01] Syntaxdiagramm des Kommandos][bild-01]

**Beispiele**

Kommandostring | Bedeutung
:--------------|------------------------
`X;X;7000`     | Nachricht von X-Plane an Arduino: Im Transponderfeld den Wert "7000" anzeigen.
`X;C; 20.3`    | Nachricht von X-Plane an Arduino: Im O.A.T.-Feld den Wert " 20.3" anzeigen. Der @ steht für ein Blank. Dieser Sonderfall ist nötig, damit das parsen einfacher wird (Blanks trennen die einzelnen Bestandteile des Kommandostrings)
`S;ON;2;3`     | Nachricht vom Transponder: Der **S**chalter an der Position row=**2** und col=**3** (in der Schaltermatrix) wurde eingeschaltet (**ON**).
| |

Für die Entwicklungs- und Testphase werde Buchstaben statt roher Bytes verwendet, da diese im Terminal direkt gelesen werden können.

[bild-01]: ./mermaid/img/kommando-01.svg

## Geräteübergreifende Infos und Status-Informationen

### Power-Devices

Diese Device-Konstanten werden für das Parsen des vom X-Plane kommenden Strings benötigt.

| Device-Konstanten<br/> `char *` | Beschreibung                                               |
| ------------------------------- | ---------------------------------------------------------- |
| `BATT_POWER[] = "PB"`           | Battery-Power                                              |
| `AVIONICS_1_POWER[] = "PA1"`    | Avionics-Bus-1-Power                                       |
| `AVIONICS_2_POWER[] = "PA2"`    | Avionics-Bus-2-Power                                       |
| `M803_POWER[] = "PM"`           | Uhr M803-Power (d.h. die Uhr ist eingeschaltet)            |
| `XPDR_POWER[] = "PX"`           | Transponder-Power (d.h. der Transponder ist eingeschaltet) |
| `COM_1_POWER[] = "PC1"`         |                                                            |
| `COM_2_POWER[] =  "PC2"`        |                                                            |
|                                 |                                                            |



### Power-Events

| Const <br/>`char *` | Beschreibung  | Parameter&nbsp;1<br/>Typ | Parameter&nbsp;2<br/>Typ | Parameter-Beschreibung |
| ------------------- | ------------- | ------------------------ | ------------------------ | ---------------------- |
| `ON[] = "ON"`       | eingeschaltet | -                        | -                        | Power ist vorhanden    |
| `OFF[] = "OFF"`     | Ausgeschaltet | -                        | -                        | Keine Power vorhanden  |



## Transponder KT 76C und Uhr Davtron M803

Siehe auch @ref commands.hpp für die aktuelle Version der Codes.
@todo noch weiter ergänzen und updaten!!

### Devices

Ein Device definiert das "Gerät", von dem die nachfolgende Action ausgeführt werden soll (auf dem Arduino) bzw. von dem die Action stammt (vom Arduino für den PC).

| Device | Const<br/>`char *`        | Beschreibung                                    |
| ------ | ------------------------- | ----------------------------------------------- |
| XPDR   | `DEVICE_XPDR[] = "XPDR "` | Action betrifft Transponder KT76C               |
| M803   | `DEVICE_M803[] = "M803"`  | Action betrifft Uhr Davtron M803                |
|        |                           |                                                 |
| D      | DEVICE_DATA = "D "        | Daten, z.B. die am Arduino eingestellte Uhrzeit |



### Events für alle Geräte

| Event-Konstanten<br/>`char *` | Beschreibung                              | Parameter&nbsp;1<br/>Typ |
| ----------------------------- | ----------------------------------------- | ------------------------ |
| `SWITCH_ON[] = "ON"`          | (Schalter) wurde eingeschaltet            | Row<br/>uint8_t          |
| `SWITCH_LON[] = "LON"`        | (Schalter) wurde lange eingeschaltet      | Row<br/>uint8_t          |
| `SWITCH_OFF[] = "OFF"`        | (Schalter) wurde ausgeschaltet            | Row<br/>uint8_t          |
| `POWER[] = "POWER"`           | Powerstatus (nur) für das jeweilige Gerät | Status<br/>String        |



### Events nur für Transponder

Event-Konstanten<br/>`char *`               | Beschreibung                                               | Parameter&nbsp;1<br/>Typ | Parameter&nbsp;2<br/>Typ | Parameter-Beschreibung
--------------------|------------------------------------------------------------|--------------------|---|-----------------------
 `CODE[] = "CODE"` | XPDR-Code anzeigen                         | Transponder-Code<br/>String        | - | 4-stellig
 `FL[] = "F" ` | Flightlevel für Transponder                                | Flightlevel<br/>String | - | 3-stellig



### Events nur für Uhr M803

| Event-Konstanten<br/>`char *` | Beschreibung                                                 | Parameter&nbsp;1<br/>Typ | Parameter&nbsp;2<br/>Typ | Parameter-Beschreibung |
| ----------------------------- | ------------------------------------------------------------ | ------------------------ | ------------------------ | ---------------------- |
| `M803_TIME[] = "TIME"`        | Aktuelle Uhrzeit (Local) und UTC                             | Uhrzeit local<br/>String | Uhrzeit UTC<br/>String   | Uhrzeit HHMMSS         |
| `M803_ET[] = "ET"`            | Elapsed Time                                                 | ?                        |                          | Vergangene Zeit HHMMSS |
| `M803_FT[] = "FT"`            | Flight Time                                                  | ?                        |                          | Flight Time in HHMMSS  |
| `M803_VOLTS[] = "V"`          | Spannung in Volt - eigentlich EMF, aber hat der Flusi nicht? | ?                        |                          | Spannung in Volt       |
| `M803_QNH[] = "Q"`            | Aktuelles QNH des X-Plane-Wetters                            | ?                        |                          | 4-stellig<br/>nnnn     |
| `M803_ALT[] = "A"`            | Aktuelles Altimeter-Setting in inHg                          | ?                        |                          | 4-stellig<br>nn.nn     |
| `M803_OATC[] = "C"`           | OAT in °C                                                    | ?                        |                          | OAT in Celsius         |
| `M803_OATF[] = "F"`           | OAT in Fahrenheit                                            | ?                        |                          | OAT in Fahrenheit      |



## @todo Steuerkommandos für den Arduino

| const-Name      | Event  | Beschreibung                                               | Parameter-Typ | Parameter-Beschreibung |
| --------------- | ------ | ---------------------------------------------------------- | ------------- | ---------------------- |
| ACK             | 0xFFFF | Acknowledge - Angeforderte Daten für Parameter Code folgen |               |                        |
| RESET_ARDUINO   | 0xFF01 | Arduino neu booten                                         | -             |                        |
| RESEND_SWITCHES | 0xFF02 | Den Status aller Schalter senden                           | -             |                        |

Für die Entwicklungs- und Testphase werde Buchstaben statt roher Bytes verwendet, da diese im Terminal direkt gelesen werden können.

## @todo-Plane-Datarefs

Event            | X-Plane-Dataref                                                                                 | X-Plane-Typ | r/w
------------------|-------------------------------------------------------------------------------------------------|-------------|----
M803_LT           | sim/cockpit2/clock_timer/local_time_hours, .../local_time_minutes, .../local_time_seconds       | int         |  r
M803_UT           | sim/cockpit2/clock_timer/zulu_time_hours, .../zulu_time_minutes, .../zulu_time_seconds          | int         |  r
M803_ET           | sim/cockpit2/clock_timer/elapsed_time_hours, .../elapsed_time_minutes, .../elapsed_time_seconds | int         |  r
M803_FT           | sim/cockpit2/clock_timer/timer_elapsed_time_sec (=total time elapsed in seconds)                | float       | r/w
M803_VOLTS        | sim/cockpit2/electrical/battery_voltage_actual_volts                                            | float[8]    |  r
M803_QNH          | = M803_ALT * 33.8637526||
M803_ALT          | sim/weather/barometer_current_inhg                                                              | float       | r/w
M803_OATC         | sim/cockpit2/temperature/outside_air_temp_degc, .../outside_air_temp_is_metric (int 1=C, 0=F)   | float       |  r
M803_OATF         | sim/cockpit2/temperature/outside_air_temp_degf, .../outside_air_temp_is_metric (int 1=C, 0=F)   | float       |  r



## Vom Arduino zum PC gesendete Actions
@todo noch weiter ergänzen und updaten!!

const-Name       | Event  | Beschreibung                          | Parameter-Typ            | Parameter-Beschreibung
-----------------|--------|---------------------------------------|--------------------------|-----------------------
SWITCH_ON        | 0x1101 | Schalter/Taster eingeschaltet         | uint8_t row, uint8_t col | Row und Col in der Schaltermatrix
SWITCH_LON       | 0x1102 | Schalter/Taster lange eingeschaltet   | uint8_t row, uint8_t col | Row und Col in der Schaltermatrix
SWITCH_OFF       | 0x1103 | Schalter/Taster ausgeschaltet         | uint8_t row, uint8_t col | Row und Col in der Schaltermatrix
REQUEST_DATA     | 0x1F01 | Daten vom PC anfordern                | uint16_t Arduino-Action  | -
