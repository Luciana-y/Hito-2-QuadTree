# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\waza_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\waza_autogen.dir\\ParseCache.txt"
  "waza_autogen"
  )
endif()
