
#pragma once

#define GL_GLEXT_PROTOTYPES

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glx.h"

#include "glm/glm.hpp"

#include "assimp/Importer.hpp"

#include <vector>

// used for indexing IBO and VBO
#define NORMAL_PTR 12
#define SERERATION 24

// triangles
#define VERTS_PER_FACE 3


class Vertex
{
public:
	float x, y, z;
	float nx,ny,nz;
	float u,v;
};

class Model
{
public:
	Model(const char* name, GLuint IBO, GLuint mVBO, int numVertices);
	~Model();
	void Draw(glm::mat4* interpolator);
	
	const char*	getName()		{ return mModelName; 	}
	const int	getNumVertices()	{ return mNumVertices;	}	
			
protected:
	char*  mModelName;
	GLuint mIBO;
	GLuint mVBO;
	int mNumVertices;
};


class ModelManager
{
public:

	ModelManager();
	
	static Model* Load(const char* modelName, const char* fileName);
	
protected:
	
	static Assimp::Importer mImporter;
	static std::vector<Model*> mModels;
};


