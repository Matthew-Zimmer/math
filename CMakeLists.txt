cmake_minimum_required(VERSION 3.13)

project(math)

add_library(math)
add_library(math_test)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

target_compile_features(math PUBLIC cxx_std_17)
target_compile_features(math_test PUBLIC cxx_std_17)

#Set(CMAKE_CXX_FLAGS "-Wall" "-Wextra")

install(TARGETS math EXPORT math DESTINATION lib/math)
install(EXPORT math DESTINATION lib/)
install(FILES math-config.cmake DESTINATION lib/math)

#libs
#find_Package(LIB REQUIRED)
#target_link_libraries(math PUBLIC LIB)