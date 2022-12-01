# FSHWPanel

## Was ist FSHWPanel?

FSHWPanel ist eine aus mehreren Komponenten bestehende Software, mittels derer Schalter und Anzeigen (LEDs, 7-Segment-Anzeigen) an X-Plane angeschlossen werden können. Damit können dann Infos aus der Avionic – z.B. der Transpondercode oder eine COM-Frequenz – auf einem aus mehreren 7-Segment-Anzeigen bestehenden Modul angezeigt werden. Andersherum löst das Betätigen eines Tasters oder Schalters eine Aktion im X-Plane aus – z.B. das Einschalten des Transponders.

## Verzeichnisse

* **Doku**  
  Alle Dateien, die für die Dokumentation aus Entwickler- und Anwendersicht notwendig sind.

* **XPanino**  
  Quellcode für den Arduino.

  * **.vscode**  
    Spezifische Konfiguration für Visual Studio Code (VSCode).
  * **XPanino/doc/html**  
    Hier landen die durch `doxygen` generierten html-Dateien für die Doku des Quellcodes. Dieses Verzeichnis ist nicht Bestandteil der Quellcodeverwaltung.

  * **XPanino/include**  
    Dieses Verzeichnis ist für Header-Dateien `*.h`, `*.hpp` gedacht.

  * **XPanino/lib**  
    Dieses Verzeichnis ist für projektspezifische (private) Libraries gedacht. Der Source-Code jeder Library soll in einem eigenen Unterverzeichnis gespeichert werden. `lib/meine_library/[hier liegen die Source-Dateien]`

  * **XPanino/src**
    Dieses Verzeichnis ist für alle Source-Dateien gedacht, die nicht Teil von Libraries sind. z.B. `*.c`, `*.cpp`, usw.

* **XPIf**  
  Quellcode für die auf dem PC laufende Schnittstelle zu X-Plane. Die Unterverzeichnisse sind hier analog zu XPanino.

  * **XPIf/.vscode**
  * **XPIf/doc/html**
  * **XPIf/lib**
  * **XPIf/src**

## GIT

* *main* (bzw. veraltet *master*) enthält zu jedem Zeitpunkt die neueste, lauffähige Version des ganzen Repository.
* Die Weiterentwicklung findet in ausschließlich in *Branches* statt.
* Minimale Einstellungen

  ```console
  $ git config --global user.name "John Doe"
  $ git config --global user.email johndoe@example.com
  $ git config --global init.defaultBranch main
  ```

Link zur [GIT-Doku](https://git-scm.com/book/de/v2).

## Style-Guides

* C++
  * [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
  * [Google C++ Linter `cpplint`](https://google.github.io/styleguide/xmlstyle.html)

* Python
  * [PEP 8](https://www.python.org/dev/peps/pep-0008/)
  * [Google Python Style Guide](https://google.github.io/styleguide/pyguide.html)
* [Google Shell Style Guide](https://google.github.io/styleguide/shell.xml)
* [Google Go Style Guide](https://google.github.io/styleguide/go/)
* [Google XML Document Format Style Guide](https://google.github.io/styleguide/xmlstyle.html)
