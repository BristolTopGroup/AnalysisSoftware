#!/bin/bash

echo 'hostname:'
hostname
source /software/root/v6.02.04/bin/thisroot.sh
python mergeATOutput.py -n $1
