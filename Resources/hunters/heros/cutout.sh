#!/bin/sh
function cutout()
{
  imgcutout $1 $1.0.png 19 95 10 116
  scatPixelGen $1.0.png
  imgcutout $1 $1.1.png 95 161 10 116
  scatPixelGen $1.1.png
  imgcutout $1 $1.2.png 77 190 110 204
  scatPixelGen $1.2.png
  imgcutout $1 $1.3.png 0 29 205 256
  scatPixelGen $1.3.png
  imgcutout $1 $1.4.png 30 74 205 256
  scatPixelGen $1.4.png
  imgcutout $1 $1.5.png 129 179 205 256
  scatPixelGen $1.5.png
  imgcutout $1 $1.6.png 179 211 205 256
  scatPixelGen $1.6.png
  imgcutout $1 $1.7.png 211 237 205 256
  scatPixelGen $1.7.png
  imgcutout $1 $1.8.png 237 256 205 256
  scatPixelGen $1.8.png
}

cutout "0.png"
cutout "1.png"
cutout "2.png"
cutout "3.png"
cutout "4.png"
cutout "5.png"
cutout "6.png"
cutout "7.png"
