# meltembin

meltembin is a simple program written in C that parses a `.cue` file and melts all identified `.bin` files into a single one.

## requirements

* POSIX-compatible OS
* C compiler available on `PATH` (such as `gcc` or `clang`)

## usage

The intended usage for meltembin is:

```bash
meltembin <cue-file> [dest-dir]
```

If `dest-dir` is not specified, the generated `.bin` and `.cue` files are created in the current directory (`.`).

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

## installation

Currently, the installation process is:

```bash
git clone https://github.com/williambcorreia/meltembin
cd meltembin
cc meltembin.c functions.c -o meltembin
sudo mv meltembin /usr/local/bin
```

In future versions, a Makefile-based installation will also be available.

## status

The project is currently functional and under active development.
