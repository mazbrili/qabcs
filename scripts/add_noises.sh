#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: adding noises in properties and properties.auto files automatically

echo "Adding noises automatically..."

pushd ..

for file in `find abcs -name abc.properties*`
do
  echo "Working on $file..."
  for line in `cat $file|grep "="|cut -d "=" -f 4,5`
  do
    word=`echo $line|cut -d "=" -f 1`
    # some words have noises from other words
    noise=`echo $word|sed "s|york|dog|g"|sed "s|yi|train|g"`
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

echo "Done."
