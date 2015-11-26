# While gamelib does depend on OpenGL and Lua51, we only build it
# into a static library. It's up to the app wrappers to link those in.

cmake_minimum_required(VERSION 3.0)

SET( GAMELIB_SRC
    AI.cpp
    Algebra.cpp
    BoxFactory.cpp
    Game.cpp
    Image.cpp
    Material.cpp
    Object.cpp
    Primitive.cpp
    Scene.cpp
    SceneLua.cpp
#    SoundManager.cpp
)

SET( GAMELIB_LIB
)

ADD_LIBRARY( boxfactory ${GAMELIB_SRC} )
TARGET_LINK_LIBRARIES( boxfactory ${GAMELIB_LIB} )
