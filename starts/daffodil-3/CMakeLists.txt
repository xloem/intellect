project(Daffodil3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp node.cpp reference.cpp touch.cpp globals.cpp)
