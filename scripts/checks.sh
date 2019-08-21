#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: check extra noises and pictures, that are not in use

pushd ..

tmpfile="tmpfile"
found="false"

rm -f $tmpfile

for file in `find abcs -name *.properties`
do
  if [ -z "`cat $file|grep "visible:false"`" ] || [ "$file" = "abcs/en/abc1.properties" ]
  then
    cat $file|grep -v '\[missing\]'|grep -v language|grep -v visible| grep -v inheritsFrom | grep -v author|grep -v espeak_params|grep -v view_letters|grep -v speak_method|grep -v typing| grep -v "#" >> $tmpfile
  fi
done

pics="`find abcs/all/pics -name "*.*"|cut -d "/" -f 5|cut -d "." -f 1`"
words="`cat $tmpfile|cut -d "=" -f 4`"

noises_f="`find abcs/all/noises -name "*.*"|cut -d "/" -f 4|cut -d "." -f 1`"
noises="`cat $tmpfile|cut -d "=" -f 5`"

for key in $pics
do
    if echo "$words" | grep --quiet --line-regexp --fixed-strings "${key}"
    then
      true
    else
      echo "Picture '$key' is not in use!"
      found="true"
    fi
done

for key in $noises_f
do
    if echo "$noises" | grep --quiet --line-regexp --fixed-strings "${key}"
    then
      true
    else
      echo "Noise '$key' is not in use!"
      found="true"
    fi
done

rm -f $tmpfile

popd

if [ $found = "false" ]
then
  echo "Everything is OK"
fi
