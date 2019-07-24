#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: searching for pictures that must be added based on properties file

file="abcs/nl/abc.properties"

pushd ..

for filename in `cat $file|grep -v ^language|grep -v ^author|grep -v ^#|sed "s|:.*=.*=.*=|:|g"|sort|cut -d ":" -f 2`
do
  if [ -z `find abcs/all/pics -name "$filename.*"` ]
  then
    echo $filename
  fi
done

popd
