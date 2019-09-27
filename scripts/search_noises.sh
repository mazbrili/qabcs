#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: searching for missing noises

pushd ..

silence=""

pics="`find abcs/all/pics -path abcs/all/pics/food -prune -o -name "*.*"|cut -d "/" -f 5|cut -d "." -f 1`"

for filename in $silence
do
  pics=`echo "$pics"|sed "s|^$filename$||g"`
  if [ -f "abcs/all/noises/$filename.ogg" ]
  then
    echo "Error: $filename"
  fi
done

found="false"

for noise in $pics
do
  # some words have noises from other words
  noise=`echo $noise|sed "s|york|dog|g"`
  noise=`echo $noise|sed "s|steam_locomotive|train|g"`
  noise=`echo $noise|sed "s|instrument|guitar|g"`
  noise=`echo $noise|sed "s|xenopus|frog|g"`
  noise=`echo $noise|sed "s|jellyfish|fish|g"`
  noise=`echo $noise|sed "s|shark|fish|g"`
  noise=`echo $noise|sed "s|misgurnus|fish|g"`
  noise=`echo $noise|sed "s|xiphias|fish|g"`
  noise=`echo $noise|sed "s|angelfish|fish|g"`
  noise=`echo $noise|sed "s|pike|fish|g"`
  if [ ! -f "abcs/all/noises/$noise.ogg" ]
  then
    echo $noise
    found="true"
  fi
done

popd

if [ "$found" = "false" ]
then
  echo "Nothing was found!"
fi
