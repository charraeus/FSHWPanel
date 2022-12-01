---
title:  'XPanino'
subtitle: "Uhr Davtron M803 und Transponder Bendix-King KT 76C"
subject: Beschreibung der Bedienung und des Aufbaus einer Uhr und eines
         Transponders für X-Plane
author: Christian Harraeus
date: 16. Oktober 2020

description: |
    This is a long
    description.

    It consists of two paragraphs
lang: de-DE
documentclass: scrartcl
pagestyle: headings
...

# Uhr Davtron M803 {#M803}

## Bedienelemente Davtron M803 {#M803_Bedienelemente}

Die Frontseite ![Frontplatte][M803Front]

Die Bedienelemente ![Bedienelemente][M803Elemente]

|Element             |Bauteil                       |Funktion                                                                             |
|--------------------|------------------------------|-------------------------------------------------------------------------------------|
|(1) *SELECT*-Knopf  | Taster                       |Umschalten der anzuzeigenden Zeiten (LT, UT, ET, FT) sowie Ein-/Ausschalten des Testmodus. Außerdem verwendet zum Einstellen der Zeit. |
|(2) *CONTROL*-Knopf |Taster                        |Stopp, Reset und Start der Elapsed Time, Reset der Flight Time, Einstellen der Zeit. |
|(3) Stunden         |Zwei 7-Segment-Anzeigen, rot  |Anzeige der Stunden.                                                                 |
|(4) Minuten         |Zwei 7-Segment-Anzeigen, rot  |Anzeige der Minuten.                                                                 |
|(5) Stunden-Minuten-Trenner |Zwei LEDs, 3mm, rot |Optische Trennung der Stunden von den Minuten.                                         |
|(6) O.A.T. / Volt   |Vier 7-Segment-Anzeigen, rot  |Anzeige der EMF Voltage, der Temperatur und des QNH.                                 |
|(7) Umschalter O.A.T.-Voltmeter |Taster |Umschalten der Anzeige von EMF Voltage, O.A.T. (Outside Air Temperature) in Fahrenheit und Celsius sowie des QNH. |
|(8) LT              |LED, 3mm, rot                 |Indikator, dass die Local Time in den Anzeigen (3) und (4) angezeigt wird.           |
|(9) UT              |LED, 3mm, rot                 |Indikator, dass die Universal Time in den Anzeigen (3) und (4) angezeigt wird.       |
|(10) ET             |LED, 3mm, rot                 |Indikator, dass die Elapsed Time in den Anzeigen (3) und (4) angezeigt wird.         |
|(11) FT             |LED, 3mm, rot                 |Indikator, dass die Flight Time in den Anzeigen (3) und (4) angezeigt wird.          |

## Funktion und Bedienung {#M803_Bedienung}

### Betrieb

Die jeweils aktuelle Zeit des Flugsimulators, die aktuelle O.A.T. in Celsius sowie der aktuelle QNH wird in regelmäßigen Abständen aus dem Flugsimulator ausgelesen und entsprechend angezeigt. Wenn kein Flugsimulator online ist, wird im oberen Display (6) „*noFS*“ angezeigt. In diesem Fall wird die Systemzeit des PCs auf dem FSHWPanel läuft, verwendet, um eine Zeitanzeige auch ohne Flugsimulator zu ermöglichen.

Ist in der Ini-Datei der Parameter *UseSystemTimeAsLT* auf *TRUE* gesetzt, wird die aktuelle Systemzeit des PCs auf dem FSHWPanel läuft, als Local Time verwendet und angezeigt. Als Universal Time hingegen wird die aktuelle Zeit des Flugsimulators verwendet. Die dient zur leichteren Abwicklung von Online-Flügen (z.B. der IVAO-VFR-Tour), bei denen die reale Zeit in den PIREP angegeben werden muss. Ein Verstellen der Local Time (siehe unten) ist dann nicht möglich.

Die Anzeige der Uhr (Displays und LEDs) ist nur eingeschaltet, wenn der Batteriehauptschalter ebenfalls eingeschaltet ist. Wenn kein Strom da ist (also der Batteriehauptschalter ausgeschaltet ist), haben die Tasten keine Wirkung. Wird der Batteriehauptschalter ausgeschaltet, wird die Anzeige wieder dunkel geschaltet.

