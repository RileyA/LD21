#include "Bucket.hpp"

Bucket::Bucket(String name)
	:mName(name)
{

}

Bucket::~Bucket()
{
	std::list<Object*>::iterator it = mObjects.begin();
	for(it; it != mObjects.end(); ++it)
	{
		delete (*it);
	}
	mObjects.clear();
}

void Bucket::update(Real delta)
{
	std::list<Object*>::iterator it = mObjects.begin();
	for(it; it != mObjects.end(); ++it)
	{
		(*it)->update(delta);
	}
}

void Bucket::pushObject(Object* obj)
{
	mObjects.push_back(obj);
}

void Bucket::removeObject(Object* obj)
{
	std::list<Object*>::iterator it = mObjects.begin();
	for(it; it != mObjects.end(); ++it)
	{
		if((*it) == obj)
		{
			delete (*it);
			mObjects.erase(it);
		}
	}
}

Object* Bucket::popObject(Object* obj)
{
	std::list<Object*>::iterator it = mObjects.begin();
	for(it; it != mObjects.end(); ++it)
	{
		if((*it) == obj)
		{
			Object* temp = *it;
			mObjects.erase(it);
			return temp;
		}
	}
}
