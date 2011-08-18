set(SOURCE_FILES
	Bucket.cpp
	EventHandler.cpp
	Game.cpp
	main.cpp
	MenuState.cpp
	Signal.cpp
	Slot.cpp
	State.cpp
	)

set(HEADER_FILES
	Bucket.hpp
	EventHandler.hpp
	FastDelegate.h
	Game.hpp
	MenuState.hpp
	MessageAny.hpp
	Message.hpp
	Object.hpp
	Signal.hpp
	Slot.hpp
	State.hpp
	StdHeaders.hpp
	)

add_executable(LD21 ${SOURCE_FILES} ${HEADER_FILES} )
set_target_properties(LD21 PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries (demo ${ORYX_LIB} ${BOOST_LIB} ${SUBSYSTEMS})
