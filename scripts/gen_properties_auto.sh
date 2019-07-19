#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: generate properties file automatically

lang="ru"

case $lang in
     en*)
       abc="abcdefghijklmnopqrstuvwxyz"
       ;;
     it)
       abc="abcdefghijklmnopqrstuvwxyz"
       ;;
     tr)
       abc="abcçdefgğhıijklmnoöprsştuüvyz"
       ;;
     sv)
       abc="abcdefghijklmnopqrstuvwxyzåäö"
       ;;
     sk)
       abc="aáäbcčdďeéfghiíjklĺľmnňoóôpqrŕsštťuúvwxyýzž"
       ;;
     pt)
       abc="abcdefghijklmnopqrstuvwxyz"
       ;;
     es)
       abc="abcdefghijklmnñopqrstuvwxyz"
       ;;
     pl)
       abc="aąbcćdeęfghijklłmnńoóprsśtuwyzźż"
       ;;
     nb)
       abc="abcdefghijklmnopqrstuvwxyzæøå"
       ;;
     fr)
       abc="abcdefghijklmnopqrstuvwxyz"
       ;;
     nl)
       abc="abcdefghijklmnopqrstuvwxyz"
       ;;
     da)
       abc="abcdefghijklmnopqrstuvwxyzæøå"
       ;;
     ru)
       abc="абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
       ;;
     uk)
       abc="абвгґдеєжзиіїйклмнопрстуфхцчшщьюя"
       ;;
     de)
       abc="aäbcdefghijklmnoöpqrsßtuüvwxyz"
       ;;
esac

file="langs/qabcs_$lang.properties"
result_file="abcs/$lang/abc.properties.auto"

if [ "$lang" = "en_gb" ]
then
  file="langs/qabcs_en.properties"
fi

pushd ..

declare -c language=`awk -F=":" -v RS="}" '$1'"~/\"$lang\"/ {print}" langs/langs.json|grep nativeName|cut -d ":" -f 2|cut -d "\"" -f 2`

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
    sed -i "/$dir:$letter=/d" $result_file.sorted
    if [ ! -z "$a" ]
    then
      echo "$a" >> $result_file
    else
      echo "$dir:$letter=[missing]" >> $result_file
    fi
  done
done

a=`cat $result_file.sorted`

if [ ! -z "$a" ]
then
  echo "#unknown" >> $result_file
  echo "$a" >> $result_file
fi

sed -i -e "1 s/^/author:Name <email>\n/;" $result_file
sed -i -e "1 s/^/language:$language\n/;" $result_file

rm -f $result_file.sorted

popd

echo "File $result_file was generated."
