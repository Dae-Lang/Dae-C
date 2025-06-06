#!/usr/bin/bash

# compile

mkdir -p build && cd build

cmake ..
make

cd ..
cp build/DAC .

# run it at termux
if [ "$1" = "--termux" ]; then
  cp DAC $HOME
  rm DAC

  cd $HOME
  chmod +x DAC
  ./DAC main.dac
fi
