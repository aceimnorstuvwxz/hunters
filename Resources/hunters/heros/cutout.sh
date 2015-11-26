#!/bin/sh
function cutout()
{
  imgcutout $1 $1.0.png 19 95 10 116
  klsdTgaScale $1.0.png $1.0.so.png
  rm $1.0.png
  scatPixelGen $1.0.so.png
  imgcutout $1 $1.1.png 95 161 10 116
  klsdTgaScale $1.1.png $1.1.so.png
  rm $1.1.png
  scatPixelGen $1.1.so.png
  imgcutout $1 $1.2.png 77 190 110 204
  klsdTgaScale $1.2.png $1.2.so.png
  rm $1.2.png
  scatPixelGen $1.2.so.png
  imgcutout $1 $1.3.png 0 29 205 256
  klsdTgaScale $1.3.png $1.3.so.png
  rm $1.3.png
  scatPixelGen $1.3.so.png
  imgcutout $1 $1.4.png 30 74 205 256
  klsdTgaScale $1.4.png $1.4.so.png
  rm $1.4.png
  scatPixelGen $1.4.so.png
  imgcutout $1 $1.5.png 129 179 205 256
  klsdTgaScale $1.5.png $1.5.so.png
  rm $1.5.png
  scatPixelGen $1.5.so.png
  imgcutout $1 $1.6.png 179 211 205 256
  klsdTgaScale $1.6.png $1.6.so.png
  rm $1.6.png
  scatPixelGen $1.6.so.png
  imgcutout $1 $1.7.png 211 237 205 256
  klsdTgaScale $1.7.png $1.7.so.png
  rm $1.7.png
  scatPixelGen $1.7.so.png
  imgcutout $1 $1.8.png 237 256 205 256
  klsdTgaScale $1.8.png $1.8.so.png
  rm $1.8.png
  scatPixelGen $1.8.so.png
}

cutout "0.png"
cutout "1.png"
cutout "2.png"
cutout "3.png"
cutout "4.png"
cutout "5.png"
cutout "6.png"
cutout "7.png"
