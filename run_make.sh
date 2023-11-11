#!/bin/sh

if [[ "$1" == "gba_driver" ]]; then
  make -f ./Makefile
elif [[ "$1" == "cpu_driver" ]]; then
  make -f ./makefile
fi

