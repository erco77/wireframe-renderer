comp-%.png: grid-%.png
	convert $< \
              \( +clone -blur 0x25 -level 0%,50% \) \
	      -compose screen \
	      -composite \
	      $@
