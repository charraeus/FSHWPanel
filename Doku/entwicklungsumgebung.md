# Entwicklungsumgebung {#entwicklungsumgebung}

@tableofcontents

## Entwicklungsumgebung mit Visual Studio Code
### Visual Studio Code
"IDE" mit folgenen VSCode-Erweiterungen:

* Für die Arduino-Programmierung: [***PlatformIO IDE***][vsc-01]
  Die Installation der Arduino-Entwicklungsumgebung ist damit *nicht* mehr notwendig.
* Für die C++-Entwicklung:
  * [***C/C++** by Microsoft*][vsc-02]
  * [***Makefile Tools** by Microsoft*][vsc-03]
* Für Dokumentation:
  * Anzeige von Doku als Popup: [***Doxygen Documentation Generator** by Christoph Schlosser*][vsc-04]
  * Markdown-Support:
    * [***Pandoc Markdown Preview** by kzvi*][vsc-05]
    * [***Markdown Preview Enhanced** by Yiyi Wang*][vsc-06]
    * [***Markdown Preview Mermaid Support** by Matt Bierner*][vsc-07]
    * inaktiv [***markdownlint** by David Anson*][vsc-08]
    * inaktiv [***vscode-pandoc** by Chris chinchilla*][vsc-09]
* Remote-Entwicklung (per Mac auf Windows):
  * [***Remote - SSH** by Microsoft*][vsc-10]
  * [***Remote - SSH: Editing Configuration Files** by Microsoft*][vsc-11]
* GIT Versionsverwaltung:
  * [***GitLens - Git supercharged** by GitKraken*][vsc-12]
  * [***Git History** by Don Jayamanne*][vsc-13]

**Konfiguration**

Die Konfigurationen der Erweiterungen sind im Verzeichnis `.vscode` abgelegt.

