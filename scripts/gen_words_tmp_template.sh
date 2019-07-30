#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: get words to be translated by transifex based on resulted properties file (JAVA UTF8 format)
# if pictures are not available yet, otherwise use gen_words_template.sh script

lang="de"

pushd ..

file="abcs/$lang/abc.properties"

rm -f langs/qabcs_en_tmp.properties

for filename in `cat $file|grep -v ^language|grep -v ^author|grep -v ^#|grep -v '\[missing\]'|sed "s|:.*=.*=.*=|:|g"|sort|cut -d ":" -f 2; cat langs/qabcs_en.properties|cut -d "=" -f 1`
do
  word=`echo $filename|sed "s|_| |g"`
  echo "$filename=$word" >> langs/qabcs_en_tmp.properties
done

cat langs/qabcs_en_tmp.properties|sort -u > langs/qabcs_en_tmp0.properties
mv -f langs/qabcs_en_tmp0.properties langs/qabcs_en_tmp.properties
mv -f langs/qabcs_en_tmp.properties langs/qabcs_en.properties

popd

echo "File langs/qabcs_en.properties was written!"
