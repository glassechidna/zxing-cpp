function(_iconv_find)
  if(ICONV_ROOT)
    list(APPEND iconv_roots ${ICONV_ROOT})
  else()
    if(NOT "$ENV{ICONV_ROOT}" STREQUAL "")
      file(TO_CMAKE_PATH "$ENV{ICONV_ROOT}" NATIVE_PATH)
      list(APPEND iconv_roots "${NATIVE_PATH}")
      set(ICONV_ROOT "${NATIVE_PATH}"
        CACHE PATH "Location of the Iconv installation" FORCE)
    endif()
  endif()

  list(APPEND iconv_library_suffixes "lib")
  list(APPEND iconv_include_suffixes "include")

  find_path(ICONV_INCLUDE_DIR
    NAMES "iconv.h"
    HINTS ${iconv_roots}
    PATH_SUFFIXES ${iconv_include_suffixes}
    DOC "Iconv include directory")
  set(ICONV_INCLUDE_DIR "${ICONV_INCLUDE_DIR}" PARENT_SCOPE)

  find_library(ICONV_LIBRARY
    NAMES
      iconv
      libiconv
      libiconv2
    HINTS ${iconv_roots}
    PATH_SUFFIXES ${iconv_library_suffixes}
    DOC "Iconv library")
  set(ICONV_LIBRARY "${ICONV_LIBRARY}" PARENT_SCOPE)

  if(ICONV_INCLUDE_DIR AND NOT ICONV_LIBRARY)
    include(CheckFunctionExists)
    check_function_exists(iconv HAVE_ICONV_IN_LIBC)
    if(HAVE_ICONV_IN_LIBC)
      set(HAVE_ICONV_IN_LIBC "${HAVE_ICONV_IN_LIBC}" PARENT_SCOPE)
      set(ICONV_LIBRARY "integrated in standard library" PARENT_SCOPE)
    endif()
  endif()

  if(ICONV_INCLUDE_DIR AND ICONV_LIBRARY)
    set(ICONV_FOUND ON PARENT_SCOPE)
  endif()
endfunction()

_iconv_find()

if(ICONV_FOUND)
  if(NOT ICONV_FIND_QUIETLY)
    message(STATUS "Found iconv library: ${ICONV_LIBRARY}")
  endif()

  if(HAVE_ICONV_IN_LIBC)
    set(_lib_type INTERFACE)
  else()
    set(_lib_type UNKNOWN)
  endif()

  add_library(Iconv::Iconv ${_lib_type} IMPORTED)
  set_target_properties(Iconv::Iconv PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${ICONV_INCLUDE_DIR}")

  if(NOT HAVE_ICONV_IN_LIBC)
    set_target_properties(Iconv::Iconv PROPERTIES
      IMPORTED_LOCATION "${ICONV_LIBRARY}")
  endif()

  unset(_lib_type)
else()
  if(ICONV_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find iconv library")
  endif()
endif()

mark_as_advanced(ICONV_LIBRARY ICONV_INCLUDE_DIR)
