# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Mess_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Mess_autogen.dir\\ParseCache.txt"
  "Mess_autogen"
  )
endif()
