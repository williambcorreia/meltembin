# meltembin

meltembin is a simple program written in C that parses a `.cue` file and melts all identified `.bin` files into a single one

## requirements

- POSIX-compatible OS
- C compiler available on `PATH` (such as `gcc` or `clang`)

## usage

the intended usage for meltembin is `meltembin [.cue] [destDir]`

if `destDir` is not specified, generates the `.bin` and `.cue` in the current directory `.`

## installation
currently, the installation process is:
```bash
git clone https://github.com/williambcorreia/meltembin
cd meltembin
cc meltembin.c functions.c -o meltembin
sudo mv meltembin /usr/local/bin
```
in future versions, a Makefile-based installation will also be available

## status
the project is currently functional and under active development
