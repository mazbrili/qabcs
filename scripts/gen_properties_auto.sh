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
       # is not full
       abc="aáäbcčdďeéfghiíjklĺľmnňoóôpqrŕsštťuúvwxyýzž"
       ;;
     pt)
       abc="abcdefghijklmnopqrstuvwxyz"
       ;;
     es)
       abc="abcdefghijklmnñopqrstuvwxyz"
       ;;
     pl)
       abc="aąbcćdeęfghijklłmnńoópqrsśtuvwxyzźż"
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
     chk)
       abc="abcdefghijklmnopqrstuvwxyz"
       ;;
     *)
       echo "Unknown language!"
       exit 1
       ;;
esac

file="langs/qabcs_$lang.properties"
result_file="abcs/$lang/abc.properties.auto"

if [ "$lang" = "en_gb" ]
then
  file="langs/qabcs_en.properties"
fi

pushd ..

rm -rf abcs/all/pics/dummy

# create dummy pictures
for filename in `cat langs/qabcs_en.properties|cut -d "=" -f 1`
do
  if [ -z `find abcs/all/pics -name "$filename.*"` ]
  then
    mkdir -p abcs/all/pics/dummy
    touch abcs/all/pics/dummy/$filename.png
  fi
done

# get native language name from langs.json
declare -c language=`awk -F=":" -v RS="}" '$1'"~/\"$lang\"/ {print}" langs/langs.json|grep nativeName|cut -d ":" -f 2|cut -d "\"" -f 2`

if [ ! -f "$file" ]
then
  echo "Missing $file"
  popd
  exit 1
fi

mkdir -p abcs/$lang
rm -f $result_file

# get directories names/categories names
for dir in `dir abcs/all/pics`
do
  # get filenames, they are keys in properties file
  for filename in `dir -1 abcs/all/pics/$dir|cut -d "." -f 1`
  do
    # get translated words in lowercase
    word=`cat $file|grep "^$filename="|cut -d "=" -f 2|sed "s|_| |g"|awk '{print tolower($0)}'`
    if [ ! -z "$word" ]
    then
      # get first letter of word
      letter=`echo $word|cut -c 1`
      # some words have noises from other words
      noise=`echo $filename|sed "s|york|dog|g"|sed "s|yi|train|g"`
      if [ -f "abcs/all/noises/$noise.ogg" ]
      then
        # if noise was found
        echo "$dir:$letter=$word==$filename=$noise" >> $result_file
      else
        # if noise was not found
        echo "$dir:$letter=$word==$filename" >> $result_file
      fi
    fi
  done
done

# sort file by modify it on 2 columns and sort by 2nd column, modify back on using 1 column
cat $result_file|sed "s|:|: |g"|sort -k 2|sed "s|: |:|g" > $result_file.sorted

rm -f $result_file

# for all letters of abc
for ((i=0; $i<${#abc}; i=$(($i+1))))
do
  # get letter
  letter=${abc:$i:1}
  # write letter with comment in resulted file
  echo "#$letter" >> $result_file
  # get directories names/categories names
  for dir in `dir abcs/all/pics`
  do
    # get all strings that have category with letter
    a=`cat $result_file.sorted|grep "$dir:$letter="`
    if [ ! -z "$a" ]
    then
      # if string was found
      echo "$a" >> $result_file
      # delete string from file
      sed -i "/$dir:$letter=/d" $result_file.sorted
    else
      # if string was not found
      echo "$dir:$letter=[missing]" >> $result_file
    fi
  done
done

a=`cat $result_file.sorted`

# if file still has strings, it means they started with unknown letter
if [ ! -z "$a" ]
then
  echo "#unknown" >> $result_file
  echo "$a" >> $result_file
fi

# add author field in head of resulted file
sed -i -e "1 s/^/author:Name <email>\n/;" $result_file
# add language field in head of resulted file
sed -i -e "1 s/^/language:$language\n/;" $result_file

# language does not have instrument names
if [ "$lang" = "chk" ]
then
  sed -i "/instrument:/d" $result_file
fi

rm -f $result_file.sorted

# delete dummy pictures
rm -rf abcs/all/pics/dummy

popd

echo "File $result_file was generated."