Anfangszustand bei jedem Einschalten: Anzeige von O.A.T. in Celsius und Local Time. Die Flight Time beginnt bei 00:00 an aufwärts zu zählen. Die Elapsed Time wird auf 00:00 gesetzt, zählt aber nicht hoch. Beim Ausschalten (Batterieschalter aus) gehen die Werte von Elapsed Time und Flight Time verloren.

Die Stellung des Avionics-Schalters ist nicht relevant.

Die zwei Leuchtdioden (5) zwischen den unteren Anzeigen (3) und (4) blinken bei Anzeige der Local Time, Universal Time oder Flight Time im Halbsekundenrythmus (500 Millisekunden an, 500 Millisekunden aus). Bei Anzeige der ET leuchten die Leuchtdioden konstant wenn die ET läuft. Läuft die ET nicht, sind die Leuchtdioden erloschen.

Mit dem *SELECT*-Knopf (1) wird die anzuzeigende Zeitvariante ausgewählt. Durch mehrfaches Drücken des *SELECT*-Knopfs wird – ausgehend von der nach dem Einschalten aktiven Anzeige der Local Time – nacheinander die Anzeige der Universal Time, der Flight Time, der Elapsed Time und wieder der Local Time ausgewählt. Die jeweils angezeigte Zeitvariante wird über die entsprechende LED (8, 9, 10, 11) signalisiert.

Der *CONTROL*-Knopf (2) setzt bei Anzeige der Flight Time diese auf Null zurück und startet sie neu, wenn er drei Sekunden lang gedrückt wird. Durch kurzes Drücken bei Anzeige der Elapsed Time wird diese gestoppt bzw. auf Null zurückgesetzt und neu gestartet.
Außerdem dienen der *SELECT*- und *CONTROL*-Knopf zum Einstellen der Local bzw. Universal Time. Näheres siehe unten.

Der O.A.T. / Volts-Knopf (7) dient zum Umschalten der Anzeige (6). Siehe unten.

### Universal Time bzw. Local Time einstellen

Zum Stellen der *Uhrzeit* die Anzeige der UT mit Hilfe des *SELECT*-Knopfes (1) auswählen. Dann gleichzeitig den *SELECT*- und *CONTROL*-Knopf drücken, um in den Stellmodus zu wechseln. Die Zehner der Stunden fangen an zu blinken und die Leuchtdioden (5) erlöschen. Der *CONTROL*-Knopf (2) steuert nun die blinkende Ziffer. Jedes Drücken von *CONTROL* erhöht die Ziffer um eins. Wenn die Zehner gestellt sind, aktiviert der *SELECT*-Knopf (1) die jeweils nächste Ziffer zum Stellen. Nachdem die letzte Ziffer ausgewählt und eingestellt wurde, bewirkt ein letzter Druck auf den *SELECT*-Knopf die Rückkehr zum normalen Betrieb. Die Leuchtdioden (5) blinken wieder wie oben beschrieben.

Das Einstellen der Local Time erfolgt analog bei über den *SELECT*-Knopf ausgewählter Local Time, *LT*. Durch gleichzeitiges Drücken der Tasten *SELECT* und *CONTROL* wird in den SET-Modus geschaltet und die Stundenzehner blinken. Die Stunden werden nun wie oben beschrieben mittels der *CONTROL*- und *SELECT*-Tasten verstellt. Die Minuten sind schon synchronisiert und können im Local Time Modus nicht verstellt werden.

Da der Flugsimulator die Universal Time in Abhängigkeit von der Local Time (und umgekehrt) berechnet, macht nur das Verstellen entweder der Local Time oder der Universal Time Sinn, da beide nicht unabhängig voneinander im Flugsimulator einstellbar sind.
Wenn die Zeit gemäß obiger Beschreibung verstellt wird und in der Ini-Datei der Parameter *Transfer-TimeToFS* aktiviert ist, wird die eingestellte Zeit zum Flugsimulator übertragen. Ist der Parameter *UseSystemTimeAsLT* aktiviert, ist ein Verstellen der Local Time nicht möglich (siehe oben).

### Elapsed Time

#### Hochzählen

