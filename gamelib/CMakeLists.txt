# FIXME Need openGL, GLUT, SDL, etc. Lots of shit.
# FIND_PACKAGE(Qt4 REQUIRED)
    
# General
SET( GAMELIB_SRC
    BoxFactory.cpp
)
         
SET( GAMELIB_LIB
)
          
ADD_LIBRARY( boxfactory ${GAMELIB_SRC} )
TARGET_LINK_LIBRARIES( boxfactory ${GAMELIB_LIB} )
