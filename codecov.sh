#!/bin/bash

BUILD_DIR=../build

function codecov {
  local folder=$1
  shift
  local target=$1
  shift
  local files=("$@")
  for filename in "${files[@]}";
  do
    local source="$folder/$filename"
    local notes="$BUILD_DIR/$folder/CMakeFiles/$target.dir/$filename.gcno"
    #echo "$source -> $notes"
    gcov-10 -o "$notes" "$source" > /dev/null
  done
}

cd src || exit
sources=(**/*.cxx)
cd ..
codecov src si "${sources[@]}"

cd test || exit
tests=(*.cxx)
cd ..
codecov test si_tests "${tests[@]}"

#for filename in ../**/*.[ch]xx;
#do
#  echo "$filename"
#  gcov-10 -n -o . "$filename" > /dev/null
#done
