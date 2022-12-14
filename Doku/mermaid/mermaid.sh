#!/bin/bash
# @brief aus mermaid-Quellcode die Bilder generieren und speichern.
#
# mmdc ist der mermaid command line client.
# Dieser kann mermaid- und PlantUML-Quellecode verarbeiten.
# PlantUML: http://plantuml.com/
# mermaid:

for mmd in ./src/*.mmd
do
    filename="${mmd##*/}"
    echo "Generating $mmd"
    /usr/local/bin/mmdc -i $mmd -o ./img/${filename%%.*}.svg
done