
# Installation

## alter Rechner ca 2015
Rechner und Linux-Einstellungen für den damaligen Rechner von ca 2015:
Intel Core 2 Quad
Linuxcnc 2.6 Debian wheezy image
Bootoptionen: isolcpus=1,2,3 idle=poll
( eventuell i915.modeset=0; wahrscheinlich damals wieder deaktiviert )
Sound deaktiviert
Parport 378 EPP

## neuer Rechner 2025

Hardware:
- PC
- MESA-Karte 6i25 (in LinuxCNC als 5i25 bezeichnet!)
- Details siehe https://github.com/fau-fablab/drehbank-schaltplan/

BIOS:
- TODO... alles ausschalten das irgendwie mit Energiesparen zu tun hat oder unnütze Hardware ist.
- Secure Boot aus

Installation:
- Linuxcnc 2.9.4 (inkl. Betriebssystem) ISO von Webseite https://www.linuxcnc.org/iso/linuxcnc_2.9.4-amd64.hybrid.iso auf USB-Stick
- Aus irgendwelchen Gründen wollte der Installer den Bootloader nicht installieren - Workaround war ungefähr folgendes: Strg-Alt-F2, mit Enter Shell öffnen, chroot /target; mount /boot/efi -o remount,rw; apt update; apt install --reinstall grub-efi-amd64*; exit; exit 
- Benutzername user / Passwort user
- Latency Test laufen lassen, auch mit glxgears usw (siehe LinuxCNC Doku und Wiki)
- TODO weiter frickeln bis Jitter Main Task < 0,1 ms (10% Takt), besser deutlich weniger (Rechner von 2015 hatte 7 µs)
- Grub Parameter: GRUB_CMDLINE_LINUX="isolcpus=1-7 intel_pstate=disable processor.max_cstate=0 idle=poll cpufreq.default_governor=performance ahci.mobile_lpm_policy=1 irqaffinity=0"
--> bissle besser aber trotzdem noch große Glitches :-(
mit nomodeset immer noch 0.5ms jitter :-(
das hier teilweise probiert, hat auch nix gebracht: https://dantalion.nl/2024/09/29/linuxcnc-latency-jitter-kernel-parameter-tuning.html
- TODO: Dateifreigabe...
- TODO: Key für Git
- Config laden
```
cd ~/linuxcnc/configs/
git clone git@github.com:fau-fablab/linuxcnc-drehbank-config.git
cp linuxcnc-drehbank-config/Drehbank.desktop ~/Desktop
```


## Installation in VM
z.B. in Virtualbox.

- Vorgehen ebenso wie für Rechner
- in "drehbank.ini" dann die Include´ für "Realität" auskommentieren und dafür die Include für "Simulation" aktivieren:
```
; #INCLUDE ini_parts/41_hal_realitaet.inc
#INCLUDE ini_parts/41_hal_simulation.inc.disabled
```
(die Dateiendung .disabled macht keinen Unterschied, ist nur zur Klarstellung dass es normalerweise deaktiviert ist)

# Starten
mittels run.sh bzw. der Verknüpfung auf dem Desktop

# Config
Ausgangspunkt der Config ist "drehbank.ini". Von dort aus werden alle Teile eingebunden. Um die Konfiguration zu verstehen sollte man dort anfangen und sich schrittweise entlanghangeln.

Die Konfiguration ist möglichst modular in viele Teildateien aufgeteilt. Durch Ein-/Auskommentieren in config.ini kann man zwischen realer HW und einer Simulation (zum Ausprobieren von Änderungen/Upgrades in einer VM) wechseln. Ebenso kann man die GUI umschalten; funktionsfähig ist aber nur die "gute alte" AXIS UI die nach Steinzeit aussieht aber funktioniert.

# TODO / BUGS

Jumper an 7i85 und 6i25 umstellen, sodass Versorgung vom Schaltschrank statt vom Host aus

TODO:
Nothalt wenn Vfield wegfällt - da kommt grad nur "smart serial comm error" :(
--> nochmal prüfen, laut Config sollte es eigentlich einen SW Watchdog geben

BUGS:
hässliche UI (axis) kann noch nicht durch hübsche (gmoccapy) ersetzt werden, weil die nach der Referenzfahrt immer noch denkt, sie sei nicht referenziert - irgendwo fällt ein not_all_homed event raus.

WONTFIX:
Herumfahren während ein Job pausiert ist geht nicht (kann linuxcnc stable nicht. Dafür gibt es zwar patches, Stichwort jog-while-pause, aber es gibt wichtigeres zum Zeitvertreib als das auszuprobieren.)





-----------------------
richtige Firmware für mesa Karte laden:

# 5i25.zip von mesa Webseite http://www.mesanet.com/software/parallel/5i25.zip

unzip 5i25.zip
cd 5i25/utils/linux
sudo -i
# VARIANTE=5i25_7i76x2 # alt (vor Ergänzung der Glasmaßstäbe)
# VARIANTE=5i25_7i76_7i85 # neu seit 2025 
# BUG: wenn --device nicht angegeben wird, macht er stillschweigend nichts
./mesaflash --device 5i25 --write ../../configs/hostmot2/$VARIANTE.bit 
./mesaflash --verify ../../configs/hostmot2/$VARIANTE.bit 

reboot


pncconf für testconfig...
Obacht: Spannung Vfield muss an sein beim Start, sonst wird FPGA Karte nicht gefunden


-----------

Testconfig erstellen mit pncconf:
(sollte in Zukunft nicht nötig sein, wurde nur gebraucht um erstmalig auszuprobieren wie die Config aussehen muss)

pncconf hat ab Werk keine Auswahl für unsere Kombi 5i25-7i76-7i85

(unvollständige Infos gab es hier.
https://forum.linuxcnc.org/39-pncconf/21670-5i25-firmware-xml-files-for-pncconf
TODO - dort antworten
)

Korrektes Vorgehen(?):
Diese Datei
https://www.mesanet.com/software/parallel/5i25.zip
entpacken und den Inhalt von 
5i25/configs/hostmot2/
nach /lib/firmware/hm2/5i25/ kopieren:
sudo cp ~u/Downloads/5i25/configs/hostmot2/*.* /lib/firmware/hm2/5i25

dann pncconf starten
im Abschnitt MESA unter board name 5i25 (ganz oben) statt 5i25InternalData auswählen
dann kann man die gewünschte 7i76-7i85 Kombi auswählen

-----




