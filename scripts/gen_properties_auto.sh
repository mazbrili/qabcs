#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: generate properties file automatically

lang="ru"

case $lang in
     en*)
       abc="abcdefghijklmnopqrstuvwxyz"
       language="English"
       ;;
     ru)
       abc="абвгдежзийклмнопрстуфхцчшщъыьэюя"
       language="Русский"
       ;;
     uk)
       abc="абвгґдеєжзиіїйклмнопрстуфхцчшщьюя"
       language="Українська"
       ;;
     de)
       abc="aäbcdefghijklmnoöpqrsßtuüvwxyz"
       language="Deutsch"
       ;;
esac

file="langs/qabcs_$lang.properties"
result_file="abcs/$lang/abc.properties.auto"

if [ "$lang" = "en_gb" ]
then
  file="langs/qabcs_en.properties"
fi

pushd ..

if [ ! -f "$file" ]
then
  echo "Missing $file"
  popd
  exit 1
fi

mkdir -p abcs/$lang
rm -f $result_file

for dir in `dir abcs/all/pics`
do
  for filename in `dir -1 abcs/all/pics/$dir|cut -d "." -f 1`
  do
    word=`cat $file|grep "^$filename="|cut -d "=" -f 2|sed "s|_| |g"|awk '{print tolower($0)}'`
    if [ ! -z "$word" ]
    then
      letter=`echo $word|cut -c 1`
      noise=`echo $filename|sed "s|york|dog|g"|sed "s|yi|train|g"`
      if [ -f "abcs/all/noises/$noise.ogg" ]
      then
        echo "$dir:$letter=$word==$filename=$noise" >> $result_file
      else
        echo "$dir:$letter=$word==$filename" >> $result_file
      fi
    fi
  done
done

cat $result_file|sed "s|:|: |g"|sort -k 2|sed "s|: |:|g" > $result_file.sorted

rm -f $result_file

for ((i=0; $i<${#abc}; i=$(($i+1))))
do
  letter=${abc:$i:1}
  echo "#$letter" >> $result_file
  for dir in `dir abcs/all/pics`
  do
    a=`cat $result_file.sorted|grep "$dir:$letter="`
    if [ ! -z "$a" ]
    then
      echo "$a" >> $result_file
    else
      echo "$dir:$letter=[missing]" >> $result_file
    fi
  done
done

sed -i -e "1 s/^/author:Name <email>\n/;" $result_file
sed -i -e "1 s/^/language:$language\n/;" $result_file

rm -f $result_file.sorted

popd

echo "File $result_file was generated."
