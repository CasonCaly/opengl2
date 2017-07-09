#include "ObjSurface.h"
#include "os/Path.h"
#include <fstream>

ObjSurface::ObjSurface(const std::string& name)
{
	m_name = name;
	m_faces.resize(this->getTriangleIndexCount() / 3);
	std::string fullPath = Path::joinResource(name);

	ifstream objFile(fullPath.c_str());
	vector<ivec3>::iterator face = m_faces.begin();
	while (objFile) {
		char c = objFile.get();
		if (c == 'f') {
			//assert(face != m_faces.end() && "parse error");
			objFile >> face->x >> face->y >> face->z;
			*face++ -= ivec3(1, 1, 1);
		}
		objFile.ignore(MaxLineSize, '\n');
	}
}

int ObjSurface::getVertexCount()
{
	if (m_vertexCount != 0)
		return m_vertexCount;

	ifstream objFile(m_name.c_str());
	while (objFile) {
		char c = objFile.get();
		if (c == 'v')
			m_vertexCount++;
		objFile.ignore(MaxLineSize, '\n');
	}
	return m_vertexCount;
}

int ObjSurface::getLineIndexCount()
{
	return 0;
}

int ObjSurface::getTriangleIndexCount()
{
	if (m_faceCount != 0)
		return m_faceCount * 3;
	std::string fullPath = Path::joinResource(m_name);
	ifstream objFile(fullPath.c_str());
	while (objFile) {
		char c = objFile.get();
		if (c == 'f')
			m_faceCount++;
		objFile.ignore(MaxLineSize, '\n');
	}
	return m_faceCount * 3;
}

void ObjSurface::generateVertices()
{
	struct Vertex {
		vec3 Position;
		vec3 Normal;
	};
	vector<float> vertices;
	// Read in the vertex positions and initialize lighting normals to (0, 0, 0).
	std::string fullPath = Path::joinResource(m_name);
	vertices.resize(this->getVertexCount() * 6);
	ifstream objFile(fullPath.c_str());
	Vertex* vertex = (Vertex*)&vertices[0];
	while (objFile) {
		char c = objFile.get();
		if (c == 'v') {
			vertex->Normal = vec3(0, 0, 0);
			vec3& position = (vertex++)->Position;
			objFile >> position.x >> position.y >> position.z;
		}
		objFile.ignore(MaxLineSize, '\n');
	}

	vertex = (Vertex*)&vertices[0];
	for (size_t faceIndex = 0; faceIndex < m_faces.size(); ++faceIndex) {
		ivec3 face = m_faces[faceIndex];

		// Compute the facet normal.
		vec3 a = vertex[face.x].Position;
		vec3 b = vertex[face.y].Position;
		vec3 c = vertex[face.z].Position;
		vec3 facetNormal = (b - a).Cross(c - a);

		// Add the facet normal to the lighting normal of each adjoining vertex.
		vertex[face.x].Normal += facetNormal;
		vertex[face.y].Normal += facetNormal;
		vertex[face.z].Normal += facetNormal;
	}

	// Normalize the normals.
	for (int v = 0; v < this->getVertexCount(); ++v)
		vertex[v].Normal.Normalize();

	m_vertexBuffer.gen();
	m_vertexBuffer.bind(GL_ARRAY_BUFFER);
	m_vertexBuffer.setData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(vertices[0]),
		&vertices[0],
		GL_STATIC_DRAW);
}

void ObjSurface::generateLineIndices()
{

}

void ObjSurface::generateTriangleIndices()
{
	int indexCount = this->getTriangleIndexCount();
	vector<GLushort> indices;
	indices.resize(indexCount);
	vector<unsigned short>::iterator index = indices.begin();
	for (vector<ivec3>::const_iterator f = m_faces.begin(); f != m_faces.end(); ++f) {
		*index++ = f->x;
		*index++ = f->y;
		*index++ = f->z;
	}

	m_triangleIndexBuffer.gen();
	m_triangleIndexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	m_triangleIndexBuffer.setData(
		GL_ELEMENT_ARRAY_BUFFER,
		indexCount * sizeof(GLushort),
		&indices[0],
		GL_STATIC_DRAW);
}