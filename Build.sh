#!/bin/bash

function compile() {
    BOARD=$1
    OUT=$2
	uecide --headless --board=$BOARD --compile .
    mv MZBoot.hex "Builds/$OUT"
}


compile chipkit-promz-boot chipKIT_Pro-MZ.hex
compile marek-akai-boot Marke_Akai-Sampler.hex: 
