## antimicrox Gamepad to KB+M event mapper
## Copyright (C) 2015 Travis Nickles <nickles.travis@gmail.com>
## Copyright (C) 2020 Jagoda Górska <juliagoda.pl@protonmail.com>
## Copyright (C) 2020 Paweł Kotiuk <kotiuk@zohomail.eu>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 3.6)
set(QT_REQUIRED_VERSION 5.8)
set(SDL_REQUIRED_VERSION 2.0.6)
project(antimicrox)

if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

if(UNIX)
    find_package(ECM REQUIRED NO_MODULE)
    set(CMAKE_MODULE_PATH ${ECM_MODULE_DIR})
endif(UNIX)

# for systems like Rapsberry Pi
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/arm-linux-gnueabihf")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" "${CMAKE_ROOT}/Modules")

set(CMAKE_INCLUDE_CURRENT_DIR ON)


if (UNIX AND NOT APPLE)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "force cmake prefix to be set for /usr" FORCE)
    endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(UNIX AND NOT APPLE)

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)

if (UNIX)
    include(ECMInstallIcons)
endif(UNIX)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)


if(COMPILER_SUPPORTS_CXX11)
    if (NOT WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wstrict-null-sentinel -Wstrict-overflow=5 -Wundef -Wno-unused -std=c++11")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# The version number.
set(ANTIMICROX_MAJOR_VERSION 3)
set(ANTIMICROX_MINOR_VERSION 1)
set(ANTIMICROX_PATCH_VERSION 3)

option(WITH_TESTS "Allow tests for classes" OFF)

if(WITH_TESTS)
    message("Tests enabled")
endif(WITH_TESTS)

if(UNIX)
    option(WITH_X11 "Compile with support for X11." ON)
    option(WITH_UINPUT "Compile with support for uinput. uinput will be usable to simulate events." ON)
    option(WITH_XTEST "Compile with support for XTest.  XTest will be usable to simulate events." ON)
    option(APPDATA "Build project with AppData file support." ON)
endif(UNIX)

option(UPDATE_TRANSLATIONS "Call lupdate to update translation files from source." OFF)
option(TRANS_KEEP_OBSOLETE "Add -noobsolete option to lupdate command to get rid of old text entries" OFF)
option(ATTACH_FAKE_CLASSES "Fake classes can be used in application to tests functionalities" OFF)


# Use pkg-config to find SDL library.
if(UNIX)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(SDL2 REQUIRED sdl2)

    if(WITH_X11)
        find_package(X11 REQUIRED)
    endif(WITH_X11)

    if(WITH_XTEST AND NOT WITH_X11)
        set(WITH_XTEST OFF)
        message("Cannot use XTest without X11. Disabling XTest support.")
    endif(WITH_XTEST AND NOT WITH_X11)

    if(WITH_XTEST)
        message("XTest support allowed for simulating events.")
    endif(WITH_XTEST)

    if(WITH_UINPUT)
        message("uinput support allowed for simulating events.")
    endif(WITH_UINPUT)

    if(NOT WITH_XTEST AND NOT WITH_UINPUT)
        message(FATAL_ERROR "No system is defined for simulating events.")
    endif(NOT WITH_XTEST AND NOT WITH_UINPUT)
endif(UNIX)

set(antimicrox_MAIN
    src/main.cpp
)

set(antimicrox_SOURCES
    src/aboutdialog.cpp
    src/addeditautoprofiledialog.cpp
    src/advancebuttondialog.cpp
    src/advancestickassignmentdialog.cpp
    src/antimicrosettings.cpp
    src/antkeymapper.cpp
    src/applaunchhelper.cpp
    src/autoprofileinfo.cpp
    src/axiseditdialog.cpp
    src/axisvaluebox.cpp
    src/buttoneditdialog.cpp
    src/calibration.cpp
    src/commandlineutility.cpp
    src/common.cpp
    src/dpadcontextmenu.cpp
    src/dpadeditdialog.cpp
    src/dpadpushbutton.cpp
    src/dpadpushbuttongroup.cpp
    src/editalldefaultautoprofiledialog.cpp
    src/event.cpp
    src/eventhandlerfactory.cpp
    src/eventhandlers/baseeventhandler.cpp
    src/extraprofilesettingsdialog.cpp
    src/flashbuttonwidget.cpp
    src/gamecontroller/gamecontroller.cpp
    src/gamecontroller/gamecontrollerdpad.cpp
    src/gamecontroller/gamecontrollerset.cpp
    src/gamecontroller/gamecontrollertrigger.cpp
    src/gamecontroller/gamecontrollertriggerbutton.cpp
    src/gamecontroller/xml/gamecontrollerdpadxml.cpp
    src/gamecontroller/xml/gamecontrollertriggerxml.cpp
    src/gamecontroller/xml/gamecontrollerxml.cpp
    src/gamecontrollerexample.cpp
    src/gamecontrollermappingdialog.cpp
    src/globalvariables.cpp
    src/inputdaemon.cpp
    src/inputdevice.cpp
    src/inputdevicebitarraystatus.cpp
    src/joyaxis.cpp
    src/joyaxiscontextmenu.cpp
    src/joyaxiswidget.cpp
    src/joybutton.cpp
    src/joybuttoncontextmenu.cpp
    src/joybuttonmousehelper.cpp
    src/joybuttonslot.cpp
    src/joybuttonstatusbox.cpp
    src/joybuttontypes/joyaxisbutton.cpp
    src/joybuttontypes/joycontrolstickbutton.cpp
    src/joybuttontypes/joycontrolstickmodifierbutton.cpp
    src/joybuttontypes/joydpadbutton.cpp
    src/joybuttontypes/joygradientbutton.cpp
    src/joybuttonwidget.cpp
    src/joycontrolstick.cpp
    src/joycontrolstickbuttonpushbutton.cpp
    src/joycontrolstickcontextmenu.cpp
    src/joycontrolstickeditdialog.cpp
    src/joycontrolstickpushbutton.cpp
    src/joycontrolstickstatusbox.cpp
    src/joydpad.cpp
    src/joydpadbuttonwidget.cpp
    src/joystick.cpp
    src/joystickstatuswindow.cpp
    src/joytabwidget.cpp
    src/joytabwidgetcontainer.cpp
    src/keyboard/virtualkeyboardmousewidget.cpp
    src/keyboard/virtualkeypushbutton.cpp
    src/keyboard/virtualmousepushbutton.cpp
    src/localantimicroserver.cpp
    src/logger.cpp
    src/mainsettingsdialog.cpp
    src/mainwindow.cpp
    src/messagehandler.cpp
    src/mousedialog/mouseaxissettingsdialog.cpp
    src/mousedialog/mousebuttonsettingsdialog.cpp
    src/mousedialog/mousecontrolsticksettingsdialog.cpp
    src/mousedialog/mousedpadsettingsdialog.cpp
    src/mousedialog/springmoderegionpreview.cpp
    src/mousedialog/uihelpers/mouseaxissettingsdialoghelper.cpp
    src/mousedialog/uihelpers/mousebuttonsettingsdialoghelper.cpp
    src/mousedialog/uihelpers/mousecontrolsticksettingsdialoghelper.cpp
    src/mousedialog/uihelpers/mousedpadsettingsdialoghelper.cpp
    src/mousehelper.cpp
    src/mousesettingsdialog.cpp
    src/qkeydisplaydialog.cpp
    src/qtkeymapperbase.cpp
    src/quicksetdialog.cpp
    src/sdleventreader.cpp
    src/setaxisthrottledialog.cpp
    src/setjoystick.cpp
    src/setnamesdialog.cpp
    src/simplekeygrabberbutton.cpp
    src/slotitemlistwidget.cpp
    src/stickpushbuttongroup.cpp
    src/uihelpers/advancebuttondialoghelper.cpp
    src/uihelpers/buttoneditdialoghelper.cpp
    src/uihelpers/dpadcontextmenuhelper.cpp
    src/uihelpers/dpadeditdialoghelper.cpp
    src/uihelpers/gamecontrollermappingdialoghelper.cpp
    src/uihelpers/joyaxiscontextmenuhelper.cpp
    src/uihelpers/joycontrolstickcontextmenuhelper.cpp
    src/uihelpers/joycontrolstickeditdialoghelper.cpp
    src/uihelpers/joytabwidgethelper.cpp
    src/vdpad.cpp
    src/xml/inputdevicexml.cpp
    src/xml/joyaxisxml.cpp
    src/xml/joybuttonslotxml.cpp
    src/xml/joybuttonxml.cpp
    src/xml/joydpadxml.cpp
    src/xml/setjoystickxml.cpp
    src/xmlconfigmigration.cpp
    src/xmlconfigreader.cpp
    src/xmlconfigwriter.cpp
)

set(antimicrox_HEADERS
    src/aboutdialog.h
    src/addeditautoprofiledialog.h
    src/advancebuttondialog.h
    src/advancestickassignmentdialog.h
    src/antimicrosettings.h
    src/antkeymapper.h
    src/applaunchhelper.h
    src/autoprofileinfo.h
    src/axiseditdialog.h
    src/axisvaluebox.h
    src/buttoneditdialog.h
    src/calibration.h
    src/commandlineutility.h
    src/dpadcontextmenu.h
    src/dpadeditdialog.h
    src/dpadpushbutton.h
    src/dpadpushbuttongroup.h
    src/editalldefaultautoprofiledialog.h
    src/eventhandlerfactory.h
    src/eventhandlers/baseeventhandler.h
    src/extraprofilesettingsdialog.h
    src/flashbuttonwidget.h
    src/gamecontroller/gamecontroller.h
    src/gamecontroller/gamecontrollerdpad.h
    src/gamecontroller/gamecontrollerset.h
    src/gamecontroller/gamecontrollertrigger.h
    src/gamecontroller/gamecontrollertriggerbutton.h
    src/gamecontroller/xml/gamecontrollerdpadxml.h
    src/gamecontroller/xml/gamecontrollertriggerxml.h
    src/gamecontroller/xml/gamecontrollerxml.h
    src/gamecontrollerexample.h
    src/gamecontrollermappingdialog.h
    src/globalvariables.h
    src/inputdaemon.h
    src/inputdevice.h
    src/inputdevicebitarraystatus.h
    src/joyaxis.h
    src/joyaxiscontextmenu.h
    src/joyaxiswidget.h
    src/joybutton.h
    src/joybuttoncontextmenu.h
    src/joybuttonmousehelper.h
    src/joybuttonslot.h
    src/joybuttonstatusbox.h
    src/joybuttontypes/joyaxisbutton.h
    src/joybuttontypes/joycontrolstickbutton.h
    src/joybuttontypes/joycontrolstickmodifierbutton.h
    src/joybuttontypes/joydpadbutton.h
    src/joybuttontypes/joygradientbutton.h
    src/joybuttonwidget.h
    src/joycontrolstick.h
    src/joycontrolstickbuttonpushbutton.h
    src/joycontrolstickcontextmenu.h
    src/joycontrolstickeditdialog.h
    src/joycontrolstickpushbutton.h
    src/joycontrolstickstatusbox.h
    src/joydpad.h
    src/joydpadbuttonwidget.h
    src/joystick.h
    src/joystickstatuswindow.h
    src/joytabwidget.h
    src/joytabwidgetcontainer.h
    src/keyboard/virtualkeyboardmousewidget.h
    src/keyboard/virtualkeypushbutton.h
    src/keyboard/virtualmousepushbutton.h
    src/localantimicroserver.h
    src/logger.h
    src/mainsettingsdialog.h
    src/mainwindow.h
    src/messagehandler.h
    src/mousedialog/mouseaxissettingsdialog.h
    src/mousedialog/mousebuttonsettingsdialog.h
    src/mousedialog/mousecontrolsticksettingsdialog.h
    src/mousedialog/mousedpadsettingsdialog.h
    src/mousedialog/springmoderegionpreview.h
    src/mousedialog/uihelpers/mouseaxissettingsdialoghelper.h
    src/mousedialog/uihelpers/mousebuttonsettingsdialoghelper.h
    src/mousedialog/uihelpers/mousecontrolsticksettingsdialoghelper.h
    src/mousedialog/uihelpers/mousedpadsettingsdialoghelper.h
    src/mousehelper.h
    src/mousesettingsdialog.h
    src/qkeydisplaydialog.h
    src/qtkeymapperbase.h
    src/quicksetdialog.h
    src/sdleventreader.h
    src/setaxisthrottledialog.h
    src/setjoystick.h
    src/setnamesdialog.h
    src/simplekeygrabberbutton.h
    src/slotitemlistwidget.h
    src/stickpushbuttongroup.h
    src/uihelpers/advancebuttondialoghelper.h
    src/uihelpers/buttoneditdialoghelper.h
    src/uihelpers/dpadcontextmenuhelper.h
    src/uihelpers/dpadeditdialoghelper.h
    src/uihelpers/gamecontrollermappingdialoghelper.h
    src/uihelpers/joyaxiscontextmenuhelper.h
    src/uihelpers/joycontrolstickcontextmenuhelper.h
    src/uihelpers/joycontrolstickeditdialoghelper.h
    src/uihelpers/joytabwidgethelper.h
    src/vdpad.h
    src/xml/inputdevicexml.h
    src/xml/joyaxisxml.h
    src/xml/joybuttonslotxml.h
    src/xml/joybuttonxml.h
    src/xml/joydpadxml.h
    src/xml/setjoystickxml.h
    src/xmlconfigmigration.h
    src/xmlconfigreader.h
    src/xmlconfigwriter.h
)

if(ATTACH_FAKE_CLASSES)
    LIST(APPEND antimicrox_SOURCES
        src/fakeclasses/xbox360wireless.cpp
    )

    LIST(APPEND antimicrox_HEADERS
        src/fakeclasses/xbox360wireless.h
    )
endif(ATTACH_FAKE_CLASSES)

set(antimicrox_FORMS
    src/aboutdialog.ui
    src/addeditautoprofiledialog.ui
    src/advancebuttondialog.ui
    src/advancestickassignmentdialog.ui
    src/axiseditdialog.ui
    src/buttoneditdialog.ui
    src/calibration.ui
    src/capturedwindowinfodialog.ui
    src/dpadeditdialog.ui
    src/editalldefaultautoprofiledialog.ui
    src/extraprofilesettingsdialog.ui
    src/gamecontrollermappingdialog.ui
    src/joycontrolstickeditdialog.ui
    src/joystickstatuswindow.ui
    src/mainsettingsdialog.ui
    src/mainwindow.ui
    src/mousesettingsdialog.ui
    src/qkeydisplaydialog.ui
    src/quicksetdialog.ui
    src/setaxisthrottledialog.ui
    src/setnamesdialog.ui
)

set(antimicrox_RESOURCES src/resources.qrc)

# Platform dependent files.
if(UNIX)
    if(WITH_X11)
        LIST(APPEND antimicrox_SOURCES src/x11extras.cpp
             src/qtx11keymapper.cpp
             src/unixcapturewindowutility.cpp
             src/autoprofilewatcher.cpp
             src/capturedwindowinfodialog.cpp
             src/qglobalshortcut/qglobalshortcut.cc
        )
        LIST(APPEND antimicrox_HEADERS src/x11extras.h
             src/qtx11keymapper.h
             src/unixcapturewindowutility.h
             src/autoprofilewatcher.h
             src/capturedwindowinfodialog.h
             src/qglobalshortcut/qglobalshortcut.h
        )

        if(WITH_XTEST)
            LIST(APPEND antimicrox_SOURCES src/eventhandlers/xtesteventhandler.cpp)
            LIST(APPEND antimicrox_HEADERS src/eventhandlers/xtesteventhandler.h)
        endif(WITH_XTEST)
    endif(WITH_X11)

    if(WITH_UINPUT)
        LIST(APPEND antimicrox_SOURCES src/qtuinputkeymapper.cpp
             src/uinputhelper.cpp
             src/eventhandlers/uinputeventhandler.cpp
        )
        LIST(APPEND antimicrox_HEADERS src/qtuinputkeymapper.h
             src/uinputhelper.h
             src/eventhandlers/uinputeventhandler.h
        )
    endif(WITH_UINPUT)
endif(UNIX)

if(UNIX)
    if(WITH_X11)
        add_definitions(-DWITH_X11)
    endif(WITH_X11)

    if(WITH_XTEST)
        add_definitions(-DWITH_XTEST)
    endif(WITH_XTEST)

    if(WITH_UINPUT)
        add_definitions(-DWITH_UINPUT)
    endif(WITH_UINPUT)
endif(UNIX)

if (UNIX)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Gui REQUIRED)
    find_package(Qt5Network REQUIRED)
    find_package(Qt5LinguistTools REQUIRED)
    find_package(Qt5Concurrent REQUIRED)
    #find_package(Qt5Test REQUIRED)

    if (WITH_X11)
        find_package(Qt5X11Extras REQUIRED)
    endif()

    QT5_WRAP_CPP(antimicrox_HEADERS_MOC ${antimicrox_HEADERS})
    QT5_WRAP_UI(antimicrox_FORMS_HEADERS ${antimicrox_FORMS})
    QT5_ADD_RESOURCES(antimicrox_RESOURCES_RCC ${antimicrox_RESOURCES})

    add_subdirectory("share/antimicrox/translations")

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif(UNIX)

