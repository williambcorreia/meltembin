# meltembin

meltembin (a pun on "melt em' bin") is a simple program written in C that parses a `.cue` file and melts all referenced `.bin` files into a single one.

## releases

Pre-built binaries are available on the [Releases](https://github.com/williambcorreia/meltembin/releases) page.

Available builds include:

- Linux CLI x86-64
- Linux GUI x86-64
- Windows GUI x86-64

## building from source

This guide is intended for users on POSIX-compatible systems who want to build the binary themselves or use an unsupported architecture (ARM64, ARM32, x86).

### requirements

- POSIX-compatible OS
- C compiler available on `PATH` (such as `gcc` or `clang`)
- Python 3.x (if building the GUI is desired)

Read it like this: ACTION | CLI | GUI

Build | `make` | `make gui`
Build and install | `make install` | `make install-gui`
Uninstall | `make uninstall` | `make uninstall-gui`

Alternatively, you can run the GUI directly with Python:
```bash
python gui/meltemgui.py
```

## usage

The intended usage for meltembin is:

```bash
meltembin <cue-file> [dest-dir]
```

If no destination directory is specified, the generated files are created in the current directory. The output filename cannot be specified manually, so the generated `.bin` file keeps the same base name as the input `.cue` file.

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

### batch processing

The binary doesn't have a batch mode, but you can easily achieve this with a shell command:

```bash
for cue in srcdir/*.cue; do meltembin "$cue" dest-dir/; done
```

## supported BIN format
The program currently expects PSX BIN files referenced by the CUE sheet to use 2352-byte sectors.
Example:
```text
FILE "Castlevania - Symphony of the Night (USA) (Track 1).bin" BINARY
  TRACK 01 MODE2/2352
    INDEX 01 00:00:00
```

## status

The project is currently functional and under active development.
