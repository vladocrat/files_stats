cmake_minimum_required(VERSION 3.16)

project(file_stats VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appfile_stats
    main.cpp
)

qt_add_qml_module(appfile_stats
    URI file_stats
    VERSION 1.0
    QML_FILES Main.qml
    QML_FILES Footer.qml
    SOURCES filestatscontroller.h filestatscontroller.cpp
    SOURCES pimpl.h
    SOURCES fileparser.h fileparser.cpp
    SOURCES worker.h worker.cpp
    SOURCES
    QML_FILES
    SOURCES graph.h graph.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfile_stats PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfile_stats
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appfile_stats
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appfile_stats
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
