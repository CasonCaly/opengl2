#pragma once
#include "glsurface/GLSurface.h"

class ObjSurface : public GLSurface
{
public:

	ObjSurface(const std::string& name);

public:

	virtual int getVertexCount();

	virtual int getLineIndexCount();

	virtual int getTriangleIndexCount();

	virtual void generateVertices();

	virtual void generateLineIndices();

	virtual void generateTriangleIndices();

private:
	string m_name;
	vector<ivec3> m_faces;
    size_t m_faceCount;
    size_t m_vertexCount;
	static const int MaxLineSize = 128;
};
