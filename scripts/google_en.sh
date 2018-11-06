#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: convert text and letters into .ogg using Google Cloud Text-to-Speech
# https://cloud.google.com/text-to-speech/docs
# WARNING: you need curl, ffmpeg installed

key="$1"
# https://cloud.google.com/text-to-speech/docs/voices
languageCode="en-US"
# https://cloud.google.com/text-to-speech/docs/voices
name="en-US-Standard-E"
# https://cloud.google.com/text-to-speech/docs/voices
ssmlGender="FEMALE"
# FLAC, LINEAR16, MULAW, AMR, AMR_WB, OGG_OPUS, SPEEX_WITH_HEADER_BYTE (https://cloud.google.com/speech-to-text/docs/encoding)
audioEncoding="LINEAR16"
# WAV for LINEAR16 and MULAW
format="wav"
# output format
output="ogg"
# abcs directory of your language
lang0="en"

if [ -z "$key" ]
then
   echo "Parameter 'key' is mandatory!"
   exit 1
fi

if [ -f "../abcs/$lang0/abc.json" ]
then
words_list=`cat ../abcs/$lang0/abc.json|grep \"name\"|cut -d ":" --fields=2 |cut -d "\"" --fields=2|awk '{print tolower($0)}'|sed "s| |yyy|g"|sort -u`
letters_list=`cat ../abcs/$lang0/abc.json|grep -v espeak_words|grep -v visible|grep -v sound_letter|grep -v general|grep -v language |grep -v author|grep -v speak_method |grep -v espeak_params| grep -v letters | grep -v misc |grep -v pic | grep -v name |grep -v noises |grep -v food | grep -v animals | grep -v instrument | grep -v toys | grep -v inheritsFrom |awk '{print tolower($0)}'|sort -u | cut -d "\"" --fields=2|grep -v '\{'|grep -v '\}' | grep -v '\[' |grep -v '\]'| sed s/' '//g|grep -v "^$"`
fi

if [ -f "../abcs/$lang0/abc.properties" ]
then
words_list=`cat ../abcs/$lang0/abc.properties|grep -v language| grep -v inheritsFrom | grep -v author|grep -v espeak_params| grep -v "#"|cut -d "=" --fields=2 |awk '{print tolower($0)}'|sed "s| |yyy|g"|sort -u`
letters_list=`cat ../abcs/$lang0/abc.properties|grep -v language| grep -v inheritsFrom | grep -v author|grep -v espeak_params| grep -v "#"|cut -d "=" --fields=1 | cut -d ":" --fields=2 |awk '{print tolower($0)}'|sed "s| |yyy|g"|sort -u`
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
text=`echo "$a"|cut -d "=" --fields=1|sed 's|yyy| |'`
# fixing pronounce
text=`echo "$text"|sed "s|ipu|eepoo|g"`
curl -H "X-Goog-Api-Key: $key" \
  -H "Content-Type: application/json; charset=utf-8" \
  --data "{
    'input':{
      'text':'$text'
    },
    'voice':{
      'languageCode':'$languageCode',
      'name':'$name',
      'ssmlGender':'$ssmlGender'
    },
    'audioConfig':{
      'audioEncoding':'$audioEncoding'
    }
  }" "https://texttospeech.googleapis.com/v1beta1/text:synthesize" > synthesize-text.txt
echo `cat synthesize-text.txt | grep audioContent|cut -d ":" --fields=2|cut -d "\"" --fields=2` > synthesize-output-base64.txt
base64 synthesize-output-base64.txt --decode > ../abcs/$lang0/sounds/words/"$filename.$format"
rm -f synthesize-text.txt synthesize-output-base64.txt
ffmpeg -i ../abcs/$lang0/sounds/words/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/words/"$filename.$output"
done

mkdir -p ../abcs/$lang0/sounds/alpha
rm -f ../abcs/$lang0/sounds/alpha/*.$format
rm -f ../abcs/$lang0/sounds/alpha/*.$output

for a in $letters_list
do
filename="$a"
text="$a"
curl -H "X-Goog-Api-Key: $key" \
  -H "Content-Type: application/json; charset=utf-8" \
  --data "{
    'input':{
      'text':'$text'
    },
    'voice':{
      'languageCode':'$languageCode',
      'name':'$name',
      'ssmlGender':'$ssmlGender'
    },
    'audioConfig':{
      'audioEncoding':'$audioEncoding'
    }
  }" "https://texttospeech.googleapis.com/v1beta1/text:synthesize" > synthesize-text.txt
echo `cat synthesize-text.txt | grep audioContent|cut -d ":" --fields=2|cut -d "\"" --fields=2` > synthesize-output-base64.txt
base64 synthesize-output-base64.txt --decode > ../abcs/$lang0/sounds/alpha/"$filename.$format"
rm -f synthesize-text.txt synthesize-output-base64.txt
ffmpeg -i ../abcs/$lang0/sounds/alpha/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/alpha/"$filename.$output"
done

#clean up
rm -f ../abcs/$lang0/sounds/words/*.$format
rm -f ../abcs/$lang0/sounds/alpha/*.$format
