#!/bin/bash
sync
cd "$(dirname $0)"

linuxcnc drehbank.ini || sleep 10
