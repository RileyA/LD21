set(SOURCE_FILES
	Bucket.cpp
	Game.cpp
	main.cpp
	State.cpp
	)

set(HEADER_FILES
	Bucket.hpp
	Game.hpp
	Object.hpp
	State.hpp
	StdHeaders.hpp
	)

add_executable(LD21 ${SOURCE_FILES} ${HEADER_FILES} )
set_target_properties(LD21 PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries (demo ${ORYX_LIB} ${BOOST_LIB} ${SUBSYSTEMS})
