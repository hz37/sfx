sfx
===

Super simple sound effects manager for Windows. After having been annoyed by expensive, commercial, bloated and slow sound effect managers, I decided to write my own super simple version. In theory it should read its WAV files from a local harddisk for efficiency and execution speed. It requires no installation, just copy all the files to a directory. Two text files dictate which sounds are found where:

config.txt
==========

This text file has one line and it tells where all the sound effects are stored. So this is the longest possible path to sound files that all files have in common. Usually something like:

D:\SFX\

data.txt
========

This text file lists all the WAV files and their descriptions. The first line is ignored, so it can be used for a small description of its purpose. A typical data file might look like this:

Data for sfx (this line is ignored).
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

info
====

Hens Zimmerman
henszimmerman@gmail.com

