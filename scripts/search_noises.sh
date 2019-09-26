#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: searching for missing noises

pushd ..

silence="paper_airplane shield glove magnet quilt firebird cabin austrian_flag joker spider origami estonian_flag necklace watercolor easel lunchbox anklet kinder_surprise_egg paint ear button pen hat star jar magnifying_glass security_camera camera_lens earth vase grommet pupil display igloo jack quarter_note jellyfish python snail urchin shark moth fish misgurnus xiphias angelfish turtle pike kite yinyang"

for filename in `find abcs/all/pics -path abcs/all/pics/food -prune -o -name "*.*"|cut -d "/" -f 5|cut -d "." -f 1`
do
  noise=$filename
  # some words have noises from other words
  noise=`echo $noise|sed "s|york|dog|g"`
  noise=`echo $noise|sed "s|steam_locomotive|train|g"`
  noise=`echo $noise|sed "s|instrument|guitar|g"`
  noise=`echo $noise|sed "s|xenopus|frog|g"`
  for a in $silence
  do
    noise=`echo $noise|sed "s|^$a$||g"`
  done
  if [ ! -f "abcs/all/noises/$noise.ogg" ] && [ ! -z "$noise" ]
  then
    echo $noise
  fi
  if [ -f "abcs/all/noises/$filename.ogg" ] && [ -z "$noise" ]
  then
    echo "Error: $filename"
  fi
done

popd
