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

[KT76CFront]: Bilder/XPDR-KT76C-V2-Front.jpg
[KT76CElemente]: Bilder/XPDR-KT76C-V2-mitNr.jpg

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