Zur Anzeige der Elapsed Time (ET) so oft den *SELECT*-Knopf (1) drücken, bis die Leuchtdiode *ET* leuchtet. Ein Druck auf den *CONTROL*-Knopf (2) stellt die ET auf 00:00 und startet die Zählung der ET. Die Elapsed Time zählt bis 59 Minuten, 59 Sekunden und schaltet dann auf die Anzeige von Stunden und Minuten um. Die weitere Zählung geht dann bis zu 99 Stunden und 59 Minuten. Ein weiterer Druck des *CONTROL*-Knopfes stoppt die Zeitzählung.

#### Herunterzählen

Nach Auswahl der Anzeige der ET wird durch gleichzeitiges Drücken beider Knöpfe der Stellmodus eingeschaltet. Ein Countdown von maximal 59 Minuten und 59 Sekunden ist möglich. Die gewünschte Zeit wird analog zur UT eingestellt. Nach dem Einstellen der Zeit startet ein Druck auf den *CONTROL*-Knopf den Countdown. Der Alarm wird bei 00:00 aktiviert, die Anzeige blinkt. Ein Druck auf einen der zwei Knöpfe stellt den Alarm ab. Bei Erreichen von 00:00 zählt die ET weiter aufwärts.

### Flight Time

#### Flight Time Start, Stop, Reset

Beim Einschalten des Geräts beginnt die "Flight Time" (intern) von Null ab aufwärts zu zählen. Zum Zurücksetzen muss die Flight Time angezeigt werden. Das Zurücksetzen auf Null erfolgt durch ein drei Sekunden langes Drücken des *CONTROL*-Knopfs (2) bis in der Anzeige 99:59 erscheint. Beim Loslassen des Kopfs wird die Flight Time dann auf Null gesetzt und beginnt wieder zu laufen.

#### Alarm einstellen

When Flight Time, FT, is displayed enter the set mode by pressing both buttons simultaneously. The alarm time is entered identically to UT setting. When the Flight Time equals the alarm time the display will flash and the alarm output activated. If FT was not being displayed at the time the alarm becomes active, the clock automatically *select*s FT for display. Pressing either the SEL or CTL but-tons turns off the alarm. Flight Time is unchanged and continues counting.

### Umschalten zwischen der O.A.T.- und Spannungsanzeige

Im Gegensatz zum echten M803 werden in der oberen Anzeige (6) neben *EMF Voltage*, *O.A.T. in Fahrenheit* und *O.A.T. in Celsius* noch zusätzlich der aktuelle, d.h. im Flugsimulator eingestellte, *QNH in hPa* sowie der aktuelle *QNH in inches hg* angezeigt.
Durch mehrfaches Drücken des Knopfs (7) wird die Anzeige (6) sequentiell zwischen EMF Voltage, O.A.T. in Fahrenheit, O.A.T. in Celsius, QNH in hPa und QNH in inches hg umgeschaltet. Beim Einschalten wird die O.A.T. in Celsius angezeigt.

Die Anzeige der einzelnen Werte erfolgt im nachfolgend beschriebenen Format:

* EMF-Voltage: 99.9E
* O.A.T. Fahrenheit
  * wenn > 99F oder < 0F: ±99F
  * sonst: 99.9F
* O.A.T. Celsius:
  * wenn >= 0°C: 99°C
  * wenn < 0°C: -99C
* QNH in hPa: 9999
* QNH in inches hg: 99.99

> **Anmerkung**:  
> Im Flugsimulator steht derzeit die EMF Voltage nicht zur Verfügung. Im echten M803 wird nach dem Einschalten die EMF Voltage angezeigt. Dies wird in der Software geändert, sobald die EMF Voltage aus dem Flugsimulator ausgelesen werden kann. Momentan wird beim Einschalten die O.A.T. in Celsius angezeigt.

@todo Checken und Doku anpassen: stimmt das noch?

### Testmodus

Nach Drücken von SELECT für 3 Sekunden zeigen beide Displays (3) und (4) jeweils 88 sowie das Display (6) 88.88 an. Außerdem leuchten alle 6 LEDs (LT, UT, ET, FT und die Trenner-LEDs (5)). Bei Loslassen des Knopfs SELECT wird zur vorherigen Anzeigesituation zurückgekehrt.

## Schnittstelle zwischen X-Plane und Arduino Uno/Micro {#M803_IF}

