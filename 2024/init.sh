#!/bin/bash

if [[ $# < 1 ]] ; then
  echo "specify folder to init"
  exit
fi

if [ -d "$1" ]; then
  echo "folder $1 already exists"
  exit
fi

mkdir $1
cp solution.cpp ./$1
cd $1
touch example.txt
touch input.txt
echo "initialized folder $1"