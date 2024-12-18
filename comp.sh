#!/bin/bash

if [[ $# < 2 ]] ; then
  echo -e "compiling \033[1;34mutils\033[0m"
  g++ -O3 -I./utils -c ./utils/utils.cpp -o ./build/utils.o
  exit
fi

if ! [ -d "$1/$2" ]; then
  echo "$1/$2 does not exist"
  exit
fi

echo -e "compiling \033[1;34m$1/$2/solution.cpp\033[0m"
g++ -O3 -I./utils ./build/utils.o ./$1/$2/solution.cpp -o ./build/$1_$2