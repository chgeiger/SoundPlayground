# SoundPlayground

Kleines Qt6/C++-Projekt zum Experimentieren mit Audio-Modulen (u. a. Sinusgenerator und Audio-Out).

## Voraussetzungen

- Linux
- CMake (>= 3.16)
- C++17 Compiler (z. B. `g++`)
- Qt6 (`Core`, `Gui`, `Widgets`)
- Optional: PortAudio (`portaudio-2.0`) für Audio-Out

## In VS Code bauen und starten

Die VS-Code-Konfiguration ist auf den Build-Ordner `build/` abgestimmt.

- **Build-Task:** `build (cmake)`
  - Führt aus: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build -j`
- **Run-Task:** `run SoundPlayground (no debug)`
  - Startet: `build/SoundPlayground`
- **Debug (F5):** `Debug SoundPlayground`
- **Run ohne Debug (Ctrl+F5):** `Run SoundPlayground (Ctrl+F5)`

## Manuell im Terminal

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
./build/SoundPlayground
```

## Hinweise

- `compile_commands.json` wird nach `build/compile_commands.json` erzeugt.
- IntelliSense in VS Code verwendet diese Datei direkt.
