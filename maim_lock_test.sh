unblurred=/tmp/i3-lock-screenshot.png
blurred=/tmp/i3-lock-screenshot-blurred.png

maim $unblurred
# No blur kernel yet:
# convert -filter Point -scale 2.5% -resize 3840x2160\! -blur 0x2 $unblurred $blurred
convert -filter Point -scale 2.5% -resize 3840x2160\! $unblurred $blurred
rm $unblurred
# i3lock -i $blurred

