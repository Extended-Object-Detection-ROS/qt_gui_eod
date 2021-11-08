cmake_minimum_required(VERSION 3.5)

project(qt_gui_eod VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(TinyXML REQUIRED)

set( tinyxml_srcs tinyxml/tinyxml.cpp tinyxml/tinyxmlparser.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinystr.cpp )

set( eod_srcs
    lib/ObjectBase.cpp
    # types
    lib/types/ComplexObjectGraph.cpp
    lib/types/ComplexObject.cpp
    lib/types/ExtendedObjectInfo.cpp
    lib/types/Attribute.cpp
    lib/types/SimpleObject.cpp
    lib/types/Tracker.cpp
    lib/types/Relationship.cpp
    lib/types/Filtering.cpp
    lib/types/Clusterization.cpp
    # lib utils
    lib/utils/geometry_utils.cpp
    lib/utils/drawing_utils.cpp
    lib/utils/contour_utils.cpp
    # lib detectors
    lib/detectors/HsvColorDetector.cpp
    lib/detectors/HaarCascadeDetector.cpp
    lib/detectors/SizeDetector.cpp
    lib/detectors/HistColorDetector.cpp
    lib/detectors/DimentionDetector.cpp
    lib/detectors/HoughDetector.cpp
    lib/detectors/BasicMotionDetector.cpp
    lib/detectors/ArucoDetector.cpp
    lib/detectors/PoseDetector.cpp
    lib/detectors/TensorFlowDetector.cpp
    lib/detectors/FeatureDetector.cpp
    lib/detectors/DnnDetector.cpp
    lib/detectors/QrDetector.cpp
    lib/detectors/QrZbarDetector.cpp
    lib/detectors/LogicDetector.cpp
    lib/detectors/BlobDetector.cpp
    lib/detectors/DepthDetector.cpp
    lib/detectors/RoughDistanceDetector.cpp
    lib/detectors/DistanceDetector.cpp
    lib/detectors/FaceDlibDetector.cpp
    lib/detectors/ExtractedInfoIdChecker.cpp
    lib/detectors/ExtractedInfoStringChecker.cpp

    # lib relations
    lib/relations/ImageRangeRelation.cpp
    lib/relations/LogicRelations.cpp
    lib/relations/ThreeDimRangeRelation.cpp
    lib/relations/SpaceRelations.cpp
    )

set(PROJECT_SOURCES
        main.cpp
        gui_eod.cpp
        gui_eod.h
        gui_eod.ui
        ${tinyxml_srcs}
        ${eod_srcs}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt_gui_eod
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qt_gui_eod APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qt_gui_eod SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qt_gui_eod
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(
    qt_gui_eod
    PRIVATE
    lib/include
    lib/detectors/include
    lib/relations/include
    lib/utils/include
    lib/types/include
    tinyxml/
    ${OpenCV_INCLUDE_DIRS}
    #${TinyXML_INCLUDE_DIRS}
    )

target_link_libraries(qt_gui_eod
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBRARIES}
    #${TinyXML_LIBRARIES}
    )

set_target_properties(qt_gui_eod PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt_gui_eod)
endif()
