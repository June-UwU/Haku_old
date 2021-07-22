cmake_minimum_required(VERSION 3.20.2)

set(PROJECT_NAME Haku)
set(MAIN "EntryPoint.cpp")


file(GLOB_RECURSE "INCLUDES" 
	 Includes/*h 
	 Includes/*cpp)


add_executable(${PROJECT_NAME} 
	 ${MAIN}
	 ${INCLUDES}
	 )

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/spdlog/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/wil/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Includes")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Shaders")


target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Haku.ico
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})