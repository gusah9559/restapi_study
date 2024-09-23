cmake_minimum_required(VERSION 3.20)

project(CppRestAPIServer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(cpprestsdk REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(restapi main.cpp)

target_link_libraries(restapi PRIVATE cpprestsdk::cpprest OpenSSL::SSL OpenSSL::Crypto)