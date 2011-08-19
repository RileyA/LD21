set(SOURCE_FILES
	Bucket.cpp
	EventHandler.cpp
	Game.cpp
	main.cpp
	MenuState.cpp
	Signal.cpp
	Slot.cpp
	State.cpp
	Input/InputMgr.cpp
	Input/OISListener.cpp
	Audio/ActiveSound.cpp
	Audio/AudioMgr.cpp
	Audio/BufferedSound.cpp
	Audio/OggStream.cpp
	Audio/SoundPtr.cpp
	Gfx/GfxMgr.cpp
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
	Input/InputMgr.cpp
	Input/OISListener.cpp
	Audio/ActiveSound.hpp
	Audio/AudioMgr.hpp
	Audio/BufferedSound.hpp
	Audio/OggStream.hpp
	Audio/SoundPtr.hpp
	Gfx/GfxMgr.hpp
	)

add_executable(LD21 ${SOURCE_FILES} ${HEADER_FILES} )
set_target_properties(LD21 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (LD21 
	${OIS_LIB} 
	${AL_LIB} ${ALUT_LIB} ${OGG_LIB} ${VORBIS_LIB} ${VORBISFILE_LIB} ${VORBISENC_LIB}
	${OGRE_LIB}
	${BOOST_DATE_LIB} ${BOOST_THREAD_LIB}
	${BULLET_C_LIB} ${BULLET_D_LIB} ${BULLET_MATH_LIB}
	)
