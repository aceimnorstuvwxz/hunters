#!/bin/sh
function cutout()
{
  layerput head_rect.png 8 20 $1.5.so.png 6 3 7 2 $1.head.tmp1.png
  layerput $1.head.tmp1.png 0 0 $1.2.so.png 7 7 22 22 $1.head.tmp2.png
  layerput $1.head.tmp2.png 0 0 head_rect.png 0 0 22 22 $1.head.png
  scatPixelGen $1.head.png
  rm $1.head.tmp1.png
  rm $1.head.tmp2.png
}

cutout "0.png"
cutout "1.png"
cutout "2.png"
cutout "3.png"
cutout "4.png"
cutout "5.png"
cutout "6.png"
cutout "7.png"
