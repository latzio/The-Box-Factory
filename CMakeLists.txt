CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(PLAYBOOK)
 
SET(CMAKE_BUILD_TYPE Debug)
  
SET(CMAKE_CXX_FLAGS "-Wall")
   
# QT4 Handling
FIND_PACKAGE(Qt4 REQUIRED)
    
INCLUDE(${QT_USE_FILE})

ADD_SUBDIRECTORY( src )

SET( HWQ_Qt4_SRC
    MainWindow.h
	HelloWidget.h
)
SET( HWQ_Qt4_UI
)
SET( HWQ_Qt4_RES
)
      
QT4_WRAP_CPP(HWQ_MOC_CPP ${HWQ_Qt4_SRC})
QT4_WRAP_UI(HWQ_UI_CPP ${HWQ_Qt4_UI})
QT4_ADD_RESOURCES(HWQ_RES_H ${HWQ_Qt4_RES})
       
        
# General
SET( HWQ_SRC
    main.cpp
    HelloWidget.cpp
    MainWindow.cpp
    ${HWQ_MOC_CPP}
    ${HWQ_UI_CPP}
    ${HWQ_RES_H}
)
         
SET( HWQ_LIB
    ${QT_LIBRARIES}
)
          
ADD_EXECUTABLE( HelloWorldQt ${HWQ_SRC} )
TARGET_LINK_LIBRARIES( HelloWorldQt ${HWQ_LIB} )
INSTALL_TARGETS( /bin HelloWorldQt)
