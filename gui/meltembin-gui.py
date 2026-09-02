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

def openOutputDirectory():
    path = filedialog.askdirectory(title="Select output directory")
    if path:
        outputPath.delete(0, tk.END)
        outputPath.insert(0, path)

def runMeltembin(source, output):
    process = subprocess.Popen([meltembin, source, output], 
                               stdout=subprocess.PIPE, 
                               stderr=subprocess.STDOUT, 
                               text=True
                               )
    lastUpdate = 0

    for line in process.stdout:
        now = time.monotonic()

        if (now - lastUpdate) >= 0.05:
            window.after(0, meltLabel.configure, {"text": line.strip()})
            lastUpdate = now
    process.wait()

def melt():
    source = sourcePath.get()
    output = outputPath.get()

    if meltembin is None:
        meltLabel.configure(text="Error: meltembin binary not found!")
        return

    if (not source):
        meltLabel.configure(text="Error: a CUE file needs to be selected!")
        return

    threading.Thread(target=runMeltembin, args=(source, output), daemon=True).start()

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

main = ttk.Frame(window)
main.grid(row=0, column=0)
main.columnconfigure(0, weight=1)

sourceLabel = ttk.Label(main, text="Source .CUE")
sourceLabel.grid(row=0, column=0, pady=10)
sourcePath = ttk.Entry(main, width=50)
sourcePath.grid(row=1, column=0, padx=10, sticky="ew")
sourceBrowse = ttk.Button(main, text="Open...", width=20, command=openCue)
sourceBrowse.grid(row=1, column=1, padx=10)

main.rowconfigure(2, minsize=30)

outputLabel = ttk.Label(main, text="Output .BIN")
outputLabel.grid(row=3, column=0, pady=10)
outputPath = ttk.Entry(main)
outputPath.grid(row=4, column=0, padx=10, sticky="ew")
outputBrowse = ttk.Button(main, text="Save at...", width=20, command=openOutputDirectory)
outputBrowse.grid(row=4, column=1, padx=10)

#main.rowconfigure(5, minsize=30)
meltLabel = ttk.Label(main)
meltLabel.grid(row=5, column=0, columnspan=2, pady=20)
meltButton = ttk.Button(main, text="Melt", width=25, padding=14, command=melt)
meltButton.grid(row=6, column=0, columnspan=2)

window.mainloop()
