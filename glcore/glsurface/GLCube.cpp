#include "GLCube.h"

GLCube::GLCube(float height){
	m_height = height;
}

int GLCube::getVertexCount(){
	return 4 * 6;//每个面4个顶点，总共6个面
}

int GLCube::getLineIndexCount(){
	return 8;
}

int GLCube::getTriangleIndexCount(){


}

void GLCube::generateVertices(){
	vector<float> vertices;
	int floatsPerVertex = 3;

	if (m_enableVertexNormal)
		floatsPerVertex += 3;

	if (m_enableTexture)
		floatsPerVertex += 2;

	int vertexCount = this->getVertexCount();
	GLfloat vertex[] = {
		0.5f
	};
	//vertices.resize(vertexCount * floatsPerVertex);
	//for (int i = 0; i < vertexCount; i++){
	//	ve
	//}

	m_vertexBuffer.gen();
	m_vertexBuffer.bind(GL_ARRAY_BUFFER);
	//m_vertexBuffer.setData(GL_ARRAY_BUFFER,
	//	)
}

void GLCube::generateLineIndices()
{

}

void GLCube::generateTriangleIndices(){

}