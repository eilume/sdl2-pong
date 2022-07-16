cmake_minimum_required(VERSION 3.2)
project("pong")
set(CMAKE_CXX_STANDARD 17)
if(APPLE)
    enable_language(OBJC)
endif()

# Options for setting build type
option(BUILD_DEBUG "Debug Build" ON)
option(BUILD_RELEASE "Release Build" OFF)
option(BUILD_DIST "Distribution Build" OFF)

# Setup files
file(GLOB_RECURSE SOURCE_FILES RELATIVE ${PROJECT_SOURCE_DIR}
	"src/*.cpp"
)

set(HEADER_FILE_DIRS
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/vendor/SDL/include"
	"${PROJECT_SOURCE_DIR}/vendor/SDL_image/include"
)

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}
	"src/*.h"
	"vendor/SDL/include/*.h"
	"vendor/SDL_image/include/*.h"
)

# Create binary
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Apply options based on build type
if (BUILD_DIST)
	set(CMAKE_BUILD_TYPE Release)

	set(BUILD_DEBUG OFF)
	set(BUILD_RELEASE OFF)
	
	set(BUILD_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/dist)
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
	set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS} -Ofast")

	target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_DIST=1)
elseif (BUILD_RELEASE)
	set(CMAKE_BUILD_TYPE Release)
	
	set(BUILD_DEBUG OFF)
	
	set(BUILD_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/release)
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

	target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_RELEASE=1)
else ()
	set(CMAKE_BUILD_TYPE Debug)

    set(BUILD_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/debug)
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Og -g")

	target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_DEBUG=1)
endif()

make_directory(${BUILD_OUTPUT_PATH})

# Link vendor files
add_subdirectory(vendor/SDL)
target_link_libraries(${PROJECT_NAME} SDL2)

add_subdirectory(vendor/SDL_image)
target_link_libraries(${PROJECT_NAME} SDL2_image)

target_include_directories(${PROJECT_NAME} PUBLIC ${HEADER_FILE_DIRS})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT_PATH})

if (BUILD_DIST)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets/")
	
	# Clear folder to remove unused assets
	file(REMOVE_RECURSE ${BUILD_OUTPUT_PATH}/assets)

    file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${BUILD_OUTPUT_PATH})
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${PROJECT_SOURCE_DIR}/assets/")
endif()
