#!/bin/sh

cnt=0
function ccc()
{
  convert $1 $cnt.png
  let cnt++
}
for img in *.tga
do
  ccc $img
done
