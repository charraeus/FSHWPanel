# Verdrahtungsplan für XPanino Transponder und Uhr {#Verdrahtungsplan}

## Raspberry Pico

### Notizen

Beispielcode für Schaltermatrix mit PIO-Code: <https://github.com/GitJer/Some_RPI-Pico_stuff/tree/main/button_matrix_4x4>

### 5 x 4 Schaltermatrix für Transponder und Uhr

|Y=Row / X=Col | X=0   | 1      | 2      | 3      | 4      |
|--------------|-------|--------|--------|--------|--------|
| Y=0          | "0"   |  "1"   | "2"    | "3"    | "CLR"  |
| 1            | "4"   |  "5"   | "6"    | "7"    | "VFR"  |
| 2            | "OFF" |  "SBY" | "TST"  | "ON"   | "ALT"  |
| 3            | "OAT" |  "SEL" | "CTRL" | -      | "IDT"  |

### Raspberry Pico <--> FSHWPanel-Transponder (Schalter)

Schaltplan: *Transponder KT 76C, Uhr Davtron M803 - Anschluss LED und 7-Segment-Anzeigen, Schalter.pdf*

Pico-PIN    |      | Stecker-PIN | Logische-Bez. | Draht-Farbe
------------|------|-------------|---------------|------------
**Schalter:**
GPxx  Pin   | <--> | Pin  1      | SW-Col 0      | gr
GPxx  Pin   | <--> | Pin  2      | SW-Col 1      | vi
GPxx  Pin   | <--> | Pin  3      | SW-Col 2      | bl
GPxx  Pin   | <--> | Pin  4      | SW-Col 3      | gn
GPxx  Pin   | <--> | Pin  5      | SW-Col 4      | ge
GPxx  Pin   | <--> | Pin  6      | SW-Row 0      | or
GPxx  Pin   | <--> | Pin  7      | SW-Row 1      | rt
GPxx  Pin   | <--> | Pin  8      | SW-Row 2      | br
GPxx  Pin   | <--> | Pin  9      | SW-Row 3      | sw
GPxx  Pin   | <--> | Pin 10      | -             | ws
**LEDs:**
GPxx  Pin   | <--> | Pin  1      |       | sw
GPxx  Pin   | <--> | Pin  2      |       | ws
GPxx  Pin   | <--> | Pin  3      |       | gr
GPxx  Pin   | <--> | Pin  4      |       | vi
GND  Pin    | <--> | Pin 10      | GND           | br


## Arduino Uno

**Achtung: Sobald die Transponder-/Uhr-Hardware gemäß den im Schaltplan-PDF blau eingezeichneten Änderungen umgebaut wurde, sind die folgenden Tabellen nicht mehr korrekt.**

Nachfolgend ist der Verdrahtungsplan dargestellt, wie er für die Realisierung des Prototyps, Transponder und Uhr, aufgebaut wurde.

### Arduino Uno <--> FSHWPanel-LED-Modul (LEDs)

Arduino-PIN |      | Stecker-PIN | IOW-Bez.      | Draht-Farbe
------------|------|-------------|---------------|------------
Pin  2      | <--> | Pin  1      | IOW1-PORT-1.0 | sw  
Pin  3      | <--> | Pin  2      | IOW1-PORT-1.1 | ws
Pin  4      | <--> | Pin  3      | IOW1-PORT-1.3 | gr
Pin  5      | <--> | Pin  4      | IOW1-PORT-1.4 | vi
Pin GND     | <--> | Pin 10      | GND           | br

### Arduino Uno <--> FSHWPanel-Transponder (Schalter)

Arduino-PIN |      | Stecker-PIN | IOW-Bez.      | Draht-Farbe
------------|------|-------------|---------------|------------
Pin  6      | <--> | Pin  1      | IOW2-COL0     | gr
Pin  7      | <--> | Pin  2      | IOW2-COL1     | vi
Pin  8      | <--> | Pin  3      | IOW2-COL2     | bl
Pin  9      | <--> | Pin  4      | IOW2-COL3     | gn
Pin 10      | <--> | Pin  5      | IOW2-COL4     | ge
Pin 11      | <--> | Pin  6      | IOW2-COL5     | or
Pin 12      | <--> | Pin  7      | IOW2-COL6     | rt
Pin 13      | <--> | Pin  8      | IOW2-COL7     | br
Pin A0      | <--> | Pin  9      | IOW2-ROW0     | sw
Pin A1      | <--> | Pin 10      | IOW2-ROW1     | ws

### Arduino Uno <--> FSHWPanel-Clock (Schalter)

|Arduino-PIN |      | Stecker-PIN | IOW-Bez.    | Draht-Farbe|
|------------|------|-------------|-------------|------------|
|Pin A2      | <--> |  Pin  1     | IOW2-ROWS6  | sw         |
|Pin A3      | <--> |  Pin  2     | IOW2-ROWS4  | ws         |
|Pin 12      | <--> |  Pin 10     | IOW2-COL7   | rt         |
|Pin 13      | <--> |  Pin  9     | IOW2-COL6   | br         |
