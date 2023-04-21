# BrainBodyComputerInterface

Entwicklung eines kleinen Spiels, um verschiedene Biosensoren zu demonstrieren.
Ziel des Spiels ist es ein Moving-Head Scheinwerfer über Biosensoren zu steuern, so dass dieser eine von der Software ausgewählte Person anstrahlt.

## Ordnerstruktur

 - bin - fertiger build
 - doc - Dokumentation und sonstige Dokumente
 - src - Sourcecode des Project
 - libs - verwendete libaries

## Einrichtung der Entwicklungsumgebung

Für das Projekt wird VS-Code und MSYS2 verwendet.
Es könnte auch alternativ Visual Studio CE oder ein anderer Editor verwendet werden.

### Installation MSYS2
MSYS2 wird benötigt, um die Build-Umgebung bereit zu stellen.
MSYS2 kann von hier heruntergeladen und installiert werden:
https://www.msys2.org/

#### Installation der notwendigen Pakete
Ist MSYS2 installiert kann es gestartet werden ("MSYS2 MSYS" im Startmenü)
Die Pakete werden mit dem folgenden Befehl installiert:

    pacman -S base-devel mingw-w64-x86_64-toolchain mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_ttf mingw64/mingw-w64-x86_64-SDL2_net 

### Installation Vulkan SDK
Vulkan an sich sollte auf den meisten PCs vorhanden sein.
Die Vulkan SDK wird dennoch benötigt, um verschiedene Entwicklungstools bereitzustellen
Die Vulkan SDK kann hier heruntergeladen werden:
https://vulkan.lunarg.com/

### Installation VS-Code
https://code.visualstudio.com/Download

#### VS-Code Erweiterungen
Es werden folgende Erweiterungen genutzt:
 - C/C++ Extension Pack
 - CMake

sonstige (nicht notwendig):
 - indent-rainbow
 - Code Spell Checker
 - Project Manager (wenn man VS-Code parallel für andere Projekte nutzen will)

### (optional) Latex einrichten
Wenn für das Schreiben der Dokumentation Latex verwendet werden soll, ist es am einfachsten, wenn man dazu "TeX Live" verwendet.
Dazu muss zuerst TexLive installiert werden: https://www.tug.org/texlive/acquire-netinstall.html (install-tl-windows.exe runterladen)
Als Editor kann einfach in VS-code das "LaTeX Workshop" plugin verwendet werden.
Alternativ kann als editor "Texmarker" verwendet werden.