### Kommunikation X-Plane zum Arduino Uno/Micro

**Source**: Ist immer "*XP*" für X-Plane.

| Event | Device | Device-Code | Parameter 1               | Parameter 2 | Bemerkung                           |
| ----- | ------ | ----------- | ------------------------- | ----------- | ----------------------------------- |
|       | FL     |             | Flightlevel 3-stellig     | -           |                                     |
|       | LT     |             | Local Time im Format HHMM | -           |                                     |
|       | OAT    |             | OAT in °C                 | -           | Details zur Darstellung siehe oben. |
|       | VOLT   |             | Volt-Zahl                 | -           | Details zur Darstellung siehe oben. |
|       | QNH    |             | QNH in inches hg          | -           | Details zur Darstellung siehe oben. |

### Kommunikation Arduino Uno/Micro zu X-Plane

\pagebreak

# Transponder Bendix-King KT 76C {#KT76C}

## Bedienelemente {#KT76C_Bedienelemente}

Die Frontseite ![Frontplatte][KT76CFront]

Die Bedienelemente ![Bedienelemente][KT76CElemente]

| Element                              | Bauteil                               | Funktion                                                     |
| ------------------------------------ | ------------------------------------- | ------------------------------------------------------------ |
| \(1) *Ident*-Taste                   | Taster T250A                          | Senden von "Squawk-Ident".                                   |
| \(2) *Code Entry-*Tasten *0* bis *7* | 8 x Taster T250A                      | Einstellen der 4 Stellen des Transponder-Codes.              |
| \(3) *CLR*-Taste                     | Taster T250A                          | Korrektur des letzten eigegebenen Digits des Squawk Codes.   |
| \(4) *VFR*-Taste                     | Taster T250A                          | Abruf und Setzen des programmierten VFR-Squawk-Codes (i.d.R. "7000"). |
| \(5) *Betriebsmodus-Wahlschalter*    | Drehschalter DS1, 1 Pol, 5 Stellungen | Einstellen des Betriebsmodus:<br />* Stellung *OFF*: Gerät ist ausgeschaltet.<br />* Stellung *SBY* (Standby): Gerät in Standby. Gerät ist an, aber reagiert nicht auf Anfragen.<br />* Stellung *TST*: Keine Reaktion auf Anfragen, alle Segmente und LEDs leuchten für mindestens 4 Sekunden.<br />* Stellung *ON*: Modus A, C und S-Anfragen werden beantwortet.<br />* Stellung *ALT* (Altitude): Modus A, C und S-Anfragen werden beantwortet. Die LED R blinkt langsam. |
| \(6) *FL-Anzeigefenster*             | 3 x 7-Segment-Anzeige SA52            | Anzeige des aktuellen Flight Levels.                         |
| \(7) *Code-Anzeigefenster*           | 4 x 7-Segment-Anzeige SA52            | Anzeige des eingestellten Squawk Codes.                      |
| \(8) *ALT-Modus-Anzeige*             | LED, rot, 5mm                         | Anzeige des Betriebsmodus *ALT*. Diese LED leuchtet, wenn der aktuell eingestellte Betriebsmodus *ALT* ist. |
| \(9) *Reply-Indikator*               | LED, rot, 5mm                         | Die LED blinkt langsam, wenn das Gerät auf eine Abfrage antwortet und leuchtet für 18 Sekunden nach Betätigung der *IDT*-Taste (1). |


## Funktion und Bedienung {#KT76C_Bedienung}

### Betrieb

Die Anzeigen und Tasten sind nur aktiv, wenn sowohl die Batterie als auch der Avionics-Bus eingeschaltet sind. Ansonsten bleiben die Anzeigen dunkel und es erfolgt keine Abfrage der Tasten. Datenübertragung zwischen X-Plane und Transponder findet nicht statt.

Sobald der Transponder Strom hat und der *Betriebsmodus-Wahlschalter* (5) auf *SBY*, *ON* oder *ALT* gestellt wird/ist, werden der aktuelle Flightlevel sowie der aktuelle Transpondercode des Flugsimulators in den Anzeigefenstern (6) und (7) angezeigt. Steht der *Betriebsmodus-Wahlschalter* (5) auf *TST* wenn der Transponder Strom bekommt, so verhält sich der Transponder wie im Modus *SBY* (siehe unten).

