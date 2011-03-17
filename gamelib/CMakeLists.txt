# FIXME Need openGL, GLUT, SDL, etc. Lots of shit.

FIND_PACKAGE(Lua51 REQUIRED)

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
    SceneLua.cpp
#    SoundManager.cpp
)
         
SET( GAMELIB_LIB
)
          
ADD_LIBRARY( boxfactory ${GAMELIB_SRC} )
TARGET_LINK_LIBRARIES( boxfactory ${GAMELIB_LIB} )
