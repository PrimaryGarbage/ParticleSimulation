cmake_minimum_required(VERSION 3.22)
project(ParticleSimulation VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}bin)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(BUILD_SHARED_LIBS OFF)
set(SFML_USE_STATIC_STD_LIBS)

set(PRIMARY_SRC
	main.cpp
	app.cpp app.h
	debug.cpp debug.h
	particle.h
	physics_solver.cpp physics_solver.h
	tools.h
	)

list(TRANSFORM PRIMARY_SRC PREPEND "src/")

add_executable(${PROJECT_NAME} ${PRIMARY_SRC})

target_compile_options(${PROJECT_NAME} PRIVATE -static)

add_subdirectory(external/sfml)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

target_include_directories(${PROJECT_NAME} PRIVATE external/include external/sfml/include src)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/external/sfml)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-system)
