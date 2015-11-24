#ifndef TEXTURES_H__
#define TEXTURES_H__


#define TEXTURE_COUNT 17

namespace Textures {

  enum Texture{
    TEX_NO_TEXTURE = 0,
    TEX_WALL1           = 1, // Grim Metal Wall
    TEX_WALL2      = 2, // Red Metal Wall
    TEX_WALL3      = 3, // Metal plate wall
    TEX_WALL4      = 4, // Rusty Nasty wall
    TEX_FLOOR1     = 5, // Steely floor
    TEX_FLOOR2     = 6, // Plate Floor
    TEX_FLOOR3     = 7, // Dark plate floor
    TEX_FLOOR4     = 8,
    TEX_GUNMETAL   = 9,
    TEX_ARMOUR     = 10,
    TEX_CEILING    = 11,
    TEX_BOX1       = 12,
    TEX_BOX2       = 13,
    TEX_BOX3       = 14,
    TEX_BOX4       = 15,
    TEX_WALL1_MAP  = 16
  };

  static std::string texFolder = "data/textures/";
  static std::string texPaths[] = {
    "",
    "wall_metal.png",
    "wall_metal2.png",
    "wall_plates.png",
    "wall_rust.png",
    "floor_steel.png",
    "floor_metal.png",
    "floor_metal2.png",
    "floor_snow.png",
    "gunmetal.png",
    "enemy_armour.png",
    "floor_concrete.png",
    "box_metal1.png",
    "box_metal2.png",
    "box_metal3.png",
    "box_wood.png",
    "wall_metal_map.png"
  };
  }

#endif
