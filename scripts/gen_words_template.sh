#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: get words to be translated by transifex based on picture names (JAVA UTF8 format)

pushd ..

rm -f langs/qabcs_en.properties

for dir in `dir abcs/all/pics`
do
  for filename in `dir -1 abcs/all/pics/$dir|cut -d "." -f 1`
  do
    word=`echo $filename|sed "s|_| |g"`
    echo "$filename=$word" >> langs/qabcs_en.properties
  done
done

cat langs/qabcs_en.properties|sort -u > langs/qabcs_en_tmp.properties
mv -f langs/qabcs_en_tmp.properties langs/qabcs_en.properties

popd

echo "File langs/qabcs_en.properties was written!"
