# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_routine_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED routine_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(routine_FOUND FALSE)
  elseif(NOT routine_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(routine_FOUND FALSE)
  endif()
  return()
endif()
set(_routine_CONFIG_INCLUDED TRUE)

# output package information
if(NOT routine_FIND_QUIETLY)
  message(STATUS "Found routine: 0.0.0 (${routine_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'routine' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${routine_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(routine_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${routine_DIR}/${_extra}")
endforeach()