if(UNIX)
    list(APPEND QT_LIBS Qt5::Widgets)
    list(APPEND QT_LIBS Qt5::Core)
    list(APPEND QT_LIBS Qt5::Gui)
    list(APPEND QT_LIBS Qt5::Network)
    list(APPEND QT_LIBS Qt5::Concurrent)

    if(WITH_X11)
        LIST(APPEND EXTRA_LIBS ${X11_X11_LIB})
        LIST(APPEND EXTRA_LIBS ${X11_Xi_LIB})
        LIST(APPEND EXTRA_LIBS ${X11_xcb_LIB})
        LIST(APPEND QT_LIBS Qt5::X11Extras)
    endif(WITH_X11)

    if(WITH_XTEST)
        LIST(APPEND EXTRA_LIBS ${X11_XTest_LIB})
    endif(WITH_XTEST)
endif (UNIX)


include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    "${CMAKE_CURRENT_BINARY_DIR}/src"
    ${PROJECT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/src"
)


if(UNIX)
    # Store executable in a bin subdir. Needed here so translations can be loaded.
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif(UNIX)

if(UNIX)
    add_executable(antimicrox
        ${antimicrox_MAIN}
        ${antimicrox_HEADERS_MOC}
        ${antimicrox_SOURCES}
        ${antimicrox_FORMS_HEADERS}
        ${antimicrox_RESOURCES_RCC}
    )

    target_link_libraries(antimicrox ${QT_LIBS} ${SDL2_LIBRARIES} ${EXTRA_LIBS})
