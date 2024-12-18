#!/bin/bash

if [[ $# < 2 ]] ; then
  echo -e "specify \033[1;34myear\033[0m and \033[1;34mday\033[0m to init"
  exit
fi

if [ -d "$1/$2" ]; then
  echo "$1/$2 is already initialized"
  exit
fi

if ! [ -d "$1" ]; then
  mkdir $1
fi

mkdir $1/$2
cp ./utils/template.cpp ./$1/$2/solution.cpp
cd $1/$2
touch example.txt
touch input.txt
echo "initialized $1/$2"