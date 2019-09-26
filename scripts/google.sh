#/bin/sh
# Author: Alexey Loginov
# Licence: GPLv3+
# Description: convert text and letters into .ogg (vorbis) using Google Cloud Text-to-Speech or Text-to-Speech
# https://cloud.google.com/text-to-speech/docs
# You need curl, ffmpeg or wget installed

# https://cloud.google.com/text-to-speech/docs/voices
#languageCode="en-US"
# https://cloud.google.com/text-to-speech/docs/voices
#name="en-US-Standard-E"
# https://cloud.google.com/text-to-speech/docs/voices
#ssmlGender="FEMALE"
# FLAC, LINEAR16, MULAW, AMR, AMR_WB, OGG_OPUS, SPEEX_WITH_HEADER_BYTE (https://cloud.google.com/speech-to-text/docs/encoding)
#audioEncoding="LINEAR16"
# WAV for LINEAR16 and MULAW
#format="wav"
# output format
#output="ogg"
# abcs directory of your language
#lang0="en"
# file format: properties or json
#file_format="properties"
# using Google Cloud Text-to-Speech API or not
#api=true

lang="$1"

if [ -z "$lang" ]
then
   echo "Parameter 'lang' is mandatory!"
   exit 1
fi

key="$2"

case $lang in
     en)
       languageCode="en-US"
       name="en-US-Standard-E"
       ssmlGender="FEMALE"
       api=true
       ;;
     el)
       languageCode="el-GR"
       name="el-GR-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     cs)
       languageCode="cs-CZ"
       name="cs-CZ-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     he)
       api=false
       ;;
     is)
       api=false
       ;;
     nl)
       languageCode="nl-NL"
       name="nl-NL-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     ru)
       languageCode="ru-RU"
       name="ru-RU-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     uk)
       languageCode="uk-UA"
       name="uk-UA-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     de)
       languageCode="de-DE"
       name="de-DE-Standard-B"
       ssmlGender="MALE"
       api=true
       ;;
     da)
       languageCode="da-DK"
       name="da-DK-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     es)
       languageCode="es-ES"
       name="es-ES-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     pl)
       languageCode="pl-PL"
       name="pl-PL-Standard-E"
       ssmlGender="FEMALE"
       api=true
       ;;
     pt)
       languageCode="pt-PT"
       name="pt-PT-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     fr)
       languageCode="fr-FR"
       name="fr-FR-Standard-C"
       ssmlGender="FEMALE"
       api=true
       ;;
     it)
       languageCode="it-IT"
       name="it-IT-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     tr)
       languageCode="tr-TR"
       name="tr-TR-Standard-D"
       ssmlGender="FEMALE"
       api=true
       ;;
     tl)
       languageCode="fil-PH"
       name="fil-PH-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     sv)
       languageCode="sv-SE"
       name="sv-SE-Standard-A"
       ssmlGender="FEMALE"
       api=true
       ;;
     be)
       api=true
       ;;
     *)
       echo "Unknown language!"
       exit 1
       ;;
esac

be_into_uk() {
  text=`echo "$text"|sed "s|э|е|g"`
  text=`echo "$text"|sed "s|э́|е́|g"`
  text=`echo "$text"|sed "s|е|є|g"`
  text=`echo "$text"|sed "s|е́|є́|g"`
  text=`echo "$text"|sed "s|ё|йо|g"`
  text=`echo "$text"|sed "s|ё́|йо́|g"`
  text=`echo "$text"|sed "s|и|і|g"`
  text=`echo "$text"|sed "s|и́|і́|g"`
  text=`echo "$text"|sed "s|ў|у|g"`
  text=`echo "$text"|sed "s|ы|и|g"`
  text=`echo "$text"|sed "s|ы́|и́|g"`
  if [ ! "$filename" = "о" ] && [ ! "$filename" = "верталёт" ] && [ ! "$filename" = "блокфлейта" ]
  then
    text=`echo "$text"|sed "s|о|о́|g"`
  fi
}

