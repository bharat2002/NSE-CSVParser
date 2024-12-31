# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\StockDataVisualizer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\StockDataVisualizer_autogen.dir\\ParseCache.txt"
  "StockDataVisualizer_autogen"
  )
endif()