endif(UNIX)


# Specify out directory for final executable.
if(UNIX)
    install(TARGETS antimicrox RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
    install(FILES ${antimicrox_HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/antimicrox")
endif(UNIX)

if(UNIX)
    ecm_install_icons(ICONS src/icons/application/16x16/apps/16-apps-io.github.antimicrox.antimicrox.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)
    ecm_install_icons(ICONS src/icons/application/24x24/apps/24-apps-io.github.antimicrox.antimicrox.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)
    ecm_install_icons(ICONS src/icons/application/32x32/apps/32-apps-io.github.antimicrox.antimicrox.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)
    ecm_install_icons(ICONS src/icons/application/48x48/apps/48-apps-io.github.antimicrox.antimicrox.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)
    ecm_install_icons(ICONS src/icons/application/64x64/apps/64-apps-io.github.antimicrox.antimicrox.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)
    ecm_install_icons(ICONS src/icons/application/128x128/apps/128-apps-io.github.antimicrox.antimicrox.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)
    ecm_install_icons(ICONS src/icons/application/scalable/apps/sc-apps-io.github.antimicrox.antimicrox.svg DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)

    ecm_install_icons(ICONS src/images/breeze_themed/48-apps-antimicrox_trayicon.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons THEME breeze)
    ecm_install_icons(ICONS src/images/48-apps-antimicrox_trayicon.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons)

    install(FILES CHANGELOG.md DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/doc/antimicrox")
    install(FILES other/io.github.antimicrox.antimicrox.desktop DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
    install(FILES other/io.github.antimicrox.antimicrox.xml DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/mime/packages")

    # Copy current Changelog file to location that the resource file expects.
    install(FILES CHANGELOG.md DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/antimicrox")
endif(UNIX)

# Add man page for *nix platforms.
if(UNIX)
    add_subdirectory(other)

    # Only way to force install target to be dependent on manpage.
    install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} manpage WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\")")

    if(APPDATA)
        # Only way to force install target to be dependent on appdata.
        install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} appdata WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\")")
    endif(APPDATA)
endif(UNIX)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)


# Only way to force install target to be dependent on updateqm.
install(CODE "execute_process(COMMAND ${CMAKE_BUILD_TOOL} updateqm WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\")")

# Use this to use some variables created here in the actual project.
# Modify the config.h.in file using the appropriate variables.
configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/CHANGELOG.md DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/src/)

if(WITH_TESTS)
    enable_testing ()
    add_subdirectory (tests)
endif(WITH_TESTS)


#building package using CPack
include(InstallRequiredSystemLibraries)


set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Program used to map keyboard keys and mouse controls to a gamepad.")


set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_VERSION_MAJOR "${ANTIMICROX_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${ANTIMICROX_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${ANTIMICROX_PATCH_VERSION}")

