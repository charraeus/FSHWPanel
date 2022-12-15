# Klassendiagramme {#klassendiagramme}

@tableofcontents

## Gesamtbild

[![Alle Klassen im Zusammenspiel.][bild-01] Das Gesamtbild aller Klassen][bild-01]

[bild-01]: ./mermaid/img/class-diagram.svg

## Davtron Uhr M803 {#m803}

@todo hier muss noch was 'rein

## Transponder Bendix King KT76C {#kt76c}

@todo hier muss noch was 'rein

## Hinweise zu den Mermaid-Diagrammen: {#mermaid}

>
> | Type    | Description   |
> |---------|---------------|
> | <\|\--  | Inheritance   |
> | *\--    | Composition   |
> | o\--    | Aggregation   |
> | \-->    | Association   |
> | \--     | Link (Solid)  |
> | ..>     | Dependency    |
> | ..\|>   | Realization   |
> | ..      | Link (Dashed) |
> 
> | 2-way relations | Description   |
> |-----------------|---------------|
> | <\|-\-\|>       | [Relation Type][Link][Relation Type] |
>
> | Relation Type | Description |
> |---------------|-------------|
> | \<\|          | Inheritance |
> | *             | Composition |
> | o             | Aggregation |
> | \>            | Association |
> | \<            | Association |
> | \|\>          | Realization |
>
> | Link Type | Description
> |-----------|------------
> | \--       | Solid
> | ..        | Dashed
>
> | Cardinality /<br/>Multiplicity on relations | Description
> |-------------|-----------------|
> | Cardinality can be easily defined by placing the text option within quotes before or after a given arrow.<br/>[classA] "cardinality1" [Arrow] "cardinality2" [ClassB]:LabelText<br/>e.g. `Student "1" --> "1..*" Course`||
> | 1           | Only 1
> | 0..1        | Zero or One
> | 1..*        | One or more
> | *           | Many
> | n           | n {where n>1}
> | 0..n        | zero to n {where n>1}
> | 1..n        | one to n {where n>1}