Steht der *Betriebsmodus-Wahlschalter* (5) auf *ON* oder *ALT* wird der jeweils aktuelle Flightlevel sowie Transponder-Code in regelmäßigen Abständen aus dem Flugsimulator ausgelesen und entsprechend angezeigt. Wenn kein Flugsimulator online ist, wird im rechten Display (7) „*noFS*" angezeigt. Das linke Display (6) bleibt dann dunkel und die Tasten werden nicht abgefragt.

### Transponder-Code einstellen

Der Transponder Identifikations-Code (Squawk Code) für das Flugzeug wird im Ident-Anzeigefenster (3) angezeigt. Der Code wird durch squentielles Drücken der *Code Entry-*Tasten *0* bis *7* eingestellt. Wurde eine falsche Taste gedrückt, kann der letzte Tastendruck durch Druck auf den *CLR*-Knopf (3) gelöscht und neu eingegeben werden. Der eingegebene Code wird im *Code-Anzeigefenster* angezeigt. Der zuletzt aktive Code wird wieder angezeigt, wenn ein unvollständiger Code eingegeben wurde und innerhalb von vier Sekunden kein Code Entry-Knopf *0* .. *7* (2), kein *VFR*-Knopf (4) und kein *CLR*-Knopf (3) betätigt wird.

Ein kurzer Druck auf den *VFR*-Knopf (4) stellt den voreingestellten VFR-Code *7000* ein. Das Drücken des *VFR*-Knopfes für zwei Sekunden stellt den zuletzt aktiven VFR-Code wieder ein.

### Squawk-Ident senden

Die Ident-Taste *IDT* ist zu drücken, wenn ATC ein „Squawk Ident“ anfordert. Nach Drücken der Taste leuchtet der Reply Indicator (4) ca. 18 Sekunden lang als Bestätigung.

### Anzeige des aktuellen Flightlevels

Der aktuelle Flight Level wird im *FL-Anzeigefenster* (9) angzeigt. Die Anzeige erfolgt in Hundertfuß, z.B. ist FL 071 gleichbedeutend mit 7100 Fuß. Die Anzeige erfolgt nur wenn als Betriebsmodus *SBY*, *ON* oder *ALT* eingestellt ist (siehe Testmodus unten). Der anzuzeigende Bereich geht von -10 bis 999.  
Der Flightlevel ist vertikale Distanz des Flugzeugs über der Isobare von 1013,25 hPa (oder 29.92 in Hg). Steht seitens des Flugsimulators nur die Altitude zur Verfügung, wird der Flightlevel aus der Altitude und dem aktuell eingestelltem Luftdruck (QNH) am Höhenmesser des Captains berechnet.

### Betriebsmodus einstellen

Die Betriebsmodi werden über den *Betriebsmodus-Wahlschalter* (1) ausgewählt.

* In Stellung *OFF* sind alle Anzeigen und LEDs ausgeschaltet. Änderungen des Ident-Codes werden nicht zum Flugsimulator übertragen.
* In Stellung *SBY* (Standby) zeigt das *FL-Anzeigefenster* den aktuellen Flightlevel und das Ident-Fenster den zuletzt eingestellten Ident-Code an. Beide Werte werden vom Flugsimulator geliefert.  
Änderungen des ID-Codes werden zum Flugsimulator übertragen.
* Beim Einschalten der Stellung *TST* werden alle Segmente aller Displays sowie alle LEDs für mindestens 4 Sekunden eingeschaltet. Nach Ablauf der 4 Sekunden wechselt der Betriebsmodus wieder auf *SBY*.  
Der Testmodus kann über den *Betriebsmodus-Wahlschalter* vor Ablauf der 4 Sekunden beendet werden indem der *Betriebsmodus-Wahlschalter* auf *SBY* oder *ON* gestellt wird. Solange der der Schalter in Stellung *TST* steht könne Änderungen des Transponder-Codes nicht erfolgen und werden nauch icht zum Flugsimulator übertragen. 
* In Stellung *ON* zeigt das Altitude-Anzeigefenster zeigt den aktuellen Flight Level an und Änderungen des Ident-Codes werden zum Flugsimulator übertragen. 
* In Stellung *ALT* zeigt das *FL-Anzeigefenster* den aktuellen Flight Level an. Änderungen des Ident-Codes werden zum Flugsimulator übertragen und die LED *R* blinkt um die Transponder-Replys zu simulieren.

