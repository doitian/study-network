cmake_minimum_required(VERSION 2.6)
project(study-network)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

set(targets
  getaddrinfo)

foreach(target IN LISTS targets)
  add_executable("${target}" "src/${target}.cpp")
endforeach(target)