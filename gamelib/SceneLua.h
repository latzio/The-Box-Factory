#ifndef SCENE_LUA_HPP
#define SCENE_LUA_HPP

#include <string>
#include "Scene.h"

SceneNode* import_lua(const std::string& filename);

#endif
