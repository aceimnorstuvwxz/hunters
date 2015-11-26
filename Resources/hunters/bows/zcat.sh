!#/bin/sh

for img in *.png
do
  klsdTgaScale $img $img.so.png
  scatPixelGen $img.so.png
done
