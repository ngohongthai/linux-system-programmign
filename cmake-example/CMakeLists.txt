cmake_minimum_required(VERSION 3.19.5)

# Khai bao ten project
project(cmake-example)

#Khai bao noi chua thu vien
set(PROJECT_LINK_LIBS libgreetings_enjavi.dylib)
link_directories(lib)

#Khai bao noi chua headers
include_directories(include)

#Them source c bang file
file(GLOB SOURCES "src/*.c")

#File chạy sẽ được tạo như thế nào
#Sử dụng thư viện đã khai báo qua PROJECT_LINK_LIBS
add_executable(cmake-example ${SOURCES})
target_link_libraries(cmake-example ${PROJECT_LINK_LIBS})
