Hexen 2 Selection Menu in SDL2
=============================

Hexen 2 Selection Menu to choose class and difficulty for mods with ease on handhelds or RetroDeck with a controller.  Uses SDL2.

## Dependencies
### Libraries
- SDL2
- SDL2_ttf

## Ubuntu / Debian
```
sudo apt install libsdl2-dev libsdl2-ttf-dev
```

## Compiling
```sh
git clone https://github.com/Slayer366/glh2menu
cd glh2menu
make
```

Since the focus of this menu is to make it easier to select class and difficulty on devices that may not have access to a keyboard, some environment variables are expected to be set first:

- DEVICE_ARCH
- DISPLAY_WIDTH
- DISPLAY_HEIGHT
- ADDLPARAMS
- RUNMOD

## Example for using the menu with the Fortress of Four Doors mod on an ARM64 machine:
```
DEVICE_ARCH=aarch64 DISPLAY_WIDTH=640 DISPLAY_HEIGHT=480 ADDLPARAMS="+r_waterwarp 0" RUNMOD="-game fo4d +map hexn1" ./glh2menu.${DEVICE_ARCH}      
```
