#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: get translated words from properties files
# You need this script to convert already translated properties files into
# JAVA UTF8 format and upload resulted file into transifex

lang="$1"

if [ -z "$lang" ]
then
   echo "Parameter 'lang' is mandatory!"
   exit 1
fi

pushd ..

rm -f langs/$lang.properties.tmp
rm -f langs/$lang.properties

for dir in `dir abcs/all/pics`
do
  cat abcs/$lang/abc*.properties|grep "$dir:" >> langs/$lang.properties.tmp
done

while read line
do
  key=`echo $line|cut -d "=" -f 4`
  word=`echo $line|cut -d "=" -f 2|sed "s|_| |g"|awk '{print tolower($0)}'`
  if [ -z "$key" ]
  then
    rm -f langs/$lang.properties.tmp
    rm -f langs/$lang.properties
    popd
    echo "Syntax error in line: $line"
    exit 1
  fi
  echo "$key=$word" >> langs/$lang.properties
done < langs/$lang.properties.tmp

sed -i "/^=$/d" langs/$lang.properties

rm -f langs/$lang.properties.tmp

popd

echo "File langs/$lang.properties was written!"
