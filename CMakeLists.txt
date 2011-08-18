cmake_minimum_required (VERSION 2.6)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Some lib stuff, probably unneeded, but I don't know cmake too well yet...
if (WIN32)
	set(LIB_PREFIX )
	set(LIB_POSTFIX .lib)
else ()
	set(LIB_PREFIX lib)
	set(LIB_POSTFIX .so)
endif ()

# OIS
set(OIS_INCLUDE_DIR "/usr/include/OIS" CACHE FILEPATH "Where to look for ois includes.")
set(OIS_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for ois libs.")
set(OIS_LIB ${OIS_LIB_DIR}/${LIB_PREFIX}OIS${LIB_POSTFIX})

# Ogre
set(OGRE_INCLUDE_DIR "/usr/include/OGRE" CACHE FILEPATH "Where to look for ogre includes.")
set(OGRE_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for ogre libs.")
set(OGRE_LIB ${OIS_LIB_DIR}/${LIB_PREFIX}OgreMain${LIB_POSTFIX})

# Oyster
set(OYSTER_INCLUDE_DIR "/home/riley/Desktop/Projects/Oyster/include" CACHE FILEPATH "Where to look for oyster includes.")

# Bullet
set(BT_INCLUDE_DIR "/usr/include/bullet" CACHE FILEPATH "Where to look for bullet includes.")
set(BT_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for bullet libs.")
set(BT_C_LIB ${BT_LIB_DIR}/${LIB_PREFIX}BulletCollision${LIB_POSTFIX})
set(BT_D_LIB ${BT_LIB_DIR}/${LIB_PREFIX}BulletDynamics${LIB_POSTFIX})
set(BT_MATH_LIB ${BT_LIB_DIR}/${LIB_PREFIX}LinearMath${LIB_POSTFIX})

# AL
set(AL_INCLUDE_DIR "/usr/include" CACHE FILEPATH "Where to look for AL includes.")
set(AL_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for AL libs.")
set(ALUT_INCLUDE_DIR "/usr/include" CACHE FILEPATH "Where to look for ALUT includes.")
set(ALUT_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for ALUT libs.")
set(OGG_INCLUDE_DIR "/usr/include" CACHE FILEPATH "Where to look for Ogg includes.")
set(OGG_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for Ogg libs.")
set(VORBIS_INCLUDE_DIR "/usr/include" CACHE FILEPATH "Where to look for Vorbis includes.")
set(VORBIS_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for Vorbis libs.")
set(AL_LIB ${AL_LIB_DIR}/${LIB_PREFIX}openal${LIB_POSTFIX})
set(ALUT_LIB ${ALUT_LIB_DIR}/${LIB_PREFIX}alut${LIB_POSTFIX})
if (UNIX)
	set(OGG_LIB ${OGG_LIB_DIR}/${LIB_PREFIX}ogg${LIB_POSTFIX})
	set(VORBIS_LIB ${VORBIS_LIB_DIR}/${LIB_PREFIX}vorbis${LIB_POSTFIX})
	set(VORBISENC_LIB ${VORBIS_LIB_DIR}/${LIB_PREFIX}vorbisenc${LIB_POSTFIX})
	set(VORBISFILE_LIB ${VORBIS_LIB_DIR}/${LIB_PREFIX}vorbisfile${LIB_POSTFIX})
else (UNIX)
	set(OGG_LIB ${OGG_LIB_DIR}/libogg_static${LIB_POSTFIX})
	set(VORBIS_LIB ${VORBIS_LIB_DIR}/libvorbis_static${LIB_POSTFIX})
	set(VORBISENC_LIB )
	set(VORBISFILE_LIB ${VORBIS_LIB_DIR}/libvorbisfile_static${LIB_POSTFIX})
endif (UNIX)

# Bullet
set(BT_INCLUDE_DIR "/usr/include/bullet" CACHE FILEPATH "Where to look for bullet includes.")
set(BT_LIB_DIR "/usr/lib" CACHE FILEPATH "Where to look for bullet libs.")
set(BOOST_THREAD_LIB ${BOOST_LIB_DIR}/${LIB_PREFIX}boost_thread${LIB_POSTFIX})

# Some options:
set(INSTALL_RUNTIMES_TO "/usr/bin" CACHE STRING "Where install executables.")
set(INSTALL_LIBS_TO "/usr/lib" CACHE STRING "Where to install libs.")

# includes
include_directories("src")

include_directories(${OGRE_INCLUDE_DIR})

include_directories(${OIS_INCLUDE_DIR})

include_directories(${BULLET_INCLUDE_DIR})

include_directories(${OYSTER_INCLUDE_DIR})

include_directories(${AL_INCLUDE_DIR})
include_directories(${OGG_INCLUDE_DIR})
include_directories(${VORBIS_INCLUDE_DIR})
include_directories(${ALUT_INCLUDE_DIR})

# add source
add_subdirectory(src)
