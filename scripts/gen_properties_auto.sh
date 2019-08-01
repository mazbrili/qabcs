#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: generate properties file automatically

# Digraphs:
# "0" is equal with "ch"
# "1" is equal with "dz"
# "2" is equal with "dž"
# "3" is equal with "mw"
# "4" is equal with "ng"
# "5" is equal with "pw"

lang="$1"

if [ -z "$lang" ]
then
   echo "Parameter 'lang' is mandatory!"
   exit 1
fi

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
       abc="aáäbcčdď12eéfgh0iíjklĺľmnňoóôpqrŕsštťuúvwxyýzž"
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
     chk)
       abc="aáeéioóuúfskm3n4p5r0twy"
       ;;
     cs)
       abc="aábcčdďeéěfgh0iíjklmnňoópqrřsštťuúůvwxyýzž"
       ;;
     ch)
       abc="'aåb0defghiklmnñ4oprstuy"
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
  popd
  echo "Missing $file"
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
      noise=$filename
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

# for all letters of abc
for ((i=0; $i<${#abc}; i=$(($i+1))))
do
  # get letter
  letter=${abc:$i:1}
  # digraph support
  case $letter in
       0)
         sed -i "s|:c=ch|:$letter=$letter|g" $result_file.sorted
         ;;
       1)
         sed -i "s|:d=dz|:$letter=$letter|g" $result_file.sorted
         ;;
       2)
         sed -i "s|:d=dž|:$letter=$letter|g" $result_file.sorted
         ;;
       3)
         sed -i "s|:m=mw|:$letter=$letter|g" $result_file.sorted
         ;;
       4)
         sed -i "s|:n=ng|:$letter=$letter|g" $result_file.sorted
         ;;
       5)
         sed -i "s|:p=pw|:$letter=$letter|g" $result_file.sorted
         ;;
  esac
done

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
      if [ ! "$dir" = "dummy" ]
      then
        echo "$dir:$letter=[missing]" >> $result_file
      fi
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

# for all letters of abc
for ((i=0; $i<${#abc}; i=$(($i+1))))
do
  # get letter
  letter=${abc:$i:1}
  # digraph support
  case $letter in
       0)
         sed -i "s|$letter|ch|g" $result_file
         ;;
       1)
         sed -i "s|$letter|dz|g" $result_file
         ;;
       2)
         sed -i "s|$letter|dž|g" $result_file
         ;;
       3)
         sed -i "s|$letter|mw|g" $result_file
         ;;
       4)
         sed -i "s|$letter|ng|g" $result_file
         ;;
       5)
         sed -i "s|$letter|pw|g" $result_file
         ;;
  esac
done

rm -f $result_file.sorted

# delete dummy pictures
rm -rf abcs/all/pics/dummy

popd

echo "Correcting same noises if needed..."
./add_noises.sh $result_file

echo "File $result_file was generated."
