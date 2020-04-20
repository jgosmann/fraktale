#!/bin/sh
for bild in `ls $1/*.bmp`
do
  echo "$bild -> ${bild%.bmp}.jpg"
  convert -quality 100 "$bild" "${bild%.bmp}.jpg"
  rm $bild
done
