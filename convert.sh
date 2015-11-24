cp ../data/*png .
IN=data/textures_hq
OUT=data/textures
convert $IN/box_metal1.png     -resize 96x96 $OUT/box_metal1.png
convert $IN/box_metal2.png     -resize 96x96 $OUT/box_metal2.png
convert $IN/box_metal3.png     -resize 96x96 $OUT/box_metal3.png
convert $IN/box_wood.png       -resize 96x96 $OUT/box_wood.png
convert $IN/enemy_armour.png   -resize 32x32 $OUT/enemy_armour.png
convert $IN/floor_concrete.png -resize 32x32 $OUT/floor_concrete.png
convert $IN/floor_metal2.png   -resize 32x32 $OUT/floor_metal2.png
convert $IN/floor_metal.png    -resize 32x32 $OUT/floor_metal.png
convert $IN/floor_snow.png     -resize 32x32 $OUT/floor_snow.png
convert $IN/floor_steel.png    -resize 32x32 $OUT/floor_steel.png
convert $IN/gunmetal.png       -resize 32x32 $OUT/gunmetal.png
convert $IN/wall_metal2.png    -resize 64x64 $OUT/wall_metal2.png
convert $IN/wall_metal.png     -resize 64x64 $OUT/wall_metal.png
convert $IN/wall_plates.png    -resize 64x64 $OUT/wall_plates.png
convert $IN/wall_rust.png      -resize 64x64 $OUT/wall_rust.png
