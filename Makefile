CC = cc
CFLAGS = -Wall -Wextra -Wno-use-after-free

.ONESHELL:

all: meltembin

meltembin: meltembin.c functions.c functions.h
	@$(CC) $(CFLAGS) meltembin.c functions.c -o meltembin

install: meltembin
	@sudo install -Dm755 meltembin /usr/local/bin/meltembin
	@sudo install -Dm755 mbrename /usr/local/bin/mbrename
	@rm meltembin

uninstall:
	@sudo rm -f /usr/local/bin/meltembin

gui: meltembin gui/meltemgui.py
	@mv meltembin gui
	@cd gui
	@python -m venv .venv
	@./.venv/bin/python -m pip install -q pyinstaller
	@./.venv/bin/python -m PyInstaller --log-level ERROR --onefile --windowed --add-binary "meltembin:." meltemgui.py
	@mv dist/meltemgui ..
	@rm -rf dist build .venv meltemgui.spec meltembin

install-gui: gui
	@sudo install -Dm755 meltemgui /usr/local/bin/meltemgui
	@rm meltemgui

uninstall-gui:
	@sudo rm -f /usr/local/bin/meltemgui

.PHONY: all install uninstall gui install-gui uninstall-gui
