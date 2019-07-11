#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: convert text and letters into .ogg (vorbis) using Yandex SpeechKit Cloud
# https://cloud.yandex.ru/docs/speechkit
# This script is not in use, just an example for information
# You need curl, ffmpeg installed

key="$1"
# mp3, wav, opus
format="wav"
# hi, lo (any quality is only for wav format), default is hi
quality="hi"
# ru-RU, en-US, uk-UK, tr-TR, default is ru-RU
lang="ru-RU"
# female: jane, oksana, alyss, omazh; male: zahar, ermil
speaker="omazh"
# 0.1 - 3.0, default is 1.0
speed="0.9"
# good, evil, neutral, default is neutral
emotion="neutral"
# abcs directory of your language
lang0="ru"
# output format
output="ogg"
# file format: properties or json
file_format="properties"

if [ -z "$key" ]
then
   echo "Parameter 'key' is mandatory!"
   exit 1
fi

curl=`curl -h`

if [ -z "$curl" ]
then
   echo "You have no curl installed!"
   exit 1
else
   echo "curl detected"
fi

ffmpeg=`ffmpeg -version`

if [ -z "$ffmpeg" ]
then
   echo "You have no ffmpeg installed!"
   exit 1
else
   echo "ffmpeg detected"
fi

if [ -f "../abcs/$lang0/abc.json" ]
then
if [ $file_format = "json" ]
then
words_list=`cat ../abcs/$lang0/abc.json|grep \"name\"|cut -d ":" --fields=2 |cut -d "\"" --fields=2|awk '{print tolower($0)}'|sed "s| |yyy|g"|sort -u`
letters_list=`cat ../abcs/$lang0/abc.json|grep -v espeak_words|grep -v visible|grep -v sound_letter|grep -v general|grep -v language |grep -v author|grep -v speak_method |grep -v espeak_params| grep -v letters | grep -v misc |grep -v pic | grep -v name |grep -v noises |grep -v food | grep -v animals | grep -v instrument | grep -v toys | grep -v inheritsFrom |awk '{print tolower($0)}'|sort -u | cut -d "\"" --fields=2|grep -v '\{'|grep -v '\}' | grep -v '\[' |grep -v '\]'| sed s/' '//g|grep -v "^$"`
fi
fi

if [ -f "../abcs/$lang0/abc.properties" ]
then
if [ $file_format = "properties" ]
then
words_list=`cat ../abcs/$lang0/abc.properties|grep -v language|grep -v visible| grep -v inheritsFrom | grep -v author|grep -v espeak_params| grep -v "#"|cut -d "=" --fields=2 |awk '{print tolower($0)}'|sed "s| |yyy|g"|sort -u`
letters_list=`cat ../abcs/$lang0/abc.properties|grep -v language|grep -v visible| grep -v inheritsFrom | grep -v author|grep -v espeak_params| grep -v "#"|cut -d "=" --fields=1 | cut -d ":" --fields=2 |awk '{print tolower($0)}'|sed "s| |yyy|g"|sort -u`
fi
fi

if [ -z "$words_list" ]
then
  echo "words_list is empty"
  exit 1
fi

if [ -z "$letters_list" ]
then
  echo "letters_list is empty"
  exit 1
fi

