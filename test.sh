#!/bin/bash
if cmake --build build; then
  build/squint
  convert -size 3840x2160 -depth 8 BGRA:test.raw -separate -delete 3 -combine test.png
  feh --fullscreen test.png
  rm -f test.raw test.png
fi
