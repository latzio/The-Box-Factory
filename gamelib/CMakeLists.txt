# FIXME Need openGL, GLUT, SDL, etc. Lots of shit.

FIND_LIBRARY(OpenGL REQUIRED)
FIND_LIBRARY(Lua51 REQUIRED)

# General
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
#    SceneLua.cpp Uncomment once libs are working.
#    SoundManager.cpp
)
         
SET( GAMELIB_LIB
)
          
ADD_LIBRARY( boxfactory ${GAMELIB_SRC} )
TARGET_LINK_LIBRARIES( boxfactory ${GAMELIB_LIB} )
