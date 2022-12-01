# Davtron M803 {#DavtronM803}

## Bedienelemente

Die Frontseite ![Frontplatte][M803Front]

Die Bedienelemente ![Bedienelemente][M803Elemente]

|Element             |Bauteil                       |Funktion                                                                             |
|--------------------|------------------------------|-------------------------------------------------------------------------------------|
|(1) *SELECT*-Knopf  | Taster                       |Umschalten der anzuzeigenden Zeiten (LT, UT, ET, FT) sowie Ein-/Ausschalten des Testmodus. Außerdem verwendet zum Einstellen der Zeit. |
|(2) *CONTROL*-Knopf |Taster                        |Stopp, Reset und Start der Elapsed Time, Reset der Flight Time, Einstellen der Zeit. |
|(3) Stunden         |Zwei 7-Segment-Anzeigen, rot  |Anzeige der Stunden                                                                  |
|(4) Minuten         |Zwei 7-Segment-Anzeigen, rot  |Anzeige der Minuten                                                                  |
|(5) Stunden-Minuten-Trenner |Zwei LEDs, 3mm, rot |Optische Trennung der Stunden von den Minuten.                                         |
|(6) O.A.T. / Volt   |Vier 7-Segment-Anzeigen, rot  |Anzeige der EMF Voltage, der Temperatur und des QNH.                                 |
|(7) Umschalter O.A.T.-Voltmeter |Taster |Umschalten der Anzeige von EMF Voltage, O.A.T. (Outside Air Temperature) in Fahrenheit und Celsius sowie des QNH. |
|(8) LT              |LED, 3mm, rot                 |Indikator, dass die Local Time in den Anzeigen (3) und (4) angezeigt wird.           |
|(9) UT              |LED, 3mm, rot                 |Indikator, dass die Universal Time in den Anzeigen (3) und (4) angezeigt wird.       |
|(10) ET             |LED, 3mm, rot                 |Indikator, dass die Elapsed Time in den Anzeigen (3) und (4) angezeigt wird.         |
|(11) FT             |LED, 3mm, rot                 |Indikator, dass die Flight Time in den Anzeigen (3) und (4) angezeigt wird.          |

## Funktion und Bedienung

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

Zum Stellen der *Uhrzeit* die Anzeige der UT mit Hilfe des *SELECT*-Knopfes (1) auswählen. Dann gleichzeitig den *SELECT*- und *CONTROL*-Knopf drücken um in den Stellmodus zu wechseln. Die Zehner der Stunden fangen an zu blinken und die Leuchtdioden (5) erlöschen. Der *CONTROL*-Knopf (2) steuert nun die blinkende Ziffer. Jedes Drücken erhöht die Ziffer um eins. Wenn die Zehner gestellt sind, aktiviert der *SELECT*-Knopf (1) jeweils die nächste Ziffer zum Stellen. Nachdem die letzte Ziffer ausgewählt und eingestellt wurde, bewirkt ein letzter Druck auf den *SELECT*-Knopf die Rückkehr zum normalen Betrieb. Die Leuchtdioden (5) blinken wieder wie oben beschrieben.

Das Einstellen der Local Time erfolgt analog bei über den *SELECT*-Knopf ausgewählter Local Time, *LT*. Durch gleichzeitiges Drücken der Tasten *SELECT* und *CONTROL* wird in den SET-Modus ge-schaltet und die Stundenzehner blinken. Die Stunden werden nun wie oben beschrieben mittels der *CONTROL*- und *SELECT*-Tasten verstellt. Die Minuten sind schon synchronisiert und können im Local Time Modus nicht verstellt werden.

Da der Flugsimulator die Universal Time in Abhängigkeit von der Local Time (und umgekehrt) berechnet, macht nur das Verstellen entweder der Local Time oder der Universal Time Sinn, da beide nicht unabhängig voneinander im Flugsimulator einstellbar sind.
Wenn die Zeit gemäß obiger Beschreibung verstellt wird und in der Ini-Datei der Parameter Transfer-TimeToFS aktiviert ist, wird die eingestellte Zeit zum Flugsimulator übertragen. Ist der Parameter *UseSystemTimeAsLT* aktiviert, ist ein Verstellen der Local Time nicht möglich (siehe oben).

### Elapsed Time

#### Hochzählen

Zur Anzeige der Elapsed Time (ET) so oft den *SELECT*-Knopf (1) drücken, bis die Leuchtdiode *ET* leuchtet. Ein Druck auf den *CONTROL*-Knopf (2) stellt die ET auf 00:00 und startet die Zählung der ET. Die Elapsed Time zählt bis 59 Minuten, 59 Sekunden und schaltet dann auf die Anzeige von Stunden und Minuten um. Die weitere Zählung geht dann bis zu 99 Stunden und 59 Minuten. Ein weiterer Druck des *CONTROL*-Knopfes stoppt die Zeitzählung.

#### Runterzählen

Nach Auswahl der Anzeige der ET wird durch gleichzeitiges Drücken beider Knöpfe der Stellmodus eingeschaltet. Ein Countdown von maximal 59 Minuten und 59 Sekunden ist möglich. Die gewünschte Zeit wird analog zur UT eingestellt. Nach dem Einstellen der Zeit startet ein Druck auf den *CONTROL*-Knopf den Countdown. Der Alarm wird bei 00:00 aktiviert, die Anzeige blinkt. Ein Druck auf einen der zwei Knöpfe stellt den Alarm ab. Bei Erreichen von 00:00 zählt die ET weiter aufwärts.

### Flight Time

#### Flight Time Start, Stop, Reset

Beim Einschalten des Geräts beginnt die "Flight Time" (intern) von Null ab aufwärts zu zählen. Zum Zurücksetzen muss die Flight Time angezeigt werden. Das Zurücksetzen auf Null erfolgt durch ein drei Sekunden langes Drücken des *CONTROL*-Knopfs (2) bis in der Anzeige 99:59 erscheint. Beim Loslassen des Kopfs wird die Flight Time dann auf Null gesetzt und beginnt wieder zu laufen.

#### Alarm einstellen

When Flight Time, FT, is displayed enter the set mode by pressing both buttons simultaneously. The alarm time is entered identically to UT setting. When the Flight Time equals the alarm time the dis-play will flash and the alarm output activated. If FT was not being displayed at the time the alarm becomes active, the clock automatically *select*s FT for display. Pressing either the SEL or CTL but-tons turns off the alarm. Flight Time is unchanged and continues counting.

### </a>Umschalten zwischen der O.A.T.- und Spannungsanzeige

Im Gegensatz zum echten M803 werden in der oberen Anzeige (6) neben *EMF Voltage*, *O.A.T. in Fahrenheit* und *O.A.T. in Celsius* noch zusätzlich der aktuelle *QNH in hPa* sowie der aktuelle *QNH in inches hg* angezeigt.
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

Nach Drücken von SELECT für 3 Sekunden zeigen beide Displays (3) und (4) jeweils 88 sowie das Display (6)   88.88 an. Außerdem leuchten alle 6 LEDs (LT, UT, ET, FT und die Trenner-LEDs (5)). Bei Loslassen des Knopfs SELECT wird zur vorherigen Anzeigesituation zurückgekehrt.

[M803Front]: file:///Users/harraeusc/Documents/Projekte/FSHWPanel/Arduino-POC/XPanino/Doku/Bilder/Davtron-M803-Front-klein.jpg

[M803Elemente]: /Users/harraeusc/Documents/Projekte/FSHWPanel/Arduino-POC/XPanino/Doku/Bilder/Davtron-M803-mitNr.jpg




