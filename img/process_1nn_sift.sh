#!/bin/sh

for f in $(ls simplified_sift_all/)
do
  echo "Traitrement de $f"
  #sed -n '4,$p' $f | tr -d ";" | sed 's/<CIRCLE [1-9].*> //' > ./trav.sift
  R --slave --no-save --no-restore --no-environ --args centers256.txt 256 simplified_sift_all/$f 1nn/$f < 1nn.R
done


