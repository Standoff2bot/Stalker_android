# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "Externals/LuaJIT-proj/HostBuildTools/buildvm"
  "Externals/LuaJIT-proj/HostBuildTools/minilua"
  )
endif()
