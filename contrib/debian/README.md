
Debian
====================
This directory contains files used to package lemoncoind/lemoncoin-qt
for Debian-based Linux systems. If you compile lemoncoind/lemoncoin-qt yourself, there are some useful files here.

## lemoncoin: URI support ##


lemoncoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install lemoncoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your lemoncoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcoin128.png` to `/usr/share/pixmaps`

lemoncoin-qt.protocol (KDE)

