# This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)

if (WIN32)
  list(APPEND PRIVATE_SOURCES ${sources_windows})
endif()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(connection_patcher ${PRIVATE_SOURCES})

target_link_libraries(connection_patcher
  PRIVATE
    trinity-core-interface
  PUBLIC
    common
)

if(UNITY_BUILDS)
  set_target_properties(connection_patcher
      PROPERTIES UNITY_BUILD ON)
  set_target_properties(connection_patcher
      PROPERTIES
        UNITY_BUILD_MODE BATCH
        UNITY_BUILD_BATCH_SIZE 2)
endif()

if (UNIX)
  install(TARGETS connection_patcher DESTINATION bin)
elseif (WIN32)
  install(TARGETS connection_patcher DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif ()
