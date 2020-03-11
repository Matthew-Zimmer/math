target_include_directories(math INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}> 
	$<INSTALL_INTERFACE:include/math>)

target_include_directories(math_test PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

set(headers 
	matrix.hpp
	vector.hpp)

install(FILES ${headers} DESTINATION include/math)
