#!/bin/bash

echo 'hostname:'
hostname
source /software/root/v5.34.25/bin/thisroot.sh 
python mergeATOutput.py -n $1
