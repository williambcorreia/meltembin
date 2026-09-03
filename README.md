# meltembin

meltembin (a pun on "melt em' bin") is a simple program written in C that parses a `.cue` file and melts all referenced `.bin` files into a single one.

## requirements

* POSIX-compatible OS
* C compiler available on `PATH` (such as `gcc` or `clang`)

## releases

Pre-built binaries are available on the [Releases](https://github.com/williambcorreia/meltembin/releases) page.

Available builds include:

- Linux CLI x86-64
- Linux GUI x86-64
- Windows GUI x86-64
- Source code

## installation via compilation

This guide is intended for users who want to build the binary themselves or use an unsupported architecture (ARM64, ARM32, x86).

### CLI
```bash
git clone https://github.com/williambcorreia/meltembin
cd meltembin
cc meltembin.c functions.c -o meltembin
sudo cp meltembin /usr/local/bin
```

### GUI

```bash
git clone https://github.com/williambcorreia/meltembin
cd meltembin
cc meltembin.c functions.c -o gui/meltembin
cd gui
python -m venv .venv
source .venv/bin/activate
pip install pyinstaller
pyinstaller --onefile --windowed --add-binary "meltembin:." meltemgui.py
sudo cp dist/meltemgui /usr/local/bin
```

Alternatively, you can run the GUI directly with Python:
```bash
python gui/meltemgui.py
```

## usage

The intended usage for meltembin is:

```bash
meltembin <cue-file> [dest-dir]
```

The program currently expects PSX BIN files referenced by the CUE sheet to use 2352-byte sectors.
Example:
```text
FILE "Castlevania - Symphony of the Night (USA) (Track 1).bin" BINARY
  TRACK 01 MODE2/2352
    INDEX 01 00:00:00
```

Currently, the output filename cannot be specified manually. The generated `.bin` file keeps the same base name as the input `.cue` file.

For example:

```bash
meltembin bingame/Castlevania\ SOTN\ \(USA\).cue dest-dir/
```

generates:

```text
dest-dir/
├── Castlevania SOTN (USA).bin
└── Castlevania SOTN (USA).cue
```

If no destination directory is specified:

```bash
meltembin bingame/Castlevania\ SOTN\ \(USA\).cue
```

the generated files are created in the current directory.

## status

The project is currently functional and under active development.
