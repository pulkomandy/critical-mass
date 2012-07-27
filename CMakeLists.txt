cmake_minimum_required(VERSION 2.8)
project(CriticalMass)
set(CMAKE_CXX_FLAGS "-Wall -Wno-multichar -Os")
add_subdirectory(Sources)

get_target_property(CMASS_PATH CriticalMass LOCATION)
add_custom_command(OUTPUT CriticalMass.zip
	COMMAND zip ${CMAKE_BINARY_DIR}/CriticalMass.zip -j ${CMASS_PATH} ${CMAKE_SOURCE_DIR}/Documentation/index.html
	COMMAND zip ${CMAKE_BINARY_DIR}/CriticalMass.zip pictures/*.png
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Documentation
	DEPENDS CriticalMass Documentation/**
)

add_custom_target(Release DEPENDS CriticalMass.zip)
