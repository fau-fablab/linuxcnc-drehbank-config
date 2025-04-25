
###############################################################################
# Hardware Abstraction Layer section
###############################################################################
[HAL]
# doppelte loadrt-Kommandos zusammenfassen:
TWOPASS=on
#,verbose,nodelete

HALUI = halui
# The run script first uses halcmd to execute any HALFILE
# files, and then to execute any individual HALCMD commands.
#
# list of hal config files to run through halcmd
#+ files are executed in the order in which they appear

# interne Verdrahtung und net-Benennung, die dann entweder mit der MESA-Karte oder mit der Simulation verbunden wird
HALFILE = hal/kinematik.hal
HALFILE = hal/spindle.hal
HALFILE = hal/endschalter.hal
HALFILE = hal/estop.hal
