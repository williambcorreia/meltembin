# meltembin (a pun on "melt em' bin")

## what is meltembin?

meltembin is a PSX BIN/CUE merger that combines multiple BIN tracks referenced by a CUE sheet into a single BIN/CUE pair. It can be used from the command line or through the graphical interface.

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

| ACTION | CLI | GUI |
|---|---|---|
| Build | `make` | `make gui` |
| Build and install | `make install` | `make install-gui` |
| Uninstall | `make uninstall` | `make uninstall-gui` |

Alternatively, you can run the GUI directly with Python (requires the installed CLI binary):
```bash
python gui/meltemgui.py
```

## supported BIN format
The program currently expects PSX BIN files referenced by the CUE sheet to use 2352-byte sectors.
Example:
```text
FILE "Castlevania - Symphony of the Night (USA) (Track 1).bin" BINARY
  TRACK 01 MODE2/2352
    INDEX 01 00:00:00
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

The binary doesn't have a batch function, but you can easily achieve this with a shell command:

```bash
for cue in srcdir/*.cue; do meltembin "$cue" dest-dir/; done
```

### renaming output BIN

To rename the output `.bin` and its references in the `.cue` sheet, you can use the `mbrename` shell script included with `make install`. It takes a `.cue` file as input, prompts you for a new name, then renames both the `.cue` and `.bin` files and updates all `.bin` references in the `.cue` file accordingly.

You can use the script with files in the same directory or in different ones:

```bash
mbrename game1.cue game2.cue
mbrename games/*.cue
mbrename games1/game.cue games2/game.cue
```
