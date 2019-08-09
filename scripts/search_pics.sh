#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: searching for pictures that must be added based on properties files

lang="$1"

if [ -z "$lang" ]
then
   echo "Parameter 'lang' is mandatory!"
   exit 1
fi

pushd ..

file="abcs/$lang/abc*.properties"

if [ -z `find abcs/$lang -name *.properties` ]
then
  popd
  echo "File abcs/$lang/abc*.properties does not exist!"
  exit 1
fi

for filename in `cat $file|grep -v ^language|grep -v ^espeak_params|grep -v ^visible|grep -v ^author|grep -v ^inheritsFrom|grep -v ^view_letters|grep -v ^speak_method|grep -v ^typing|grep -v ^#|grep -v '\[missing\]'|sed "s|:.*=.*=.*=|:|g"|sort|cut -d ":" -f 2`
do
  if [ -z `find abcs/all/pics -name "$filename.*"` ]
  then
    echo $filename
  fi
done

popd
