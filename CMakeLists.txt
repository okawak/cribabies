if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "DO NOT BUILD in-tree.")
endif()

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(cribabies)

# Default installation dir
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "..." FORCE)
endif()

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# subdirectory
add_subdirectory(src)


# config file
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.thiscribabies.sh.in"
    "${CMAKE_SOURCE_DIR}/thiscribabies.sh"
    @ONLY
)

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)