### Ändern des voreingestellten VFR-Codes

Die Voreinstellung des VFR-Codes kann folgendermaßen geändert werden:

1. Mit Hilfe des *Betriebsmodus-Wahlschalters* (1) den Betriebsmouds *SBY* (*Standby*) einstellen.
1. Den gewünschten VFR-Code mit den *Code Entry-*Knöpfen (2) einstellen.
1. Drücken und Festhalten des *IDT*-Knopfs (1) und Druck auf den *VFR*-Knopf (4).

## Schnittstelle zwischen X-Plane und Arduino Uno/Micro {#KT76C_IF}

### Kommunikation X-Plane zum Arduino Uno/Micro

### Kommunikation Arduino Uno/Micro zu X-Plane

\pagebreak

# Hardware für die Uhr und den Transponder {#Hardware_M803_KT76C}

## Anordnung der Schalter für Uhr und Transponder in der Schaltermatrix {#M803_KT76C_Schaltermatrix}



|              |**Col 0** |**Col 1** |**Col 2** |**Col 3** |**Col 4** |**Col 5** |**Col 6** |**Col 7** |
|--------------|----------|----------|----------|----------|----------|----------|----------|----------|
|**Row 0**     |0.0<br />XPDR<br />*IDT*       |0.1<br />XPDR<br />*VFR*       |0.2<br />XPDR<br />*CLR*       |0.3<br />XPDR<br />*OFF*       |0.4<br />XPDR<br />*SBY*       |0.5<br />XPDR<br />*TST*       |0.6<br />XPDR<br />*ON*       |0.7<br />XPDR<br />*ALT*       |
|**Row 1**     |1.0<br />XPDR<br />*0*       |1.1<br />XPDR<br />*1*       |1.2<br />XPDR<br />*2*       |1.3<br />XPDR<br />*3*       |1.4<br />XPDR<br />*4*       |1.5<br />XPDR<br />*5*       |1.6<br />XPDR<br />*6*       |1.7<br />XPDR<br />*7*       |
|**Row 2**     |2.0<br />Uhr<br />*SELECT*       |2.1<br />Uhr<br />*CONTROL*       |2.2<br />Uhr<br />*O.A.T*       |2.3       |2.4       |2.5       |2.6       |2.7       |

**ACHTUNG:** Die Col-Nummer muss noch auf die Gegebenheiten des Anschlusses an den Arduino anstatt an den IOW angepasst werden!

## Verdrahtung Arduino Uno/Micro <--> Schalter der Uhr {#M803_Schalterverdrahtung}

|Arduino-Uno |Arduino-Micro | Stecker-PIN | IOW-Bez.    | Draht-Farbe|
|------------|--------------|-------------|-------------|------------|
|Pin A2      |Pin           |  Pin  1     | IOW2-ROWS6  | sw         | umlöten!! und hier korrigieren
|Pin A3      |Pin           |  Pin  2     | IOW2-ROWS4  | ws         | umlöten!! und hier korrigieren
|Pin 12      |Pin           |  Pin 10     | IOW2-COL7   | rt         | umlöten!! und hier korrigieren
|Pin 13      |Pin           |  Pin  9     | IOW2-COL6   | br         | umlöten!! und hier korrigieren

Table: Verdrahtung der Uhren-Schalter

Die Spalte Arduino-Uno stellt eine alternative Verdrahtungsmöglichkeit bei Nutzung eines Arduino-Uno dar.

## Verdrahtung Arduino Uno/Micro <--> Schalter des Transponders {#KT76C_Schalterverdrahtung}

|Arduino-Uno |Arduino-Micro | Stecker-PIN | IOW-Bez.    | Draht-Farbe|
|------------|--------------|-------------|-------------|------------|

Table: Verdrahtung der Transponderschalter

## LEDs und 7-Segmentanzeigen für Uhr und Transponder {#M803_KT76C_LED}

### Anordnung der LEDs für Uhr und Transponder in der LED-Matrix {#M803_KT76C_LEDMatrix}

