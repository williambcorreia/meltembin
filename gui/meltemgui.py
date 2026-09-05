import os
import sys
import subprocess
import threading
import time
import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
from shutil import which

if getattr(sys, "frozen", False):
    meltembin = os.path.join(sys._MEIPASS, "meltembin")
else:
    meltembin = which("meltembin")

def openCue():
    path = filedialog.askopenfilename(
            title="Select CUE file",
            filetypes=[("CUE files", "*.cue"), ("All files", "*.*")]
            )
    if path:
        sourcePath.delete(0, tk.END)
        sourcePath.insert(0, path)

def openDirectory(extension):
    if extension == "cue":
        path = filedialog.askdirectory(title="Select input directory")
        if path:
            sourcePath.delete(0, tk.END)
            sourcePath.insert(0, path)
    else:
        path = filedialog.askdirectory(title="Select output directory")
        if path:
            outputPath.delete(0, tk.END)
            outputPath.insert(0, path)

def runMeltembin(source, output):
    kwargs = {
            "stdout": subprocess.PIPE,
            "stderr": subprocess.STDOUT,
            "text": True
        }

    if sys.platform == "win32":
        kwargs["creationflags"] = subprocess.CREATE_NO_WINDOW

    process = subprocess.Popen([meltembin, source, output], **kwargs)
    lastUpdate = 0

    lastLine = ""
    for line in process.stdout:
        lastLine = line.strip()
        now = time.monotonic()

        if (now - lastUpdate) >= 0.05:
            window.after(0, meltLabel.configure, {"text": lastLine})
            lastUpdate = now

    process.wait()
    window.after(0, meltLabel.configure, {"text": lastLine})

def meltOne(source, output):
    runMeltembin(source, output)
    window.after(0, meltButton.configure, {"state": "enabled"})

def meltAll(source, output):
    for cue in os.listdir(source):
        if cue.endswith(".cue"):
            runMeltembin(os.path.join(source, cue), output)
    window.after(0, meltButton.configure, {"state": "enabled"})

def melt():
    source = sourcePath.get()
    output = outputPath.get()

    if meltembin is None:
        meltLabel.configure(text="Error: meltembin binary not found!")
        return

    if (not source):
        meltLabel.configure(text="Error: a CUE file needs to be selected!")
        return

    meltButton.configure(state="disabled")

    match mode.get():
        case "single":
            target = meltOne
        case "batch":
            target = meltAll

    threading.Thread(target=target, args=(source, output), daemon=True).start()

def rename(cue):
    originalName = os.path.basename(cue)
    newName = tk.simpledialog.askstring("Rename", originalName)

    if not newName:
        return

    bin = cue[:-4] + ".bin"
    newCue = os.path.join(os.path.dirname(cue), newName + ".cue")
    newBin = os.path.join(os.path.dirname(cue), newName + ".bin")

    with open(cue, "r") as file:
        content = file.read()

    content = content.replace(os.path.basename(bin), newName + ".bin")
    os.rename(cue, newCue)

    with open(newCue, "w") as file:
        file.write(content)

    os.rename(bin, newBin)
    window.after(0, meltLabel.configure, {"text": "Game file and references renamed!"})

def renameAll(source):
    for cue in os.listdir(source):
        if cue.endswith(".cue"):
            rename(os.path.join(source, cue))
    window.after(0, meltLabel.configure, {"text": "All game files and references renamed!"})

def modes():
    match mode.get():
        case "renameAll":
            meltButton.configure(text="Rename", command=lambda: renameAll(sourcePath.get()))
            sourceLabel.configure(text="Source CUEs directory")
            sourceBrowse.configure(command=lambda: openDirectory("cue"))
            outputLabel.configure(text="-")
            outputPath.configure(state="disabled")
            outputBrowse.configure(state="disabled")
        case "rename":
            meltButton.configure(text="Rename", command=lambda: rename(sourcePath.get()))
            sourceLabel.configure(text="Source .CUE")
            sourceBrowse.configure(command=openCue)
            outputLabel.configure(text="-")
            outputPath.configure(state="disabled")
            outputBrowse.configure(state="disabled")
        case "batch":
            sourceLabel.configure(text="Source CUEs directory")
            sourceBrowse.configure(command=lambda: openDirectory("cue"))
            outputLabel.configure(text="Output directory")
            outputPath.configure(state="enabled")
            outputBrowse.configure(state="enabled")
        case _:
            meltButton.configure(text="Melt")
            sourceLabel.configure(text="Source .CUE")
            sourceBrowse.configure(command=openCue)
            outputLabel.configure(text="Output directory")
            outputPath.configure(state="enabled")
            outputBrowse.configure(state="enabled")

window = tk.Tk()
window.title("Meltembin GUI")
window.geometry("960x540")
window.rowconfigure(0, weight=1)
window.columnconfigure(0, weight=1)

style = ttk.Style()
style.theme_use("clam")
style.configure("TEntry", padding=6)
style.configure("TLabel", font=("DejaVu Sans", 12))
style.configure("TButton", font=("DejaVu Sans", 12))
style.configure("TRadiobutton", font=("DejaVu Sans", 12))

container = ttk.Frame(window)
container.grid(row=0, column=0, sticky="nsew")

container.rowconfigure(0, weight=1)
container.columnconfigure(0, weight=1)

main = ttk.Frame(container)
main.grid(row=0, column=0, sticky="ew")
main.columnconfigure(0, weight=1)

sourceLabel = ttk.Label(main, text="Source .CUE")
sourceLabel.grid(row=0, column=0, pady=10)
sourcePath = ttk.Entry(main, width=50, font=("DejaVu Sans", 12))
sourcePath.grid(row=1, column=0, padx=10, sticky="ew")
sourceBrowse = ttk.Button(main, text="Open...", width=20, command=openCue)
sourceBrowse.grid(row=1, column=1, padx=10)

main.rowconfigure(2, minsize=30)

outputLabel = ttk.Label(main, text="Output directory")
outputLabel.grid(row=3, column=0, pady=10)
outputPath = ttk.Entry(main, font=("DejaVu Sans", 12))
outputPath.grid(row=4, column=0, padx=10, sticky="ew")
outputBrowse = ttk.Button(main, text="Save to...", width=20, command=lambda: openDirectory("bin"))
outputBrowse.grid(row=4, column=1, padx=10)

meltLabel = ttk.Label(main)
meltLabel.grid(row=5, column=0, columnspan=2, pady=20)
meltButton = ttk.Button(main, text="Melt", width=25, padding=14, command=melt)
meltButton.grid(row=6, column=0, columnspan=2)

main.rowconfigure(7, minsize=20)

mode = tk.StringVar(value="single")

singleButton = ttk.Radiobutton(main, text="Melt one game", variable=mode, value="single", command=modes)
singleButton.grid(row=8, column=0, columnspan=2)
batchButton = ttk.Radiobutton(main, text="Melt all games", variable=mode, value="batch", command=modes)
batchButton.grid(row=9, column=0, columnspan=2)

renameButton = ttk.Radiobutton(main, text="Rename one game", variable=mode, value="rename", command=modes)
renameButton.grid(row=10, column=0, columnspan=2)
renameAllButton = ttk.Radiobutton(main, text="Rename all games", variable=mode, value="renameAll", command=modes)
renameAllButton.grid(row=11, column=0, columnspan=2)

window.mainloop()
