sfx
=== 

Super simple sound effects manager for Windows. After having been annoyed by expensive, commercial, bloated and slow sound effect managers, I decided to write my own super simple version. It can read its WAV files from a local harddisk for efficiency and execution speed, but we're seeing excellent results at work reading WAV files from an ISIS network drive. It requires no installation, just copy all the files to a directory. Two text files dictate which sounds are found where:

config.txt
==========

This text file has one line and it tells where all the sound effects are stored. So this is the longest possible path to sound files that all files have in common. Usually something like:

D:\SFX\

config.txt must be in the same directory as sfx.exe

data.txt
========

This text file lists all the WAV files and their descriptions. The first line is ignored, so it can be used for a small description of its purpose. A typical data file might look like this:

* * * Data for sfx (this line is ignored).  
Sound of a frog  
animals\frog.wav  
Sound of a tiger  
animals\tiger.wav  
High speed train passing by  
transport\fast train.wav  

Combined with the above CONFIG.TXT, this will only work if the following files exist on the system:

D:\SFX\animals\frog.wav  
D:\SFX\animals\tiger.wav  
D:\SFX\transport\fast train.wav  

data.txt must be in the same directory as the sound effects, so in the above example it should be at:

D:\SFX\data.txt

textbuilder
===========

There is a simple textbuilder console application, which builds data.txt and config.txt from a given directory of sound effects. E.g. with the above directory structure, you'd call it with:

textbuilder "D:\SFX"

process.py
==========

A simple python script that checks if all the files are in their proper location. For hand generated data.txt files, this can be used to weed out missing files and to alpha sort on descriptions, etc.

info
====

Hens Zimmerman
hz37@xs4all.nl

