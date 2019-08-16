#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: adding noises in properties and properties.auto files automatically

pushd ..

file="$1"

if [ -z "$file" ]
then
  echo "Adding noises automatically for all files..."
  files=`find abcs -name abc*.properties*`
else
  if [ -f "$file" ]
  then
    echo "Adding noises automatically for $file..."
    files=$file
  else
    popd
    echo "File $file does not exists!"
    exit 1
  fi
fi

for file in $files
do
  if [ -z "$1" ]
  then
    echo "Working on $file..."
  fi
  for line in `cat $file|grep "="|cut -d "=" -f 4,5`
  do
    word=`echo $line|cut -d "=" -f 1`
    noise=$word
    # some words have noises from other words
    noise=`echo $noise|sed "s|york|dog|g"`
    noise=`echo $noise|sed "s|steam_locomotive|train|g"`
    noise=`echo $noise|sed "s|instrument|guitar|g"`
    if [ -f "abcs/all/noises/$noise.ogg" ]
    then
      # if noise was found
      if [ ! "$line" = "$word=$noise" ]
      then
        echo "Adding noise '$noise' for word '$word' to file $file"
        sed -i "s|=$word$|=$word=$noise|g" $file
      fi
    fi
  done
done

popd

echo "Adding noises was done."
