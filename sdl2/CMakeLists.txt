# Libs to link binary using gamelib
cmake_minimum_required(VERSION 3.0)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(Lua51 REQUIRED)

# General
SET(BF_SDL2_SRC
    main.cpp
)

SET(BF_SDL2_LIB
    ${LUA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    ${SDL2_LIBRARY}
)

ADD_EXECUTABLE( bfsdl2 ${BF_SDL2_SRC} )
TARGET_LINK_LIBRARIES( bfsdl2
                       ${LIBRARY_OUTPUT_PATH}/libboxfactory.a
                       ${BF_SDL2_LIB})
