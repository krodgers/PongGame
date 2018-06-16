cmake_minimum_required(VERSION 2.8.4)
project(PongGame)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

INCLUDE_DIRECTORIES(/home/kevin/code/jsoncpp-pre-C-11/include)
INCLUDE_DIRECTORIES(/home/kevin/code/ics167/PongGame/server/include)
LINK_DIRECTORIES(/home/kevin/code/jsoncpp-pre-C-11/build/debug/lib)
find_package (Threads)

set(SOURCE_FILES server/src/main.cpp server/src/latency.cpp server/src/twoPlayerPong.cpp server/src/base64.cpp server/src/sha1.cpp server/src/websocket.cpp server/src/player.cpp)
add_executable(PongGame ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(PongGame jsoncpp)
target_link_libraries (PongGame ${CMAKE_THREAD_LIBS_INIT})