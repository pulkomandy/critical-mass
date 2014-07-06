include_directories(../Headers)

add_executable(CriticalMass
	CMassApplication.cpp CMassWindow.cpp CMassView.cpp CMassMenuBar.cpp
	CriticalMass.cpp CMBoard.cpp CMassBlitView.cpp Ball.c BallMath.c BallAux.c
	CMBoardQueue.cpp CMGameTurn.cpp CMGameTurnStack.cpp CMSound.cpp CMBrain.cpp
	CMSemaphore.cpp CMRowBrain.cpp
)

get_target_property(CMASS_PATH CriticalMass LOCATION)

add_custom_command(TARGET CriticalMass POST_BUILD 
	COMMAND rc -o Resource.rsrc ${CMAKE_SOURCE_DIR}/Resources.rdef
	COMMAND xres -o ${CMASS_PATH} Resource.rsrc
	DEPENDS Resource.rdef)

target_link_libraries(CriticalMass be GL media GLU )