be_into_ru() {
  text=`echo "$text"|sed "s|і|и|g"`
  text=`echo "$text"|sed "s|і́|и́|g"`
  text=`echo "$text"|sed "s|ў|у|g"`
  text=`echo "$text"|sed "s|'|ъ|g"`
  if [ ! "$filename" = "о" ] && [ ! "$filename" = "верталёт" ] && [ ! "$filename" = "блокфлейта" ]
  then
    text=`echo "$text"|sed "s|о|о́|g"`
  fi
}

lang_runtime() {
case $1 in
   use)
      case $lang in
      be)
          if [ -z "`echo $filename|grep '^ч$'`" ] && [ -z "`echo $filename|grep -v '^ў$'|grep 'ў'`" ] && [ -z "`echo $filename|grep 'г'`" ] && [ -z "`echo $filename|grep 'шч'`" ] && [ -z "`echo $filename|grep 'чы'`" ] && [ -z "`echo $filename|grep 'чэ'`" ] && [ -z "`echo $filename|grep 'чу'`" ] && [ -z "`echo $filename|grep 'ці'`" ] && [ -z "`echo $filename|grep -v 'формачка'|grep 'чк'`" ] && [ -z "`echo $filename|grep -v 'качаня'|grep 'ча'`" ] && [ -z "`echo $filename|grep 'чо'`" ] && [ -z "`echo $filename|grep 'ця'`" ] && [ ! "$filename" = "ы" ] && [ ! "$filename" = "ыых" ] && [ ! "$filename" = "ямс" ] && [ ! "$filename" = "воппер" ] && [ ! "$filename" = "абялікс" ] && [ ! "$filename" = "марскі_цмок" ] && [ ! "$filename" = "мядзведзь" ] && [ ! "$filename" = "чвэрць_долара" ] && [ ! "$filename" = "абедзенная_скрынка" ] && [ ! "$filename" = "абруч" ] && [ ! "$filename" = "смятана" ]
          then
            lang="ru"
            languageCode="ru-RU"
            name="ru-RU-Standard-A"
            ssmlGender="FEMALE"
            be_into_ru "$text"
          else
            lang="uk"
            languageCode="uk-UA"
            name="uk-UA-Standard-A"
            ssmlGender="FEMALE"
            be_into_uk "$text"
          fi
      ;;
      esac
      ;;
   reset)
      case $lang0 in
      be)
        lang=$lang0
      ;;
      esac
;;
esac
}

get_sound(){
lang_runtime "use"
if [ "$api" = "true" ]
then
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
    }" "https://texttospeech.googleapis.com/v1/text:synthesize" > synthesize-text.txt
  echo `cat synthesize-text.txt | grep audioContent|cut -d ":" --fields=2|cut -d "\"" --fields=2` > synthesize-output-base64.txt
  base64 synthesize-output-base64.txt --decode > ../abcs/$lang0/sounds/$1/"$filename.$format"
  rm -f synthesize-text.txt synthesize-output-base64.txt
  ffmpeg -i ../abcs/$lang0/sounds/$1/"$filename.$format" -acodec libvorbis ../abcs/$lang0/sounds/$1/"$filename.$output"
else
  wget "http://translate.google.com/translate_tts?tl=$lang&ie=UTF-8&q=$text&client=tw-ob" -U Mozilla/5.0 -O ../abcs/$lang0/sounds/$1/"$filename.$output"
