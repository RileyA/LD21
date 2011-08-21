#include "Gui.hpp"

void Gui::update()
{
	t = 0;
	Oyster::DirtyFlags f = mBatch->update();	
	const Oyster::Mesh& m = mBatch->getMesh();

	if(f & 8)
		rebuildMesh(m.getVertexCount(), m.getFaceCount(), m.getVertices(), m.getTexcoords(), m.getColors(), m.getIndices());
	else
	{
		if(f & 1)
			updatePositions(m.getVertices());
		if(f & 2)
			updateTexcoords(m.getTexcoords());
		if(f & 4)
			updateColors(m.getColors());
	}
}

void Gui::buildMesh(size_t vertexCount, size_t faceCount, float* pos, float* tex, float* colors, unsigned short* indices)
{
	using namespace Ogre;
	
	mVisible = true;

	mRenderOp.vertexData = new Ogre::VertexData;
	mRenderOp.vertexData->vertexStart = 0;
	Ogre::VertexDeclaration* vdecl = mRenderOp.vertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* vbind = mRenderOp.vertexData->vertexBufferBinding;
	size_t offset = 0;

	HardwareVertexBufferSharedPtr posVB;
	HardwareVertexBufferSharedPtr texcoordVB;
	HardwareVertexBufferSharedPtr colorVB;
	HardwareVertexBufferSharedPtr indexVB;

	mRenderOp.vertexData->vertexCount = vertexCount;
	
	size_t bufferCount = 0;
	vdecl->addElement(bufferCount, 0, VET_FLOAT3, VES_POSITION);
	vdecl->addElement(++bufferCount, 0, VET_FLOAT4, VES_DIFFUSE);
	vdecl->addElement(++bufferCount, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES);
	bufferCount = 0;

	// Positions
	posVB = HardwareBufferManager::getSingleton().createVertexBuffer(
		3*sizeof(float), vertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	vbind->setBinding(bufferCount, posVB);
	posVB->writeData(0,posVB->getSizeInBytes(), pos, true);

	// Vertex colors
	colorVB = HardwareBufferManager::getSingleton().createVertexBuffer(
		4*sizeof(float), vertexCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	vbind->setBinding(++bufferCount, colorVB);
	colorVB->writeData(0,colorVB->getSizeInBytes(), colors, true);

	// Texcoords
	texcoordVB = HardwareBufferManager::getSingleton().createVertexBuffer(
		2*sizeof(float), vertexCount,HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vbind->setBinding(++bufferCount, texcoordVB);
	texcoordVB->writeData(0,sizeof(float) * 2 * vertexCount, tex, true);

	// Indices
	Ogre::HardwareIndexBufferSharedPtr indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
		HardwareIndexBuffer::IT_16BIT,3*faceCount,HardwareBuffer::HBU_STATIC, true);
	indexBuffer->writeData(0,indexBuffer->getSizeInBytes(),indices,true);
	
	mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
	mRenderOp.useIndexes = true;
	mRenderOp.indexData = new Ogre::IndexData;

	// Set index buffer for the render op
	mRenderOp.indexData->indexBuffer = indexBuffer;
	mRenderOp.indexData->indexStart = 0;
	mRenderOp.indexData->indexCount = 3 * faceCount;
}
//-----------------------------------------------------------------------

Gui::~Gui()
{
	if(mVisible)
	{
		// delete render operation data
		delete mRenderOp.vertexData;
		delete mRenderOp.indexData;
	}

	Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->removeRenderQueueListener(this);
}
//-----------------------------------------------------------------------

void Gui::rebuildMesh(size_t vertexCount, size_t faceCount, float* pos, float* tex, float* colors, unsigned short* indices)
{
	// TODO do an actual update rather than a full delete and rebuild!
	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;
	mRenderOp.vertexData = 0;
	mRenderOp.indexData = 0;
	buildMesh(vertexCount, faceCount, pos, tex, colors, indices);
}
//-----------------------------------------------------------------------

void Gui::updatePositions(float* pos)
{
	Ogre::VertexDeclaration* vdecl = mRenderOp.vertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* vbind = mRenderOp.vertexData->vertexBufferBinding;
	const Ogre::VertexElement* buf = vdecl->findElementBySemantic(Ogre::VES_POSITION);
	Ogre::HardwareVertexBufferSharedPtr buffer = vbind->getBuffer(buf->getSource());
	if(buf && !buffer.isNull())
	{
		buffer->writeData(0,buffer->getSizeInBytes(), pos, true);
	}
	else
	{
	}
}
//-----------------------------------------------------------------------

void Gui::updateTexcoords(float* tex)
{
	Ogre::VertexDeclaration* vdecl = mRenderOp.vertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* vbind = mRenderOp.vertexData->vertexBufferBinding;
	const Ogre::VertexElement* pos = vdecl->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
	Ogre::HardwareVertexBufferSharedPtr buffer = vbind->getBuffer(pos->getSource());
	if(pos && !buffer.isNull())
	{
		buffer->writeData(0,buffer->getSizeInBytes(),tex, true);
	}
	else
	{
	}
}
//-----------------------------------------------------------------------

void Gui::updateColors(float* colors)
{
	Ogre::VertexDeclaration* vdecl = mRenderOp.vertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* vbind = mRenderOp.vertexData->vertexBufferBinding;
	const Ogre::VertexElement* pos = vdecl->findElementBySemantic(Ogre::VES_DIFFUSE);
	Ogre::HardwareVertexBufferSharedPtr buffer = vbind->getBuffer(pos->getSource());
	if(pos && !buffer.isNull())
	{
		buffer->writeData(0,buffer->getSizeInBytes(),colors, true);
	}
	else
	{
	}
}
//-----------------------------------------------------------------------

void Gui::renderQueueEnded(Ogre::uint8 id, const Ogre::String& invocation, bool& repeat)
{
	if (t || !mVisible || id != Ogre::RENDER_QUEUE_OVERLAY)
		return;
	++t;

	// setup uniform matrices
	Ogre::Root::getSingletonPtr()->getRenderSystem()->_setWorldMatrix( Ogre::Matrix4::IDENTITY );
	Ogre::Root::getSingletonPtr()->getRenderSystem()->_setProjectionMatrix( Ogre::Matrix4::IDENTITY );
	Ogre::Root::getSingletonPtr()->getRenderSystem()->_setViewMatrix( Ogre::Matrix4::IDENTITY );

	// set the material pass
	Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->_setPass(
		mMaterial->getTechnique(0)->getPass(0), false, false);
	
	// actually render the thing
	Ogre::Root::getSingletonPtr()->getRenderSystem()->_render(mRenderOp);
}
//-----------------------------------------------------------------------
