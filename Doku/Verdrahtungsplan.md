# Verdrahtungsplan für XPanino Transponder und Uhr {#verdrahtungsplan}

@tableofcontents

## Arduino Uno

Nachfolgend ist der Verdrahtungsplan dargestellt, wie er für die Realisierung des Prototyps, Transponder und Uhr, aufgebaut wurde.

### Schaltermatrix 3 x 8

| Y=Row / X=Col | X=0    | 1     | 2     | 3     | 4     | 5     | 6    | 7     |
| ------------- | ------ | ----- | ----- | ----- | ----- | ----- | ---- | ----- |
| Y=0           | "IDT"  | "VFR" | "CLR" | "OFF" | "SBY" | "TST" | "ON" | "ALT" |
| 1             | "0"    | "1"   | "2"   | "3"   | "4"   | "5"   | "6"  | "7"   |
| 2             | "CTRL" | "SEL" | "OAT" |       |       |       |      |       |

### Arduino Uno <--> Schalter-Matrix

Diese Zuordnung muss geprüft und neu geschrieben werden, da sie erstmal nicht stimmt.

| Arduino-<br />Pin | Atmega328-<br />Bezeichnung | Stecker-<br />Pin | Signal | Draht-<br />Farbe |
| ----------------- | --------------------------- | ----------------- | ------ | ----------------- |
| Pin A0            | PC0                         | Pin 1             | ROW0   | sw                |
| Pin A1            | PC1                         | Pin 14            | ROW1   | Ws                |
| Pin 4             | PD4                         | Pin 2             | COL0   | gr                |
| Pin 5             | PD5                         | Pin 13            | COL1   | vi                |
| Pin 6             | PD6                         | Pin 3             | COL2   | bl                |
| Pin 7             | PD7                         | Pin 12            | COL3   | gn                |
| Pin 8             | PB0                         | Pin 4             | COL4   | ge                |
| Pin 9             | PB1                         | Pin 11            | COL5   | or                |
| Pin 10            | PB2                         | Pin 5             | COL6   | rt                |
| Pin 11            | PB3                         | Pin 10            | COL7   | br                |
| Pin A2            | PC2                         | Pin 6             | ROW2   | gr                |
| --                | --                          | Pin 9             | --     | ws                |
| --                | --                          | Pin 7             | --     | sw                |
| --                | --                          | Pin 8             | --     | --                |

### LED-Matrix 2 x (8 x 16)

noch zu ergänzen.

### Arduino Uno <--> LED-Matrix

Arduino-<br />Pin |Atmega328-<br />Bezeichnung | Stecker-<br />Pin | Signal | Draht-<br />Farbe 
------------|-------------|---------------|------------|------------
Pin A3      |PC3      | Pin 1      |  | sw  
Pin A4      |PC4      | Pin 10    |  | ws
Pin A5      |PC5      | Pin 2     |        | gr
Pin 12      |PB4      | Pin 9     |  | vi
Pin GND     |GND     | Pin 6     | GND           | br

## Raspberry Pico

### Notizen

Beispielcode für Schaltermatrix mit PIO-Code: <https://github.com/GitJer/Some_RPI-Pico_stuff/tree/main/button_matrix_4x4>

