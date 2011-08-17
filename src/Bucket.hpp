#ifndef Bucket_H
#define Bucket_H

#include "StdHeaders.hpp"
#include "Object.hpp"

class Bucket
{
public:

	Bucket(String name);
	virtual ~Bucket();

	/** Update all the contained objects */
	void update(Real delta);

	/** Add the object */
	void pushObject(Object* obj);

	/** Remove and return */
	Object* popObject(Object* obj);

	/** Remove and delete */
	void removeObject(Object* obj);

private:

	std::list<Object*> mObjects;
	String mName;

};

#endif
