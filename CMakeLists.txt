cmake_minimum_required(VERSION 3.12)

project(
    scalr
    LANGUAGES CXX
    VERSION 0.3.0
    DESCRIPTION "Physical quantity/unit representation and manipulation library for Modern C++"
    HOMEPAGE_URL "https://github.com/doganulus/scalr"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
include(GNUInstallDirs)

set(SCALR_TARGET_NAME               "${PROJECT_NAME}")
set(SCALR_TARGETS_EXPORT_NAME       "${PROJECT_NAME}Targets")

add_library(${SCALR_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${SCALR_TARGET_NAME} ALIAS ${SCALR_TARGET_NAME})

target_include_directories(
  ${SCALR_TARGET_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Install
install(
  TARGETS ${SCALR_TARGET_NAME}
  EXPORT ${SCALR_TARGETS_EXPORT_NAME}
  ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
