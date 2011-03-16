# QT4 Handling
FIND_PACKAGE(Qt4 REQUIRED)
    
INCLUDE(${QT_USE_FILE})

SET( PLAY_Qt4_SRC
    MainWindow.h
	HelloWidget.h
)
SET( PLAY_Qt4_UI
)
SET( PLAY_Qt4_RES
)
      
QT4_WRAP_CPP(PLAY_MOC_CPP ${PLAY_Qt4_SRC})
QT4_WRAP_UI(PLAY_UI_CPP ${PLAY_Qt4_UI})
QT4_ADD_RESOURCES(PLAY_RES_H ${PLAY_Qt4_RES})
       
# General
SET( PLAY_SRC
    main.cpp
    HelloWidget.cpp
    MainWindow.cpp
    ${PLAY_MOC_CPP}
    ${PLAY_UI_CPP}
    ${PLAY_RES_H}
)
         
SET( PLAY_LIB
    ${QT_LIBRARIES}
)
          
ADD_EXECUTABLE( out ${PLAY_SRC} )
TARGET_LINK_LIBRARIES( out ${PLAY_LIB} )
#INSTALL_TARGETS( ${PLAYBOOK_SOURCE_DIR}/bin out )
