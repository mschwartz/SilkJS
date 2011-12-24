# CMakeLists.txt 20111224 (C) markc@renta.net (AGPLv3)
#
# mkdir build && cd build
# cmake .. && make
# sudo make install

project(silkjs)
cmake_minimum_required(VERSION 2.6)
include_directories(${CMAKE_SOURCE_DIR})
set(SRCS
  base64.cpp
  buffer.cpp
  console.cpp
  fs.cpp
  gd.cpp
  global.cpp
  http.cpp
  logfile.cpp
  main.cpp
  md5.cpp
  mysql.cpp
  ncurses.cpp
  net.cpp
  process.cpp
  sem.cpp
  sqlite3.cpp
  v8.cpp
)
set(HDRS SilkJS.h)
add_executable(silkjs ${SRCS})
target_link_libraries(silkjs v8 mysqlclient mm gd ncurses ssl pthread sqlite3)
install(TARGETS silkjs DESTINATION /usr/bin)