fi
if [ ! -f ../abcs/$lang0/sounds/$1/"$filename.$output" ]
then
  echo "Error while creating "$filename.$output""
  rm -f ../abcs/$lang0/sounds/$1/*.$format
  exit 1
fi
lang_runtime "reset"
}

lang0=$lang
audioEncoding="LINEAR16"
if [ "$api" = "true" ]
then
  format="wav"
  output="ogg"
else
  format="wav"
  output="mp3"
fi
file_format="properties"

if [ "$api" = "true" ]
then
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
else
  wget=`wget -h`

  if [ -z "$wget" ]
  then
     echo "You have no wget installed!"
     exit 1
  else
     echo "wget detected"
  fi
fi

if [ ! -z `find ../abcs/$lang0 -name *.json` ]
then
  if [ $file_format = "json" ]
  then
    words_list=`cat ../abcs/$lang0/abc*.json|grep \"name\"|cut -d ":" --fields=2 |cut -d "\"" --fields=2|awk '{print tolower($0)}'|sed "s| |YYY|g"|sort -u`
    letters_list=`cat ../abcs/$lang0/abc*.json|grep -v espeak_words|grep -v visible|grep -v sound_letter|grep -v general|grep -v language |grep -v author|grep -v speak_method |grep -v espeak_params|grep -v view_letters|grep -v typing| grep -v letters | grep -v misc |grep -v pic | grep -v name |grep -v noises |grep -v food | grep -v animals | grep -v instrument | grep -v toys | grep -v inheritsFrom |awk '{print tolower($0)}'|sort -u | cut -d "\"" --fields=2|grep -v '\{'|grep -v '\}' | grep -v '\[' |grep -v '\]'| sed s/' '//g|grep -v "^$"`
  fi
fi

if [ ! -z `find ../abcs/$lang0 -name *.properties` ]
then
  if [ $file_format = "properties" ]
  then
    words_list=`cat ../abcs/$lang0/abc*.properties|grep -v '\[missing\]'|grep -v language|grep -v visible| grep -v inheritsFrom | grep -v author|grep -v espeak_params|grep -v view_letters|grep -v speak_method|grep -v typing| grep -v "#"|cut -d "=" --fields=2 |awk '{print tolower($0)}'|sed "s| |YYY|g"|sort -u`
    letters_list=`cat ../abcs/$lang0/abc*.properties|grep -v language|grep -v visible| grep -v inheritsFrom | grep -v author|grep -v espeak_params|grep -v view_letters|grep -v speak_method|grep -v typing| grep -v "#"|cut -d "=" --fields=1 | cut -d ":" --fields=2 |awk '{print tolower($0)}'|sed "s| |YYY|g"|sort -u`
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
  filename=`echo "$a"|cut -d "=" --fields=1|sed 's|YYY| |g'|sed 's| |_|g'`
  text=`echo "$a"|cut -d "=" --fields=1|sed 's|YYY| |g'|sed 's|_| |g'`
  # adding ◌́  for stress, - for pause, fixing pronounce
  case $lang in
     en)
       text=`echo "$text"|sed "s|quena|qweena|g"`
       text=`echo "$text"|sed "s|yangqin|yang ch'in|g"`
       text=`echo "$text"|sed "s|emelya|yemyelya|g"`
       text=`echo "$text"|sed "s|guanaco|guanako|g"`
       text=`echo "$text"|sed "s|iotshin|yot ch'in|g"`
       text=`echo "$text"|sed "s|lirone|lironi|g"`
       text=`echo "$text"|sed "s|oedephon|oedeephon|g"`
       text=`echo "$text"|sed "s|rozhok|rojok|g"`
       text=`echo "$text"|sed "s|uszka|uzhka|g"`
       text=`echo "$text"|sed "s|yaga|yagaw|g"`
       text=`echo "$text"|sed "s|yakult|yakoolt|g"`
       text=`echo "$text"|sed "s|youhikko|your-hikko|g"`
       text=`echo "$text"|sed "s|yueqin|yuee ch'in|g"`
       text=`echo "$text"|sed "s|yupka|yoopka|g"`
       text=`echo "$text"|sed "s|zhaleyka|zhaaleyka|g"`
       ;;
     fr)
       text=`echo "$text"|sed "s|dulzaina|doulzaina|g"`
       text=`echo "$text"|sed "s|ipu|ipou|g"`
       text=`echo "$text"|sed "s|yochin|yotchine|g"`
       text=`echo "$text"|sed "s|yueqin|uétchine|g"`
       text=`echo "$text"|sed "s|yangqin|yangchin|g"`
       ;;
     pl)
       text=`echo "$text"|sed "s|zozulka|zo-zulka|g"`
       text=`echo "$text"|sed "s|walkie-talkie|łoki-toki|g"`
       text=`echo "$text"|sed "s|kastaniety|kasta-niety|g"`
       text=`echo "$text"|sed "s|luwers|luvers|g"`
       text=`echo "$text"|sed "s|ćwierć dolara|ćwierć dollara|g"`
       ;;
     de)
       text=`echo "$text"|sed "s|x-keks|x-kiks|g"`
       text=`echo "$text"|sed "s|umka|oomka|g"`
       text=`echo "$text"|sed "s|luftballons|luft-ballons|g"`
       text=`echo "$text"|sed "s|xun|ksoon|g"`
       text=`echo "$text"|sed "s|yangqin|yang--qin|g"`
       text=`echo "$text"|sed "s|äoline|äoliene|g"`
       text=`echo "$text"|sed "s|jug|jugg|g"`
       text=`echo "$text"|sed "s|kazoo|kazuu|g"`
       text=`echo "$text"|sed "s|ödephon|ödephoon|g"`
       text=`echo "$text"|sed "s|öse|ösae|g"`
       text=`echo "$text"|sed "s|python|pyton|g"`
       text=`echo "$text"|sed "s|wagon|wa-gon|g"`
       ;;
     ru)
       text=`echo "$text"|sed "s|жар-птица|жарптица|g"`
       text=`echo "$text"|sed "s|адъютант|адъйутант|g"`
       text=`echo "$text"|sed "s|ёочин|ё́очин|g"`
       text=`echo "$text"|sed "s|еху|йеху|g"`
       text=`echo "$text"|sed "s|жаркое|жарко́е|g"`
       text=`echo "$text"|sed "s|удон|удо́н|g"`
       text=`echo "$text"|sed "s|шары|шары́|g"`
       text=`echo "$text"|sed "s|иглу|и́глу|g"`
       text=`echo "$text"|sed "s|йогурт|йо́гурт|g"`
       text=`echo "$text"|sed "s|кларнет|кларнэт|g"`
       text=`echo "$text"|sed "s|маракасы|мара́касы|g"`
       text=`echo "$text"|sed "s|орган|орга́н|g"`
       text=`echo "$text"|sed "s|подъязок|подъйазок|g"`
       text=`echo "$text"|sed "s|подъёлочник|подъё́лочник|g"`
       text=`echo "$text"|sed "s|цуккини|цукини|g"`
       text=`echo "$text"|sed "s|аэолин|аэоли́н|g"`
       text=`echo "$text"|sed "s|белка|бе́лка|g"`
       text=`echo "$text"|sed "s|васаби|васа́би|g"`
       text=`echo "$text"|sed "s|вибраслэп|вибраслэ́п|g"`
       text=`echo "$text"|sed "s|видеонаблюдение|видео-наблюдение|g"`
       text=`echo "$text"|sed "s|вінаград|ві-наград|g"`
       text=`echo "$text"|sed "s|воппер|во́ппер|g"`
       text=`echo "$text"|sed "s|дулзайна|дулза́йна|g"`
       text=`echo "$text"|sed "s|идефикс|иде́фикс|g"`
       text=`echo "$text"|sed "s|йора|йё́ра|g"`
       text=`echo "$text"|sed "s|казу|казу́|g"`
       text=`echo "$text"|sed "s|квазимодо|квазимо́до|g"`
       text=`echo "$text"|sed "s|кетчуп|ке́тчуп|g"`
       text=`echo "$text"|sed "s|княженика|княжени́ка|g"`
       text=`echo "$text"|sed "s|ксифактин|ксифакти́н|g"`
       text=`echo "$text"|sed "s|ксоубас|ксо́убас|g"`
       text=`echo "$text"|sed "s|лазанья|лаза́нья|g"`
       text=`echo "$text"|sed "s|перепел|пе́репел|g"`
       text=`echo "$text"|sed "s|равел|раве́л|g"`
       text=`echo "$text"|sed "s|тальятелле|тальяте́лле|g"`
       text=`echo "$text"|sed "s|уакари|уака́ри|g"`
       text=`echo "$text"|sed "s|урсула|у́рсула|g"`
       text=`echo "$text"|sed "s|флейта бёма|флейта бё́ма|g"`
       text=`echo "$text"|sed "s|цельнозерновой хлеб|цельно-зерновой хлеб|g"`
       text=`echo "$text"|sed "s|шалюмо|шалюмо́|g"`
       text=`echo "$text"|sed "s|эстрагон|э-страгон|g"`
       text=`echo "$text"|sed "s|юринг|ю́ринг|g"`
       text=`echo "$text"|sed "s|якульт|йаку́льт|g"`
       text=`echo "$text"|sed "s|янорнис|яно́рнис|g"`
       ;;
     be)
       text=`echo "$text"|sed "s|абедзенная|а-бедзенная|g"`
       text=`echo "$text"|sed "s|авечка|а-вечка|g"`
       text=`echo "$text"|sed "s|агеньчык|а-геньчык|g"`
       text=`echo "$text"|sed "s|абялікс|абя-лікс|g"`
       text=`echo "$text"|sed "s|аліў'е|аліў-е|g"`
       text=`echo "$text"|sed "s|ад'ютант|ад'йутант|g"`
       text=`echo "$text"|sed "s|акарына|ака́рына|g"`
       text=`echo "$text"|sed "s|арган|ар-ган|g"`
       text=`echo "$text"|sed "s|арыгамі|ары-гамі|g"`
       text=`echo "$text"|sed "s|піяніна|піяні́на|g"`
       text=`echo "$text"|sed "s|бачкавое|бачка-вое|g"`
       text=`echo "$text"|sed "s|бульба|бу́льба|g"`
       text=`echo "$text"|sed "s|васьміног|вась-міног|g"`
       text=`echo "$text"|sed "s|ваўчок|ваў--чок|g"`
       text=`echo "$text"|sed "s|гародніна|га-родніна|g"`
       text=`echo "$text"|sed "s|гарох|га-рох|g"`
       text=`echo "$text"|sed "s|васабі|васа́бі|g"`
       text=`echo "$text"|sed "s|вермішэль|вермішэ́ль|g"`
       text=`echo "$text"|sed "s|верталёт|вертолёт|g"`
       text=`echo "$text"|sed "s|віктарына|віктары́на|g"`
       text=`echo "$text"|sed "s|вінягрэт|віняг-рэт|g"`
       text=`echo "$text"|sed "s|выбраслэп|выбраслэ́п|g"`
       text=`echo "$text"|sed "s|газель|га-зель|g"`
       text=`echo "$text"|sed "s|галактычны салдат|галактыч-ны сал-дат|g"`
       text=`echo "$text"|sed "s|глушэц|глу-шэц|g"`
       text=`echo "$text"|sed "s|гуанака|гуа-нака|g"`
       text=`echo "$text"|sed "s|даніё рэрыё|да́нійо рэ́рыйо|g"`
       text=`echo "$text"|sed "s|дзікабраз|дзікабра́з|g"`
       text=`echo "$text"|sed "s|дулзайна|дульза́йна|g"`
       text=`echo "$text"|sed "s|дурыян|дурыя́н|g"`
       text=`echo "$text"|sed "s|духі|духі́|g"`
       text=`echo "$text"|sed "s|емуранчык|ему-ранчык|g"`
       text=`echo "$text"|sed "s|жэлацін|жэла-цін|g"`
       text=`echo "$text"|sed "s|зубная шчотка|зуб-ная шчотка|g"`
       text=`echo "$text"|sed "s|ідэфікс|іде́фікс|g"`
       text=`echo "$text"|sed "s|ірга|ір-га|g"`
       text=`echo "$text"|sed "s|йора|йё́ра|g"`
       text=`echo "$text"|sed "s|каала|каа́ла|g"`
       text=`echo "$text"|sed "s|казу|казу́|g"`
       text=`echo "$text"|sed "s|кавалак|кава́лак|g"`
       text=`echo "$text"|sed "s|каркадэ|каркадэ́|g"`
       text=`echo "$text"|sed "s|касцяніца|касця-ніца|g"`
       text=`echo "$text"|sed "s|кісанджы|кісанджі|g"`
       text=`echo "$text"|sed "s|ксіфактын|ксіфакты́н|g"`
       text=`echo "$text"|sed "s|лазанья|лаза́нья|g"`
       text=`echo "$text"|sed "s|літаўры|лі-таўры|g"`
       text=`echo "$text"|sed "s|ляжак|ляжа́к|g"`
       text=`echo "$text"|sed "s|малы барабан|ма́лы барабан|g"`
       text=`echo "$text"|sed "s|маракасы|мара́касы|g"`
       text=`echo "$text"|sed "s|марская|марска́я|g"`
       text=`echo "$text"|sed "s|марскі вожык|марскі́ вожык|g"`
       text=`echo "$text"|sed "s|марскі цмок|мар-с-кі цмок|g"`
       text=`echo "$text"|sed "s|мядуза|мя-дуза|g"`
       text=`echo "$text"|sed "s|мядзведзь|мядзьведзь|g"`
       text=`echo "$text"|sed "s|мідыя|мі́дыя|g"`
       text=`echo "$text"|sed "s|мурашкаед|мурашкае́д|g"`
       text=`echo "$text"|sed "s|бранзалет|бранзале́т|g"`
       text=`echo "$text"|sed "s|нажніцы|нажні́цы|g"`
       text=`echo "$text"|sed "s|цырк|цирк|g"`
       text=`echo "$text"|sed "s|насарог|на-са-рог|g"`
       text=`echo "$text"|sed "s|нектарын|нектары́н|g"`
       text=`echo "$text"|sed "s|нутрыя|ну́трыя|g"`
       text=`echo "$text"|sed "s|віяланчэль|віялан-чэ́ль|g"`
       text=`echo "$text"|sed "s|нязнайка|ня-знайка|g"`
       text=`echo "$text"|sed "s|паветраны|паве́траны|g"`
       text=`echo "$text"|sed "s|павук|паву́к|g"`
       text=`echo "$text"|sed "s|пад’ёмны|пад’-ё́мны|g"`
       text=`echo "$text"|sed "s|самалёт|сама-лё́т|g"`
       text=`echo "$text"|sed "s|слімак|сьлима́к|g"`
       text=`echo "$text"|sed "s|снежны барс|сьнежны барс|g"`
       text=`echo "$text"|sed "s|пальчатка|паль-чатка|g"`
       text=`echo "$text"|sed "s|рабэль|ра́бэль|g"`
       text=`echo "$text"|sed "s|радыска|рады́ска|g"`
       text=`echo "$text"|sed "s|рацыя|ра́цыя|g"`
       text=`echo "$text"|sed "s|раяль|ра-я́ль|g"`
       text=`echo "$text"|sed "s|рэнтгенаўскае выпраменьванне|рэнтге-наўскае выпра-меньваньне|g"`
       text=`echo "$text"|sed "s|ручная|ручна́я|g"`
       text=`echo "$text"|sed "s|сава|сава́|g"`
       text=`echo "$text"|sed "s|селядзец|селядзе́ц|g"`
       text=`echo "$text"|sed "s|снягір|сьня-гір|g"`
       text=`echo "$text"|sed "s|спадніца|спадні́ца|g"`
       text=`echo "$text"|sed "s|сцярвятнік|сцяр-вятнік|g"`
       text=`echo "$text"|sed "s|сэмплер|семплер|g"`
       text=`echo "$text"|sed "s|свіння|сьвіньня|g"`
       text=`echo "$text"|sed "s|талеркі|та-леркі|g"`
       text=`echo "$text"|sed "s|танжэла|танжэ́ла|g"`
       text=`echo "$text"|sed "s|тархун|тарху́н|g"`
       text=`echo "$text"|sed "s|трактар|тра́ктар|g"`
       text=`echo "$text"|sed "s|трашчотка|траш--чотка|g"`
       text=`echo "$text"|sed "s|тэрменвокс|тэр-менвокс|g"`
       text=`echo "$text"|sed "s|туалетная шчотка|туа-летная шчотка|g"`
       text=`echo "$text"|sed "s|угал|у-гал|g"`
       text=`echo "$text"|sed "s|урсула|у́рсула|g"`
       text=`echo "$text"|sed "s|фартэпіяна|фартэ-піяна|g"`
       text=`echo "$text"|sed "s|фламінга|фла-мінга|g"`
       text=`echo "$text"|sed "s|флейта бёма|флейта бё́ма|g"`
       text=`echo "$text"|sed "s|фрула|фру́ла|g"`
       text=`echo "$text"|sed "s|цацарка|цаца́рка|g"`
       text=`echo "$text"|sed "s|цыбуля|цыбу́ля|g"`
       text=`echo "$text"|sed "s|цытра|цы́тра|g"`
       text=`echo "$text"|sed "s|чабор|ча-бор|g"`
       text=`echo "$text"|sed "s|чабурашка|чабу-рашка|g"`
       text=`echo "$text"|sed "s|чарапаха|ча-ра-па-ха|g"`
       text=`echo "$text"|sed "s|часнок|час-нок|g"`
       text=`echo "$text"|sed "s|чвэрць долара|чвэррць долара|g"`
       text=`echo "$text"|sed "s|чэлеста|чэ-леста|g"`
       text=`echo "$text"|sed "s|шакалад|ша-кала́д|g"`
       text=`echo "$text"|sed "s|шары|шары́|g"`
       text=`echo "$text"|sed "s|шчавель|шча-вель|g"`
       text=`echo "$text"|sed "s|шчупак|шчу-пак|g"`
       text=`echo "$text"|sed "s|шымпанзэ|шымпанзэ́|g"`
       text=`echo "$text"|sed "s|эаліна|эалі́на|g"`
       text=`echo "$text"|sed "s|экскаватар|эк-скава́тар|g"`
       text=`echo "$text"|sed "s|электрагітара|э-лект-ра-гітара|g"`
       text=`echo "$text"|sed "s|эстонскі|э-стон-скі|g"`
       text=`echo "$text"|sed "s|юэцынь|юэцы́нь|g"`
       text=`echo "$text"|sed "s|ягіня|йа-гі-ня|g"`
       text=`echo "$text"|sed "s|якульт|йаку́льт|g"`
       text=`echo "$text"|sed "s|ялец|яле́ц|g"`
       text=`echo "$text"|sed "s|ямеля|яме́ля|g"`
       text=`echo "$text"|sed "s|ятаган|йа-та-ган|g"`
       text=`echo "$text"|sed "s|яхідна|йахідна|g"`
       text=`echo "$text"|sed "s|цяля|ця-ля|g"`
       text=`echo "$text"|sed "s|блазан|бла́зан|g"`
       text=`echo "$text"|sed "s|гваздзік|гвазд-зік|g"`
       text=`echo "$text"|sed "s|качаня|качаня́|g"`
       text=`echo "$text"|sed "s|жарабя|жарабя́|g"`
       text=`echo "$text"|sed "s|пацеркі|па́церкі|g"`
       text=`echo "$text"|sed "s|шкляная|шкля-ная|g"`
       text=`echo "$text"|sed "s|вадзянік|вадзяні́к|g"`
       text=`echo "$text"|sed "s|часопіс|ча-сопіс|g"`
       text=`echo "$text"|sed "s|кручок|кру--чок|g"`
       text=`echo "$text"|sed "s|смятана|сьмя-тана|g"`
       text=`echo "$text"|sed "s|запяканка|запя-канка|g"`
       ;;
     uk)
       #acute accent does not work with uk yet
       text=`echo "$text"|sed "s|аґрус|а́ґрус|g"`
       text=`echo "$text"|sed "s|горох|горо́х|g"`
       text=`echo "$text"|sed "s|дзиґа|дзи́ґа|g"`
       text=`echo "$text"|sed "s|еклер|екле́р|g"`
       text=`echo "$text"|sed "s|єнот|єно́т|g"`
       text=`echo "$text"|sed "s|жаба|жа́ба|g"`
       text=`echo "$text"|sed "s|зурна|зурна́|g"`
       text=`echo "$text"|sed "s|iрга|iрга́|g"`
       text=`echo "$text"|sed "s|йоухікко|йоухі́кко|g"`
       text=`echo "$text"|sed "s|магніт|магні́т|g"`
       text=`echo "$text"|sed "s|маракаси|мара́каси|g"`
       text=`echo "$text"|sed "s|мольберт|мольбе́рт|g"`
       text=`echo "$text"|sed "s|незнайка|незна́йка|g"`
       text=`echo "$text"|sed "s|носоріг|носорі́г|g"`
       text=`echo "$text"|sed "s|орган|орга́н|g"`
       text=`echo "$text"|sed "s|пелікан|пеліка́н|g"`
       text=`echo "$text"|sed "s|редис|реди́с|g"`
       text=`echo "$text"|sed "s|ріжок|ріжо́к|g"`
       text=`echo "$text"|sed "s|тритон|трито́н|g"`
       text=`echo "$text"|sed "s|труба|труба́|g"`
       text=`echo "$text"|sed "s|халва|халва́|g"`
       text=`echo "$text"|sed "s|цукіні|цукі́ні|g"`
       text=`echo "$text"|sed "s|чаньго|ча́ньго|g"`
       text=`echo "$text"|sed "s|шарманка|шарма́нка|g"`
       text=`echo "$text"|sed "s|шимпанзе|шимпанзе́|g"`
       ;;
  esac
  if [ "$api" = "true" ]
  then
    text=${text//\'/\\\'}
  fi
  get_sound "words"
done

mkdir -p ../abcs/$lang0/sounds/alpha
rm -f ../abcs/$lang0/sounds/alpha/*.$format
rm -f ../abcs/$lang0/sounds/alpha/*.$output

for a in $letters_list
do
  filename="$a"
  text="$a"
  case $lang in
     be)
       text=`echo "$text"|sed "s|^й$|і нескладовае|g"`
       text=`echo "$text"|sed "s|^ў$|у нескладовае|g"`
       text=`echo "$text"|sed "s|^ь$|мяккі знак|g"`
       ;;
  esac
  get_sound "alpha"
done

# sometimes using other languages instead of native because of better pronounce
if [ "$lang" = "ru" ]
then
    lang="uk"
    languageCode="uk-UA"
    name="uk-UA-Standard-A"
    filename="ы"
    text="и"
    if [ -f "../abcs/$lang0/sounds/alpha/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/alpha/$filename.$format
      rm -f ../abcs/$lang0/sounds/alpha/$filename.$output
      get_sound "alpha"
    fi

    lang="uk"
    languageCode="uk-UA"
    name="uk-UA-Standard-A"
    filename="ы"
    text="и"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi

    lang="uk"
    languageCode="uk-UA"
    name="uk-UA-Standard-A"
    filename="ыых"
    text="и-их"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi

    lang="uk"
    languageCode="uk-UA"
    name="uk-UA-Standard-A"
    filename="ямс"
    text="ямс"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi

    lang="uk"
    languageCode="uk-UA"
    name="uk-UA-Standard-A"
    filename="воппер"
    text="воппєр"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi

    lang="uk"
    languageCode="uk-UA"
    name="uk-UA-Standard-A"
    filename="моноцикл"
    text="моноцикл"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi

    lang="uk"
    languageCode="uk-UA"
    name="uk-UA-Standard-A"
    filename="лиса"
    text="лi-са"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi

fi
if [ "$lang" = "de" ]
then
    lang="pl"
    languageCode="pl-PL"
    name="pl-PL-Standard-B"
    filename="ipu"
    text="ipu"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi
fi
if [ "$lang" = "en" ]
then
    lang="ru"
    languageCode="ru-RU"
    name="ru-RU-Standard-A"
    filename="ipu"
    text="ипу"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi

    lang="fr"
    languageCode="fr-FR"
    name="fr-FR-Standard-C"
    filename="xun"
    text="xun"
    if [ -f "../abcs/$lang0/sounds/words/$filename.$output" ]
    then
      rm -f ../abcs/$lang0/sounds/words/$filename.$format
      rm -f ../abcs/$lang0/sounds/words/$filename.$output
      get_sound "words"
    fi
fi

#clean up
rm -f ../abcs/$lang0/sounds/words/*.$format
rm -f ../abcs/$lang0/sounds/alpha/*.$format
