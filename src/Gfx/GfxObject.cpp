#include "GfxObject.hpp"
#include "Game.hpp"

int GfxObject::mAutoNameIndex = 0;

GfxObject::GfxObject(const MeshData& data, String material)
{
	mSmgr = Game::getPtr()->getGfx()->getSceneManager();
	nombre = "SceneNodeAutoNamed"+StringUtils::toString(mAutoNameIndex);
	++mAutoNameIndex;
	Game::getPtr()->getGfx()->addGfxObject(this);
	mNode = mSmgr->getRootSceneNode()->createChildSceneNode(nombre);
	mEntity = 0;
	load(data);
	mat = material;
}

GfxObject::GfxObject(String material, Vector3 pos)
{
	mSmgr = Game::getPtr()->getGfx()->getSceneManager();
	nombre = "SceneNodeAutoNamed"+StringUtils::toString(mAutoNameIndex);
	++mAutoNameIndex;
	Game::getPtr()->getGfx()->addGfxObject(this);
	mNode = mSmgr->getRootSceneNode()->createChildSceneNode(nombre);
	mNode->setPosition(pos);
	mEntity = 0;
	mat = material;
}

GfxObject::GfxObject(String mesh)
{
	mSmgr = Game::getPtr()->getGfx()->getSceneManager();
	mEntity = mSmgr->createEntity(mesh);
	mNode = mSmgr->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(mEntity);
	Game::getPtr()->getGfx()->addGfxObject(this);
}

GfxObject::GfxObject(String mesh, String name)
{
	mSmgr = Game::getPtr()->getGfx()->getSceneManager();
	mEntity = mSmgr->createEntity(name, mesh);
	mNode = mSmgr->getRootSceneNode()->createChildSceneNode(name);
	mNode->attachObject(mEntity);
	Game::getPtr()->getGfx()->addGfxObject(this);
}

void GfxObject::kill()
{
	Game::getPtr()->getGfx()->removeGfxObject(this);
}

GfxObject::~GfxObject()
{
	mNode->detachAllObjects();
	mSmgr->destroySceneNode(mNode);
	if(mEntity)
		mSmgr->destroyEntity(mEntity);
}

void GfxObject::load(const MeshData& data)
{
	using namespace Ogre;

	bool hasVertexColor = data.getDiffuse();
	bool hasNormals = data.getNormals();

	int numFaces = data.indices.size()/3;
	int numVertices = data.vertices.size()/3;

	HardwareVertexBufferSharedPtr posVertexBuffer;
	HardwareVertexBufferSharedPtr normVertexBuffer;
	std::vector<HardwareVertexBufferSharedPtr> texcoordsVertexBuffer;
	HardwareVertexBufferSharedPtr diffuseVertexBuffer;
	HardwareIndexBufferSharedPtr indexBuffer;

	Ogre::Mesh* m = Ogre::MeshManager::getSingletonPtr()->createManual(
	nombre,ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).get();

	Ogre::SubMesh* sm = m->createSubMesh();
	sm->useSharedVertices = false;
	sm->vertexData = new VertexData();
	sm->vertexData->vertexStart = 0;
	sm->vertexData->vertexCount = numVertices;

	Ogre::VertexDeclaration* vdecl = sm->vertexData->vertexDeclaration;
	Ogre::VertexBufferBinding* vbind = sm->vertexData->vertexBufferBinding;

	size_t bufferCount = 0;

	vdecl->addElement(bufferCount, 0, VET_FLOAT3, VES_POSITION);

	if(hasNormals)
		vdecl->addElement(++bufferCount, 0, VET_FLOAT3, VES_NORMAL);

	if(hasVertexColor)
		vdecl->addElement(++bufferCount, 0, VET_FLOAT4, VES_DIFFUSE);

	for(int i=0;i<data.texcoords.size();++i)
		vdecl->addElement(++bufferCount, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES,i);

	bufferCount = 0;

	// Positions
	posVertexBuffer = HardwareBufferManager::getSingleton().createVertexBuffer(
		3*sizeof(float),numVertices,Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

	vbind->setBinding(bufferCount, posVertexBuffer);

	float* vertices = data.getVertices();
	float* normals = data.getNormals();
	float* diffuse = data.getDiffuse();
	unsigned short* indices = data.getIndices();

	posVertexBuffer->writeData(0,posVertexBuffer->getSizeInBytes(),vertices, true);

	// Normals
	if(hasNormals)
	{
		normVertexBuffer = HardwareBufferManager::getSingleton().createVertexBuffer(
		3*sizeof(float),numVertices,HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

		vbind->setBinding(++bufferCount, normVertexBuffer);

		normVertexBuffer->writeData(0,normVertexBuffer->getSizeInBytes(),normals, true);
	}

	if(hasVertexColor)
	{
		diffuseVertexBuffer = HardwareBufferManager::getSingleton().createVertexBuffer(
		4*sizeof(float),numVertices,HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		vbind->setBinding(++bufferCount, diffuseVertexBuffer);

		diffuseVertexBuffer->writeData(0,diffuseVertexBuffer->getSizeInBytes(), diffuse, true);
	}

	// Texcoords
	for(int i=0;i<data.texcoords.size();++i)
	{
		texcoordsVertexBuffer.push_back(HardwareBufferManager::getSingleton().createVertexBuffer(
		2*sizeof(float),numVertices,HardwareBuffer::HBU_STATIC_WRITE_ONLY));

		vbind->setBinding(++bufferCount, texcoordsVertexBuffer[i]);

		texcoordsVertexBuffer[i]->writeData(0,sizeof(float)*data.texcoords[i].size(),&data.texcoords[i][0], false);
	}

	if(!data.indices.empty())
	{
		// Prepare buffer for indices
		indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
		HardwareIndexBuffer::IT_16BIT,3*numFaces,HardwareBuffer::HBU_STATIC_WRITE_ONLY, true);

		//unsigned short *faceVertexIndices = (unsigned short*)
		//indexBuffer->lock(0, numFaces*3*2, HardwareBuffer::HBL_DISCARD);

		// Set index buffer for this submesh
		sm->indexData->indexBuffer = indexBuffer;
		sm->indexData->indexStart = 0;
		sm->indexData->indexCount = 3*numFaces;

		indexBuffer->writeData(0,indexBuffer->getSizeInBytes(),indices,true);
	}

	//vdecl->sort();

	m->load();
	m->touch();

	//int dims = 16;
	//m->_setBounds(AxisAlignedBox(-dims/2,-64/2,-dims/2,dims/2,64/2,dims/2));
	m->_setBounds(AxisAlignedBox(0.f,0.f,0.f,7.f,7.f,32.f));
	m->_setBoundingSphereRadius(35.f);//11.313f);

	sm->setMaterialName(mat);

	mEntity = mSmgr->createEntity(nombre,m->getName());
	mNode->attachObject(mEntity);
	mEntity->setCastShadows(false);
}

void GfxObject::unload()
{
	Ogre::MeshPtr m = mEntity->getMesh();
	if(m->isManuallyLoaded())
	{
		Ogre::MeshManager::getSingleton().unload(m->getName());
		Ogre::MeshManager::getSingleton().remove(m->getName());
	}
	mSmgr->destroyEntity(mEntity);
}
