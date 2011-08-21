#ifndef GUI_H
#define GUI_H

#include "StdHeaders.hpp"
#include "Ogre.h"
#include "Oyster.h"

class Gui : public Ogre::RenderQueueListener
{
public:

	Gui(Oyster::Batch* b, String material)
		:mVisible(true)
	{
		mMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingletonPtr()->
			getByName(material)).getPointer();
		Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->addRenderQueueListener(this);
		mRenderOp.vertexData = 0;
		mRenderOp.indexData = 0;
		mBatch = b;
		t = 0;
		const Oyster::Mesh& m = mBatch->getMesh();
		buildMesh(m.getVertexCount(), m.getFaceCount(), m.getVertices(), m.getTexcoords(), m.getColors(), m.getIndices());
	}

	virtual ~Gui();

	void update();

	/** Builds the whole thing */
	void buildMesh(size_t vertexCount, size_t faceCount, float* pos, float* tex, float* colors, unsigned short* indices);

	/** rebuilds the whole thing */
	void rebuildMesh(size_t vertexCount, size_t faceCount, float* pos, float* tex, float* colors, unsigned short* indices);

	/** Updates vertex positions */
	void updatePositions(float* pos);

	/** Updates texcoords */
	void updateTexcoords(float* tex);

	/** Updates vertex colors */
	void updateColors(float* colors);

	/** Empty implementations... */
	void preRenderQueues(){}
	void postRenderQueues(){}
	void renderQueueStarted(Ogre::uint8 id, const Ogre::String& invocation, bool& repeat){}
	
	/** Manually render it here. */
	void renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeat);

	//void setVisible(bool v){mVisible = v;}
	//bool isVisible(){return mVisible;}

private:

	Oyster::Batch* mBatch;

	Ogre::Material* mMaterial;
	Ogre::RenderOperation mRenderOp;
	size_t mVertexCount;

	bool mVisible;
	int t;

};

#endif
