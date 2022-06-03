# Lyrics finder

### Description
Lyrics finder is a program used to search for and store your favourite lyrics on tekstowo.pl. Program also allows you to search for songs on YT. 
### Requirements
- cmake 3.10 or newer
- msgedge.exe location must be added to PATH (C:\Program Files (x86)\Microsoft\Edge\Application is a deafult installation location)
- internnet connection is necessary.
### Build
To build and use lyrics finder you must:
- download latest release
- unpack folder
- open powershell in project direcotry and type:
```
mkdir build
cd build
cmake ..
cmake --build .
.\Debug\lyrics_finder.exe
```