|              |**Row 0**   |**Row 1**   |**Row 2** |**Row 3** |**Row 4** |**Row 5** |**Row 6** |**Row 7** |
|:--------------|:---------------|:---------------|:---------------|:---------------|:---------------|:---------------|:---------------|:---------------|
|Byte 0:<br />**Col 4**<br />LED rot, R = 270 Ω       |0.4         |1.4         |2.4       |3.4       |4.4       |5.4       |6.4       |7.4       |
|     |Uhr         |Uhr         |Uhr       |Uhr       |Uhr       |Uhr       |XPDR      |XPDR      |
|       |*Std-/Min-<br />Trenner-1* |*Std-/Min-*<br />Trenner-2 |*LT*      |*UT*      |*ET*      |*FT*      |*ALT*     |*R*       |

Table: Einzelne LEDs --> @todo: Diese 8 müssen in das freie Byte der 7-Segment-Anzeigen mit 'rein (Byte 1, Row 7)

**ACHTUNG:** Die Col-Nummer muss noch auf die Gegebenheiten des Anschlusses an den Arduino anstatt an den IOW angepasst werden!

|  |**Row 0** |**Row 1** |**Row 2** |**Row 3** |**Row 4** |**Row 5** |**Row 6** |**Row 7** |
|--------------|----------|----------|----------|----------|----------|----------|----------|---------|
|**Byte 1**:<br />**COL 8** bis **Col 15**<br />7-Segm,<br />R8 - R15: 270&nbsp;Ω       |col.0     |col.1     |col.2     |col.3     |col.4     |col.5     |col.6     | col.7    |
| |XPDR      |XPDR      |XPDR      |XPDR      |XPDR      |XPDR      |XPDR      | frei     |
|    |Segmente  |Segmente  |Segmente  |Segmente  |Segmente  |Segmente  |Segmente  |          |
|       |a - g, DP |a - g, DP |a - g, DP |a - g, DP |a - g, DP |a - g, DP |a - g, DP |          |
| |Digit 0   |Digit 1   |Digit 2   |Digit 3   |Digit 4   |Digit 5   |Digit 6   |          |
|**Byte 2**:<br />**Col 16** bis **Col 23**<br />7-Segm.,<br />R16 - R23: 270&nbsp;Ω       |col.0     |col.1     |col.2     |col.3     |col.4     |col.5     |col.6     |col.7     |
||Uhr       |Uhr       |Uhr       |Uhr       |Uhr       |Uhr       |Uhr       |Uhr       |
|    |Segmente  |Segmente  |Segmente  |Segmente  |Segmente  |Segmente  |Segmente  |Segmente  |
|       |a - g, DP |a - g, DP |a - g, DP |a - g, DP |a - g, DP |a - g, DP |a - g, DP |a - g, DP |
||Digit 0   |Digit 1   |Digit 2   |Digit 3   |Digit 4   |Digit 5   |Digit 6   |Digit 7   |

Table: 7-Segment-Anzeigen

**ACHTUNG:** Die Col-Nummer muss noch auf die Gegebenheiten des Anschlusses an den Arduino anstatt an den IOW angepasst werden!

### Verdrahtung Arduino Uno/Micro <--> FSHWPanel-LED-Modul (LEDs) {#M803_KT76C_LEDVerdrahtung}

Arduino-Uno | Arduino-Micro | Stecker-PIN | IOW-Bez.      | Draht-Farbe
------------|---------------|-------------|---------------|------------
Pin  2      | Pin  x        | Pin  1      | IOW1-PORT-1.0 | sw  
Pin  3      | Pin  x        | Pin  2      | IOW1-PORT-1.1 | ws
Pin  4      | Pin  x        | Pin  3      | IOW1-PORT-1.3 | gr
Pin  5      | Pin  x        | Pin  4      | IOW1-PORT-1.4 | vi
Pin GND     | Pin  x        | Pin 10      | GND           | br

Table: Verdrahtung der LEDs

# Links zu weiteren Bildern


[M803Front]: Bilder/Davtron-M803-Front-klein.jpg
[M803Elemente]: Bilder/Davtron-M803-mitNr.jpg
[KT76CFront]: Bilder/XPDR-KT76C-V2-Front.jpg
[KT76CElemente]: Bilder/XPDR-KT76C-V2-mitNr.jpg
