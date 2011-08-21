#ifndef StdHeaders_H
#define StdHeaders_H

// some common includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <list>
#include <assert.h>
#include <cstdlib>

// some random typedefs
typedef std::string String;
typedef float Real;
typedef unsigned char byte;
typedef unsigned int uint;

class StringUtils
{
public:
	template <class T> static String toString(T v)
	{
		std::stringstream ss;
		ss<<v;
		return ss.str();
	}
};

class Rand
{
public:
	static float randFloat(float min_, float max_)
	{
		return rand()/(1.f + RAND_MAX) * (max_ - min_) + min_;
	}
};

#define PLATFORM_WIN32 1
#define PLATFORM_LINUX 2
#define PLATFORM_APPLE 3

// Determine platform
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined (WIN32)
	#define LD_PLATFORM PLATFORM_WIN32
	#include <windows.h>
#elif defined( __APPLE_CC__)
	#define LD_PLATFORM PLATFORM_APPLE
#else
	#define LD_PLATFORM PLATFORM_LINUX
	#include <sys/time.h>
#endif

#endif

