# nopNES
This is a NES Emulator that I'm working on as a side project.  
This means that there may occasionally be gaps of no progress between commits.  
# Mappers
The emulator currently supports mappers 0, 66, and 113.
It has partial support for mappers 1, 2, 3, 4.
It has unfinished support for mapper 7, 9, 52, 90, 209, and 211.
## Running (Windows)
In the bin/Debug folder, there is a file called nopNES.exe.  
Download that, as well as pal.pal and ubuntu.ttf and put those in a seperate folder.  
Put your roms there as well.  
Download all of the DLLS and put it in that folder too  
Then, double click on nopNES.exe to run it.  
Enter the name of the rom and play.  
NOTE!  Windows builds are untested and experimental and may contain bugs!  
## Running (Linux)
In the bin/Debug folder, there is a file called nopNES.  
Download that, as well as pal.pal and ubuntu.ttf and put those in a seperate folder.  
Put your roms there as well.  
From there, open a terminal and navigate to the place that you put nopNES and the roms.  
Type ./nopNES and press enter.  
Enter the name of the rom and play.  
## Controls
Up = Up Arrow  
Down = Down Arrow  
Left = Left Arrow  
Right = Right Arrow  
Start = Enter  
Select = Space  
B = Z  
A = X  
## Hotkeys
Left Shift = Rewind
Q = Slowdown the emulator by an amount  
W = Overclock or underclock the CPU  
E = Toggle Sound  
R = Reset  
T = Configure Real Time Corruptor  
Y = Toggle Emulator Throttle Mode  
U = Toggle between the old and new renderer  
I = Savestate  
O = Loadstate  
P = Print Debug Info  
1 = Restart Emulator  
2 = Toggle Rewind  
7 = Toggle Square Channel 1  
8 = Toggle Square Channel 2  
9 = Toggle Triangle Channel  
0 = Toggle Noise Channel  
