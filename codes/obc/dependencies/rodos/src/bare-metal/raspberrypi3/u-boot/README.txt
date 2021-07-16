Die Datei(en) in diesen Verzeichnis:

config.txt:
    Minimalkonfiguration der RaspberryPi-Firmware

Kurzanleitung zur Übertragung von U-Boot-Abbildern über die UART-Schnittstelle mittels des Kermit-Protokolls:

Voraussetzungen:
- Programm: kermit (Debian-Paket 'ckermit')
- Inhalt der Datei '~/.kermrc' (falls noch nicht existiert => erstellen)

set line /dev/ttyUSB0
set speed 115200
set carrier-watch off
set prefixing all
set parity none
set stop-bits 1
set modem none
set file type bin
set file name lit
set flow-control none
set terminal lf-display crlf
set prompt "Linux Kermit> "

- evtl. den Namen der seriellen Schnittstelle ändern.

Laden eines neuen Images:
- Starten des kermit-Programms in einen (neuen) Terminalfenster
- Es erscheint eine Eingabeaufforderung mit 'Linux Kermit> ' am Anfang
- 'connect'
- Es erscheint die Eingabeaufforderung 'U-Boot> '
- 'loadb' (optional kann eine Speicheradresse angegeben werden an die das Image geladen werden soll)
- Zurück zu Kermit mittels (STRG + Backslash) und anschließend 'C'
- 'send u-boot-kernel7'
- 'connect'
- 'bootm' (optional wird hier die Speicheradresse angegeben, an der das Image im Speicher liegt)