[vsc-01]: (https://docs.platformio.org/en/latest/) "PlatformIO IDE  auf dem Visual Studio Marketplace"
[vsc-02]: (https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) "C/C++ auf dem Visual Studio Marketplace"
[vsc-03]: (https://marketplace.visualstudio.com/items?itemName=ms-vscode.makefile-tools) "Makefile Tools auf dem Visual Studio Marketplace"
[vsc-04]: (https://www.doxygen.nl) "Doxygen-Webseite"
[vsc-05]: (https://github.com/kzvi/vsc-pandoc-markdown-preview) "Pandoc Markdown Preview auf Github"
[vsc-06]: (https://github.com/shd101wyy/markdown-preview-enhanced) "Markdown preview enhanced auf github.com"
[vsc-07]: (https://marketplace.visualstudio.com/items?itemName=bierner.markdown-mermaid) "Markdown Preview Mermaid Support auf github.com"
[vsc-08]: (https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint) "markdownlint auf dem Visual Studio Marketplace"
[vsc-09]: (https://marketplace.visualstudio.com/items?itemName=ChrisChinchilla.vscode-pandoc) "vscode-pandoc auf dem Visual Studio Marketplace"
[vsc-10]: (https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh) "Remote-SSH auf dem Visual Studio Marketplace"
[vsc-11]: (https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh-edit) "Remote-SSH: Editing Configuration Files auf dem Visual Studio Marketplace"
[vsc-12]: (https://marketplace.visualstudio.com/items?itemName=eamodio.gitlens) "Gitlens - GIT supercharged auf dem Visual Studio Marketplace"
[vsc-13]: (https://marketplace.visualstudio.com/items?itemName=donjayamanne.githistory) "Git History auf dem Visual Studio Marketplace"

### Git

Für die Versionsverwaltung. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss.
```shell
brew install git
```

Auf [**Github**](https://github.com/CHarraeus) existiert ein entsprechendes Repository, das alle für das Projekt notwendigen Dateien enthält.

**Hinweise**

* *main* (die veraltete Bezeichnung *master* wird nicht verwendet) enthält zu jedem Zeitpunkt die neueste, lauffähige Version des ganzen Repository.
* Die Weiterentwicklung findet in ausschließlich im Branch *Development* und weiteren *Branches* statt.
* Minimale Einstellungen
  ```console
  $ git config --global user.name "Vorname Nachname"
  $ git config --global user.email vorname.nachname@example.com
  $ git config --global init.defaultBranch main
  ```

Link zur [GIT-Doku](https://git-scm.com/book/de/v2).

### GNU C++-Compiler: GCC

Für die Entwicklung der Softwareteile, die auf dem Arduino laufen wird die VSCode-Erweiterung ***PlatformIO IDE*** (siehe oben) verwendet. Für die Entwicklung des X-Plane-Interfaces (, das auf dem Flugsimulations-PC läuft), wird der ***GNU C++***-Compiler aus der ***GNU Compiler Collection (GCC)*** verwendet.

@todo noch zu ergänzen: Compiler-Konfiguration? etc.

@todo Eine gute Anleitung ["Configure the VSCode Environment for C++ on MacOS"][gcc-01] habe ich im Netz gefunden. Diese muss ich aber noch umsetzen -- insbes. die *tasks.json* und *launch.json*-Dateien verstehen.

```shell
brew install gcc
brew install cmake
```

Link zur [GCC-Doku][gcc-02].

[gcc-01]: (https://wrayx.uk/posts/configure-vscode-env-for-cpp-on-macos/)
[gcc-02]: (https://gcc.gnu.org/onlinedocs/)

### Doxygen mit zusätzlicher Software
Für das Generieren von Sourcecode-Doku. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss. Siehe auch hier: https://www.doxygen.nl.
Um [*mermaid*][mermaid]-Diagramme einbinden zu können, wird das [*Command-line interface (CLI) for mermaid*][mermaid-cli] benötigt.

```shell
brew install doxygen
brew install mermaid.cli
```

[mermaid]: https://github.com/mermaid-js/mermaid "mermaid auf github.com"
[mermaid-cli]: https://github.com/mermaid-js/mermaid-cli "mermaid-cli auf github.com"

**Konfiguration**

* *Doxygen* wird über die Konfigurationsdatei `Doxygen`konfiguriert, die im Projektverzeichnis liegt.
* Eigentlich wird noch ein mermaid-Filter für *pandoc* benötigt (siehe unten bei *pandoc*). Aber mittels des mermaid-cli braucht's das wohl nicht? Dafür muss dann aber
  * der mermaid-Quellcode in einzelnen Dateien stehen,
  * die Bilder per Skript `mermaid.sh` erzeugt und
  * die Bilder dann in die Markdown-Datei eingebunden werden.

### pandoc

Wird u.A. von den VSCode-Erweiterungen benötigt. Siehe unten. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss. Siehe auch hier: https://pandoc.org
```shell
brew install pandoc
```

**Konfiguration**

* Keine besondere Konfiguration notwendig.
* Eigentlich wird noch ein mermaid-Filter benötigt. Dieser ist aber noch nicht installiert und daher auch hier noch nicht dokumentiert.

### Visual Paradigm Community Editon

Für die Erstellung von Diagrammen (insbes. Zustandsdiagrammen) wird die Community-Edition von Visual Paradigm genutzt. Die Installation auf dem Mac erfolgt mittels Homebrew, das natürlich installiert sein muss. https://www.visual-paradigm.com
```shell
brew install visual-paradigm-ce
```

## Style-Guides

* C++
  * [Google C++ Style Guide][stg-01]
  * [Google C++ Linter `cpplint`][stg-02]
* Python
  * [PEP 8][stg-03]
  * [Google Python Style Guide][stg-04]
* [Google Shell Style Guide][stg-05]
* [Google Go Style Guide][stg-06]
* [Google XML Document Format Style Guide][stg-07]

[stg-01]: https://google.github.io/styleguide/cppguide.html
[stg-02]: https://google.github.io/styleguide/xmlstyle.html
[stg-03]: https://www.python.org/dev/peps/pep-0008/
[stg-04]: https://google.github.io/styleguide/pyguide.html
[stg-05]: https://google.github.io/styleguide/shell.xml
[stg-06]: https://google.github.io/styleguide/go/
[stg-07]: https://google.github.io/styleguide/xmlstyle.html