set(CPACK_PACKAGE_FILE_NAME "antimicrox-${ANTIMICROX_MAJOR_VERSION}.${ANTIMICROX_MINOR_VERSION}.${ANTIMICROX_PATCH_VERSION}-${CMAKE_HOST_SYSTEM_PROCESSOR}")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

if(UNIX)
    set(CPACK_STRIP_FILES "")
    set(CPACK_SOURCE_STRIP_FILES "")
endif()

# Building .deb package
if(CPACK_GENERATOR STREQUAL "DEB")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "qtbase5-dev (>= ${QT_REQUIRED_VERSION}), libsdl2-2.0-0 (>= ${SDL_REQUIRED_VERSION}), libqt5x11extras5, libc6")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "pktiuk <kotiuk@zohomail.eu>")
  set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "AntiMicroX is a graphical program used to map gamepad buttons to keyboard, mouse, scripts and macros.

It is a new fork of discontinued AntiMicro.")

  message("Preparing documentation for DEB package")
  add_custom_target(package_docummentation ALL)

  #Compress changelog and save it as share/doc/xournalpp/changelog.Debian.gz
  add_custom_command(TARGET package_docummentation PRE_BUILD
  COMMAND gzip -c -9 -n "${PROJECT_SOURCE_DIR}/CHANGELOG.md" > "changelog.Debian.gz" VERBATIM)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/changelog.Debian.gz" DESTINATION "share/doc/antimicrox/")

  #Strip binaries from unnecessary notes, comments, etc
  add_custom_command(TARGET antimicrox POST_BUILD
  COMMAND strip --strip-unneeded --remove-section=.comment --remove-section=.note "${CMAKE_CURRENT_BINARY_DIR}/bin/antimicrox" VERBATIM)
endif()

set(CPACK_PACKAGE_EXECUTABLES "antimicrox" "antimicrox")
include(CPack)
