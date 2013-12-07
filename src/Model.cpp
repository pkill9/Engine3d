#include "Model.h"

#include <string.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"



Model::Model(const char* name, GLuint IBO, GLuint VBO, int numVertices)
{
	int length = strlen(name);
	mModelName = new char[length];

	for(int i = 0; i < length; i++)
		mModelName[i] = name[i];
	

	mIBO = IBO;
	mVBO = VBO;

	mNumVertices = numVertices;
}

Model::~Model()
{
	
	glDeleteBuffersARB(1, &mVBO);
	delete[] mModelName;
}


void Model::Draw(glm::mat4* interpolator)
{
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(*interpolator));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mVBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glEnableVertexAttribArray(0);
    	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, SERERATION, (void*)NORMAL_PTR);
	glVertexAttribPointer(0, VERTS_PER_FACE, GL_FLOAT, GL_FALSE, SERERATION, (void *) 0);
	glDrawElements(GL_TRIANGLES, mNumVertices, GL_UNSIGNED_SHORT, (void*)0);
	glDisableClientState(GL_NORMAL_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glPopMatrix();
}

// static member variables
Assimp::Importer	ModelManager::mImporter;
std::vector<Model*>	ModelManager::mModels;

ModelManager::ModelManager()
{
}

// what still needs to be done:
//			right now each model gets its own vertex buffer object,
//			it would be more efficent to have all IBOs index
//			a single VBO.
Model* ModelManager::Load(const char* name, const char* filename)
{
	for(int i = 0; i < mModels.size(); i++)
	{

		if(strcmp(mModels[i]->getName(), name) == 0)
		{
			return mModels[i];
		}
	
	}


	const aiScene *scene = mImporter.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll needai
	aiMesh *mesh = scene->mMeshes[0]; 
 	Vertex* vertices = new Vertex[mesh->mNumFaces*3*3];
	std::cout << mesh->mNumVertices  << ' ' << mesh->mNumFaces << std::endl;
	int index = 0;
	
	GLshort* indices = new GLshort[mesh->mNumFaces*3];
	
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];			
		for(int j = 0; j < 3; j++)
			indices[index++] = face.mIndices[j];
	}

	float* array = new float[mesh->mNumVertices*3*2];
	for(int i = 0; i < mesh->mNumVertices; i++)
	{
		array[6*i+0] = mesh->mVertices[i].x;
		array[6*i+1] = mesh->mVertices[i].y;
		array[6*i+2] = mesh->mVertices[i].z;
		array[6*i+3] = mesh->mNormals[i].x;
		array[6*i+4] = mesh->mNormals[i].y;
		array[6*i+5] = mesh->mNormals[i].z;
	}

	GLuint VBO = -1;
	GLuint IBO = -1;	
	

	glGenBuffersARB(1, &VBO);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, mesh->mNumVertices*3*2*4, array, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &IBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, index, indices, GL_STATIC_DRAW_ARB);

	Model* mod = new Model(name, IBO, VBO, index);
	
	mModels.push_back(mod);

	return mod;
}











