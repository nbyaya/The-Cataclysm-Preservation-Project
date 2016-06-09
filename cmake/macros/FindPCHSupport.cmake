FUNCTION(GET_COMMON_PCH_PARAMS TARGET_NAME_LIST PCH_HEADER PCH_FE INCLUDE_PREFIX)
  GET_FILENAME_COMPONENT(PCH_HEADER_N ${PCH_HEADER} NAME)
  GET_DIRECTORY_PROPERTY(TARGET_INCLUDES INCLUDE_DIRECTORIES)

  # Stores the inherited dependency definitions and include directories
  # from the given target into the given variables
  MACRO(CollectIncludes target inherited_includes inherited_definitions)
    # Append the includes and definitions of the current target to the list
    get_property(included TARGET ${target} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    LIST(APPEND "${inherited_includes}" ${included})
    get_property(definitions TARGET ${target} PROPERTY INTERFACE_COMPILE_DEFINITIONS)
    FOREACH(def ${definitions})
      LIST(APPEND "${inherited_definitions}" "-D${def}")
    ENDFOREACH()
    # Add all inherited link targets which weren't included already
    get_property(links TARGET ${target} PROPERTY INTERFACE_LINK_LIBRARIES)
    # TODO Maybe catch circular dependencies?
    FOREACH(target_link ${links})
      IF(TARGET ${target_link})
        CollectIncludes(${target_link} "${inherited_includes}" "${inherited_definitions}")
      ENDIF()
    ENDFOREACH()
  ENDMACRO()

  FOREACH(TARGET_NAME ${TARGET_NAME_LIST})
    CollectIncludes(${TARGET_NAME} TARGET_INCLUDES TARGET_DEFINITIONS)
  ENDFOREACH()

    IF (PARENT_PACKAGE_INCLUDE_DIRS)
      LIST(REMOVE_DUPLICATES TARGET_INCLUDES)
    ENDIF()
    IF (PARENT_PACKAGE_LIBRARY_DIRS)
      LIST(REMOVE_DUPLICATES TARGET_DEFINITIONS)
    ENDIF()
    # NOTE: Above, in the rare case that none of the subpackages contain any
    # libraries or any include directories, we need to not call
    # LIST(REMOVE_DUPLICATES ...).

  FOREACH(ITEM ${TARGET_INCLUDES})
    LIST(APPEND INCLUDE_FLAGS_LIST "${INCLUDE_PREFIX}\"${ITEM}\" ")
  ENDFOREACH(ITEM)

  SET(PCH_INCLUDES ${TARGET_INCLUDES} PARENT_SCOPE)
  SET(PCH_DEFINITIONS ${TARGET_DEFINITIONS} PARENT_SCOPE)
  SET(PCH_HEADER_NAME ${PCH_HEADER_N} PARENT_SCOPE)
  SET(PCH_HEADER_OUT ${CMAKE_CURRENT_BINARY_DIR}/${PCH_HEADER_N}.${PCH_FE} PARENT_SCOPE)
  SET(INCLUDE_FLAGS ${INCLUDE_FLAGS_LIST} PARENT_SCOPE)
ENDFUNCTION(GET_COMMON_PCH_PARAMS)

FUNCTION(GENERATE_CXX_PCH_COMMAND TARGET_NAME_LIST INCLUDE_FLAGS IN PCH_SRC OUT)
  include_directories(${PCH_INCLUDES})
  add_definitions(${PCH_DEFINITIONS})

  IF (CMAKE_BUILD_TYPE)
    STRING(TOUPPER _${CMAKE_BUILD_TYPE} CURRENT_BUILD_TYPE)
  ENDIF ()

  SET(COMPILE_FLAGS ${CMAKE_CXX_FLAGS${CURRENT_BUILD_TYPE}})
  LIST(APPEND COMPILE_FLAGS ${CMAKE_CXX_FLAGS})

  IF ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    IF (NOT "${CMAKE_OSX_ARCHITECTURES}" STREQUAL "")
      LIST(APPEND COMPILE_FLAGS "-arch ${CMAKE_OSX_ARCHITECTURES}")
    ENDIF ()
    IF (NOT "${CMAKE_OSX_SYSROOT}" STREQUAL "")
      LIST(APPEND COMPILE_FLAGS "-isysroot ${CMAKE_OSX_SYSROOT}")
    ENDIF ()
    IF (NOT "${CMAKE_OSX_DEPLOYMENT_TARGET}" STREQUAL "")
      LIST(APPEND COMPILE_FLAGS "-mmacosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}")
    ENDIF ()
  ENDIF ()

  GET_DIRECTORY_PROPERTY(TARGET_DEFINITIONS COMPILE_DEFINITIONS)
  FOREACH(ITEM ${TARGET_DEFINITIONS})
    LIST(APPEND DEFINITION_FLAGS "-D${ITEM} ")
  ENDFOREACH(ITEM)

  SEPARATE_ARGUMENTS(COMPILE_FLAGS)
  SEPARATE_ARGUMENTS(INCLUDE_FLAGS)
  SEPARATE_ARGUMENTS(DEFINITION_FLAGS)

  GET_FILENAME_COMPONENT(PCH_SRC_N ${PCH_SRC} NAME)
  ADD_LIBRARY(${PCH_SRC_N}_dephelp MODULE ${PCH_SRC})

  ADD_CUSTOM_COMMAND(
    OUTPUT ${OUT}
    COMMAND ${CMAKE_CXX_COMPILER}
    ARGS ${DEFINITION_FLAGS} ${COMPILE_FLAGS} ${INCLUDE_FLAGS} -x c++-header ${IN} -o ${OUT}
    DEPENDS ${IN} ${PCH_SRC_N}_dephelp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  ADD_CUSTOM_TARGET(generate_${PCH_SRC_N}
    DEPENDS ${OUT}
  )

  FOREACH(TARGET_NAME ${TARGET_NAME_LIST})
    ADD_DEPENDENCIES(${TARGET_NAME} generate_${PCH_SRC_N})
  ENDFOREACH()

ENDFUNCTION(GENERATE_CXX_PCH_COMMAND)

FUNCTION(ADD_CXX_PCH_GCC TARGET_NAME_LIST PCH_HEADER PCH_SOURCE)
  GET_COMMON_PCH_PARAMS("${TARGET_NAME_LIST}" ${PCH_HEADER} "gch" "-I")
  GENERATE_CXX_PCH_COMMAND("${TARGET_NAME_LIST}" "${INCLUDE_FLAGS}" ${PCH_HEADER} ${PCH_SOURCE} ${PCH_HEADER_OUT})

  FOREACH(TARGET_NAME ${TARGET_NAME_LIST})
    SET_TARGET_PROPERTIES(
      ${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "-include ${CMAKE_CURRENT_BINARY_DIR}/${PCH_HEADER_NAME}"
    )
  ENDFOREACH()
ENDFUNCTION(ADD_CXX_PCH_GCC)

FUNCTION(ADD_CXX_PCH_CLANG TARGET_NAME_LIST PCH_HEADER PCH_SOURCE)
  GET_COMMON_PCH_PARAMS("${TARGET_NAME_LIST}" ${PCH_HEADER} "pch" "-I")
  GENERATE_CXX_PCH_COMMAND("${TARGET_NAME_LIST}" "${INCLUDE_FLAGS}" ${PCH_HEADER} ${PCH_SOURCE} ${PCH_HEADER_OUT})

  FOREACH(TARGET_NAME ${TARGET_NAME_LIST})
    SET_TARGET_PROPERTIES(
      ${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "-include-pch ${PCH_HEADER_OUT}"
    )
  ENDFOREACH()
ENDFUNCTION(ADD_CXX_PCH_CLANG)

FUNCTION(ADD_CXX_PCH_MSVC TARGET_NAME_LIST PCH_HEADER PCH_SOURCE)
  GET_COMMON_PCH_PARAMS("${TARGET_NAME_LIST}" ${PCH_HEADER} "pch" "/I")

  FOREACH(TARGET_NAME ${TARGET_NAME_LIST})
    SET_TARGET_PROPERTIES(
      ${TARGET_NAME} PROPERTIES
      COMPILE_FLAGS "/FI${PCH_HEADER_NAME} /Yu${PCH_HEADER_NAME}"
    )
  ENDFOREACH()

  SET_SOURCE_FILES_PROPERTIES(
    ${PCH_SOURCE} PROPERTIES
    COMPILE_FLAGS "/Yc${PCH_HEADER_NAME}"
  )
ENDFUNCTION(ADD_CXX_PCH_MSVC)

FUNCTION(ADD_CXX_PCH_XCODE TARGET_NAME_LIST PCH_HEADER PCH_SOURCE)
  FOREACH(TARGET_NAME ${TARGET_NAME_LIST})
    SET_TARGET_PROPERTIES("${TARGET_NAME}" PROPERTIES
      XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER YES
      XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/${PCH_HEADER}"
    )
  ENDFOREACH()
ENDFUNCTION(ADD_CXX_PCH_XCODE)

FUNCTION(ADD_CXX_PCH TARGET_NAME_LIST PCH_HEADER PCH_SOURCE)
  IF (MSVC)
    ADD_CXX_PCH_MSVC("${TARGET_NAME_LIST}" ${PCH_HEADER} ${PCH_SOURCE})
  ELSEIF ("${CMAKE_GENERATOR}" MATCHES "Xcode")
    ADD_CXX_PCH_XCODE("${TARGET_NAME_LIST}" ${PCH_HEADER} ${PCH_SOURCE})
  ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    ADD_CXX_PCH_CLANG("${TARGET_NAME_LIST}" ${PCH_HEADER} ${PCH_SOURCE})
  ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    ADD_CXX_PCH_GCC("${TARGET_NAME_LIST}" ${PCH_HEADER} ${PCH_SOURCE})
  ENDIF ()
ENDFUNCTION(ADD_CXX_PCH)
