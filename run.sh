#!/bin/bash

if [[ $# < 3 ]] ; then
  echo -e "specify \033[1;34myear\033[0m, \033[1;34mday\033[0m and \033[1;34mmode (e/i)\033[0m to run"
  exit
fi

if ! [ -f "./build/$1_$2" ]; then
  echo "$1/$2 has not been compiled"
  bash comp.sh $1 $2
fi

echo -e "running \033[1;34m$1/$2/solution.cpp\033[0m"
cd $1/$2
../../build/$1_$2 $3 || exit 1