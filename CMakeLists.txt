# CMakeLists.txt 20111224 (C) markc@renta.net (AGPLv3)
#
# mkdir build && cd build
# cmake .. && make
# sudo make install

project(silkjs)
cmake_minimum_required(VERSION 2.6)
include_directories(${CMAKE_SOURCE_DIR}/src)
set(SRCS
  src/base64.cpp
  src/buffer.cpp
  src/console.cpp
  src/fs.cpp
  src/gd.cpp
  src/global.cpp
  src/http.cpp
  src/logfile.cpp
  src/main.cpp
  src/md5.cpp
  src/mysql.cpp
  src/ncurses.cpp
  src/net.cpp
  src/process.cpp
  src/sem.cpp
  src/sqlite3.cpp
  src/v8.cpp
)
set(HDRS src/SilkJS.h)
add_executable(silkjs ${SRCS})
target_link_libraries(silkjs v8 mysqlclient mm gd ncurses ssl pthread sqlite3)
install(TARGETS silkjs DESTINATION /usr/bin)
