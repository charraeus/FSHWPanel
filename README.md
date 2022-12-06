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

## Entwicklungsumgebung mit Visual Studio Code
### Visual Studio Code
"IDE" mit folgenen VSCode-Erweiterungen:
* Für die Arduino-Programmierung: [***PlatformIO IDE***](https://docs.platformio.org/en/latest/)  
  Die Installation der Arduino-Entwicklungsumgebung ist damit *nicht* mehr notwendig.
* Für die C++-Entwicklung:
  * [***C/C++** by Microsoft*](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
  * [***Makefile Tools** by Microsoft*](https://marketplace.visualstudio.com/items?itemName=ms-vscode.makefile-tools)
* Für Dokumentation:
  * Anzeige von Doku als Popup: [***Doxygen Documentation Generator** by Christoph Schlosser*](https://www.doxygen.nl)
  * Markdown-Support:
    * [***Pandoc Markdown Preview** by kzvi*](https://github.com/kzvi/vsc-pandoc-markdown-preview)
    * [***Markdown Preview Mermaid Support** by Matt Bierner](https://marketplace.visualstudio.com/items?itemName=bierner.markdown-mermaid)
    * inaktiv [***markdownlint** by David Anson*](https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint)
    * inaktiv [***vscode-pandoc** by Chris chinchilla*](https://marketplace.visualstudio.com/items?itemName=ChrisChinchilla.vscode-pandoc)
* Remote-Entwicklung (per Mac auf Windows):
  * [***Remote - SSH** by Microsoft*](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh)
  * [***Remote - SSH: Editing Configuration Files** by Microsoft*](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh-edit)
* GIT Versionsverwaltung:
  * [***GitLens - Git supercharged** by GitKraken*](https://marketplace.visualstudio.com/items?itemName=eamodio.gitlens)
  * [***Git History** by Don Jayamanne*](https://marketplace.visualstudio.com/items?itemName=donjayamanne.githistory)

### Git
Für die Versionsverwaltung. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss. https://www.visual-paradigm.com
```shell
brew install git
```

Auf [**Github**](https://github.com/CHarraeus) existiert ein entsprechendes Repository, das alle für das Projekt notwendigen Dateien enthält.

**Hinweise**
* *main* (bzw. veraltet *master*) enthält zu jedem Zeitpunkt die neueste, lauffähige Version des ganzen Repository.
* Die Weiterentwicklung findet in ausschließlich in *Branches* statt.
* Minimale Einstellungen
  ```console
  $ git config --global user.name "Vorname Nachname"
  $ git config --global user.email vorname.nachname@example.com
  $ git config --global init.defaultBranch main
  ```

Link zur [GIT-Doku](https://git-scm.com/book/de/v2).


### C++-Compiler
Für die Entwicklung der Softwareteile, die auf dem Arduino laufen wird die VSCode-Erweiterung ***PlatformIO IDE*** (siehe oben) verwendet. Für die Entwicklung des X-Plane-Interfaces (, das auf dem Flugsimulations-PC läuft), wird der ***GNU C++***-Compiler bzw. ***CLANG*** verwendet.
@todo: noch zu ergänzen: welcher compiler genau? Compiler-Konfiguration? etc.
### Doxygen
Für das Generieren von Sourcecode-Doku. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss. Siehe auch hier: https://www.doxygen.nl
```shell
brew install doxygen
```

Eigentlich wird noch ein mermaid-Filter für `pandoc` benötigt, dieser ist aber noch nicht installiert und daher hier nicht dokumentiert.

### pandoc
Wird u.A. von den VSCode-Erweiterungen benötigt. Siehe unten. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss. Siehe auch hier: https://pandoc.org
```shell
brew install pandoc
```

### Visual Paradigm Community Editon
Für die Erstellung von Diagrammen (insbes. Zustandsdiagrammen) wird die Community-Edition von Visual Paradigm genutzt. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss. https://www.visual-paradigm.com
```shell
brew install visual-paradigm-ce
```

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
