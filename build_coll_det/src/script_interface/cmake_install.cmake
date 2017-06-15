# Install script for directory: /Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/src/script_interface

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RELEASE")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/script_interface/libEspressoScriptInterface.4.dylib"
    "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/script_interface/libEspressoScriptInterface.dylib"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libEspressoScriptInterface.4.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libEspressoScriptInterface.dylib"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/actor"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/object-in-fluid"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/immersed_boundary"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/shapes"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/constraints"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/utils"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/correlators"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -delete_rpath "/Users/bindgens2/Documents/espresso_dev/espresso_rudolf/espresso/build_coll_det/src/core/observables"
        "${file}")
      execute_process(COMMAND /opt/local/bin/install_name_tool
        -add_rpath "/usr/local/lib"
        "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

