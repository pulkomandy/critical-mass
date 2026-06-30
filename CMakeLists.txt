cmake_minimum_required(VERSION 3.5)
project(CriticalMass)
set(CMAKE_CXX_FLAGS "-Wall -Wno-multichar -Os")
add_subdirectory(Sources)

add_custom_command(OUTPUT CriticalMass.zip
	COMMAND zip ${CMAKE_BINARY_DIR}/CriticalMass.zip -j $<TARGET_FILE:CriticalMass> ${CMAKE_SOURCE_DIR}/Documentation/index.html
	COMMAND zip ${CMAKE_BINARY_DIR}/CriticalMass.zip pictures/*.png
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Documentation
	DEPENDS CriticalMass Documentation/**
)

add_custom_target(Release DEPENDS CriticalMass.zip)
