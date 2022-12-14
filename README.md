# FSHWPanel
[toc]

## Was ist FSHWPanel?
FSHWPanel ist eine aus mehreren Komponenten bestehende Software, mittels derer Schalter und Anzeigen (LEDs, 7-Segment-Anzeigen) an X-Plane angeschlossen werden können. Damit können dann Infos aus der Avionic – z.B. der Transpondercode oder eine COM-Frequenz – auf einem aus mehreren 7-Segment-Anzeigen bestehenden Modul angezeigt werden. Andersherum löst das Betätigen eines Tasters oder Schalters eine Aktion im X-Plane aus – z.B. das Einschalten des Transponders.

## Verzeichnisse
* **Doku**
  Alle Dateien, die für die Dokumentation aus Entwickler- und Anwendersicht notwendig sind.
* **XPanino**
  Quellcode für den Arduino.
  * **\.vscode**
    Spezifische Konfiguration für Visual Studio Code (VSCode).
  * **XPanino/include**
    Dieses Verzeichnis ist für Header-Dateien `*.h`, `*.hpp` gedacht.
  * **XPanino/lib**
    Dieses Verzeichnis ist für projektspezifische (private) Libraries gedacht. Der Source-Code jeder Library soll in einem eigenen Unterverzeichnis gespeichert werden:
    `lib/meine_library/[hier liegen die Source-Dateien]`
  * **XPanino/src**
    Dieses Verzeichnis ist für alle Source-Dateien gedacht, die nicht Teil von Libraries sind. z.B. `*.c`, `*.cpp`, usw.
* **XPIf**
  Quellcode für die auf dem PC laufende Schnittstelle zu X-Plane. Die Unterverzeichnisse sind hier analog zu XPanino.
  * **XPIf/.vscode**
  * **XPIf/doc/html**
  * **XPIf/lib**
  * **XPIf/src**
* **doc/html**
  Hier landen die durch *Doxygen* generierten html-Dateien für die Doku des Quellcodes. Dieses Verzeichnis ist nicht Bestandteil der Quellcodeverwaltung.
