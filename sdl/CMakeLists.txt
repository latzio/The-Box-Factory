# Libs to link binary using gamelib
cmake_minimum_required(VERSION 3.0)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(PNG REQUIRED)
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(Lua51 REQUIRED)

# General
SET(BF_SDL_SRC
    main.cpp
)

SET(BF_SDL_LIB
    ${LUA_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    ${SDL_LIBRARY}
)

ADD_EXECUTABLE( bfsdl ${BF_SDL_SRC} )
TARGET_LINK_LIBRARIES( bfsdl
                       ${LIBRARY_OUTPUT_PATH}/libboxfactory.a
                       ${BF_SDL_LIB})
