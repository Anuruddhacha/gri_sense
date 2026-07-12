# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  [[CMakeFiles\agri_sense_dash_autogen.dir\AutogenUsed.txt]]
  [[CMakeFiles\agri_sense_dash_autogen.dir\ParseCache.txt]]
  "agri_sense_dash_autogen"
  )
endif()