mkdir -p ../abcs/$lang0/sounds/words
rm -f ../abcs/$lang0/sounds/words/*.$format
rm -f ../abcs/$lang0/sounds/words/*.$output

for a in $words_list
do
filename=`echo "$a"|cut -d "=" --fields=1|sed 's|yyy| |'|sed 's| |_|'`
text=`echo "$a"|cut -d "=" --fields=1|sed 's|yyy| |'|sed 's|_| |'`
# adding + for stress, - for pause, fixing pronounce
text=`echo "$text"|sed "s|ёочин|+ёочин|g"`
text=`echo "$text"|sed "s|еху|йеху|g"`
text=`echo "$text"|sed "s|жаркое|жарк+ое|g"`
text=`echo "$text"|sed "s|иглу|+иглу|g"`
text=`echo "$text"|sed "s|ирга|ир-га|g"`
text=`echo "$text"|sed "s|кларнет|кларн+эт|g"`
text=`echo "$text"|sed "s|маракасы|мар+акасы|g"`
text=`echo "$text"|sed "s|орган|орг+ан|g"`
text=`echo "$text"|sed "s|подъёлочник|под-+ёлочник|g"`
text=`echo "$text"|sed "s|цуккини|цукини|g"`
text=`echo "$text"|sed "s|чангу|ч+ангу|g"`
text=`echo "$text"|sed "s|щавель|щав+ель|g"`
text=`echo "$text"|sed "s|ыых|ы-ых|g"`
text=`echo "$text"|sed "s|янцинь|ян-цинь|g"`
text=`echo "$text"|sed "s|йо-йо|йо-й+о|g"`
curl "https://tts.voicetech.yandex.net/generate?format=$format&lang=$lang&speaker=$speaker&emotion=$emotion&key=$key&quality=$quality&speed=$speed" -G --data-urlencode "text=$text" > ../abcs/$lang0/sounds/words/"$filename.$format"
ffmpeg -i ../abcs/$lang0/sounds/words/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/words/"$filename.$output"
done

mkdir -p ../abcs/$lang0/sounds/alpha
rm -f ../abcs/$lang0/sounds/alpha/*.$format
rm -f ../abcs/$lang0/sounds/alpha/*.$output

for a in $letters_list
do
filename="$a"
text="$a"
# fixing pronounce
text=`echo "$text"|sed "s|в|вэ|g"`
text=`echo "$text"|sed "s|и|e|g"`
text=`echo "$text"|sed "s|к|ка|g"`
text=`echo "$text"|sed "s|с|эс|g"`
text=`echo "$text"|sed "s|ъ|твёрдый знак|g"`
text=`echo "$text"|sed "s|ь|мягкий знак|g"`
curl "https://tts.voicetech.yandex.net/generate?format=$format&lang=$lang&speaker=$speaker&emotion=$emotion&key=$key&quality=$quality&speed=$speed" -G --data-urlencode "text=$text" > ../abcs/$lang0/sounds/alpha/"$filename.$format"
ffmpeg -i ../abcs/$lang0/sounds/alpha/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/alpha/"$filename.$output"
done

# sometimes using other languages instead of native because of better pronounce
lang="uk-UK"
filename="ы"
text="и"
rm -f ../abcs/$lang0/sounds/alpha/$filename.$format
rm -f ../abcs/$lang0/sounds/alpha/$filename.$output
curl "https://tts.voicetech.yandex.net/generate?format=$format&lang=$lang&speaker=$speaker&emotion=$emotion&key=$key&quality=$quality&speed=$speed" -G --data-urlencode "text=$text" > ../abcs/$lang0/sounds/alpha/"$filename.$format"
ffmpeg -i ../abcs/$lang0/sounds/alpha/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/alpha/"$filename.$output"
filename="у"
text="у"
rm -f ../abcs/$lang0/sounds/alpha/$filename.$format
rm -f ../abcs/$lang0/sounds/alpha/$filename.$output
curl "https://tts.voicetech.yandex.net/generate?format=$format&lang=$lang&speaker=$speaker&emotion=$emotion&key=$key&quality=$quality&speed=$speed" -G --data-urlencode "text=$text" > ../abcs/$lang0/sounds/alpha/"$filename.$format"
ffmpeg -i ../abcs/$lang0/sounds/alpha/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/alpha/"$filename.$output"
filename="иглу"
text="іґлу"
rm -f ../abcs/$lang0/sounds/words/$filename.$format
rm -f ../abcs/$lang0/sounds/words/$filename.$output
curl "https://tts.voicetech.yandex.net/generate?format=$format&lang=$lang&speaker=$speaker&emotion=$emotion&key=$key&quality=$quality&speed=$speed" -G --data-urlencode "text=$text" > ../abcs/$lang0/sounds/words/"$filename.$format"
ffmpeg -i ../abcs/$lang0/sounds/words/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/words/"$filename.$output"
filename="ы"
text="и"
rm -f ../abcs/$lang0/sounds/words/$filename.$format
rm -f ../abcs/$lang0/sounds/words/$filename.$output
curl "https://tts.voicetech.yandex.net/generate?format=$format&lang=$lang&speaker=$speaker&emotion=$emotion&key=$key&quality=$quality&speed=$speed" -G --data-urlencode "text=$text" > ../abcs/$lang0/sounds/words/"$filename.$format"
ffmpeg -i ../abcs/$lang0/sounds/words/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/words/"$filename.$output"
filename="ыых"
text="и-их"
rm -f ../abcs/$lang0/sounds/words/$filename.$format
rm -f ../abcs/$lang0/sounds/words/$filename.$output
curl "https://tts.voicetech.yandex.net/generate?format=$format&lang=$lang&speaker=$speaker&emotion=$emotion&key=$key&quality=$quality&speed=$speed" -G --data-urlencode "text=$text" > ../abcs/$lang0/sounds/words/"$filename.$format"
ffmpeg -i ../abcs/$lang0/sounds/words/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/words/"$filename.$output"

#clean up
rm -f ../abcs/$lang0/sounds/words/*.$format
rm -f ../abcs/$lang0/sounds/alpha/*.$format
