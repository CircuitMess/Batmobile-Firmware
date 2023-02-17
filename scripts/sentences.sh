#!/bin/bash

set -e -o pipefail

# Takes the input file sentences.txt and saves .aac files in the data folder

# Google TTS api key
KEY=

while IFS= read -r sentence; do
  name=$(echo "$sentence" | cut -d " " -f1)
  echo "$name"
  sentence=$(echo "$sentence" | tail -c +$(expr ${#name} + 2))
  mkdir -p "data/Voice"
  echo "data/Voice/$name: $sentence"
  curl -s --location --request POST "https://texttospeech.googleapis.com/v1/text:synthesize?key=$KEY" \
  --header "Content-Type: application/json" \
  --data-raw '{
    "input": { "ssml": "<speak> '"$sentence"' <break time=\"500ms\"/> </speak> " },
   "voice": {
       "languageCode": "en-GB",
       "name": "en-GB-Standard-D"
     },
       "audioConfig": {
       "audioEncoding": "MP3",
       "pitch": -3.2,
       "speakingRate": 0.90,
       "sampleRateHertz": 32000
     }
  }' | jq -r '.audioContent'  | base64 -d  | ffmpeg -loglevel quiet -f mp3 -i pipe: -c:a aac -b:a 32k "data/Voice/$name.aac"
done < sentences.txt
#<emphasis level=\"moderate\">