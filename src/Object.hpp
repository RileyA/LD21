#ifndef Object_H
#define Object_H

#include "StdHeaders.hpp"

class Object
{
public:

	Object(String bucket = "Default");
	virtual ~Object();

	virtual void update(Real delta) = 0;

private:


};

#endif
