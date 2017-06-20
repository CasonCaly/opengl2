//
//  GLSurface.hpp
//  glsurface
//
//  Created by zhuang yusong on 2017/6/11.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#include "GLSurface.h"

GLSurface::~GLSurface()
{
}

int GLSurface::getVertexCount()
{
	return m_divisions.x * m_divisions.y; 
}

int GLSurface::getLineIndexCount() 
{
	return 4 * m_slices.x * m_slices.y;
}

int GLSurface::getTriangleIndexCount() 
{
	return 6 * m_slices.x * m_slices.y;
}

void GLSurface::generateVertices() 
{
	vector<float> vertices;
	int floatsPerVertex = 3;
	vertices.resize(this->getVertexCount() * floatsPerVertex);
	float* attribute = &vertices[0];

	for (int j = 0; j < m_divisions.y; j++)
	{
		for (int i = 0; i < m_divisions.x; i++)
		{
			vec2 domain = this->computeDomain((float)i, (float)j);
			vec3 range = this->evaluate(domain);
			attribute = range.Write(attribute);
		}
	}

	m_vertexBuffer.gen();
	m_vertexBuffer.bind(GL_ARRAY_BUFFER);
	m_vertexBuffer.setData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(vertices[0]),
		&vertices[0],
		GL_STATIC_DRAW);
}

void GLSurface::generateLineIndices() 
{
	int indexCount = this->getLineIndexCount();
	vector<GLushort> indices;
	indices.resize(indexCount);
	vector<unsigned short>::iterator index = indices.begin();
	for (int j = 0, vertex = 0; j < m_slices.y; j++) {
		for (int i = 0; i < m_slices.x; i++) {
			int next = (i + 1) % m_divisions.x;
			*index++ = vertex + i;
			*index++ = vertex + next;
			*index++ = vertex + i;
			*index++ = vertex + i + m_divisions.x;
		}
		vertex += m_divisions.x;
	}

	m_indexBuffer;
	m_indexBuffer.gen();
	m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	m_indexBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLushort),
		&indices[0],
		GL_STATIC_DRAW);
}

void GLSurface::generateTriangleIndices(vector<unsigned short>& indices) 
{
}

vec2 GLSurface::computeDomain(float x, float y)
{
	return vec2(x * m_upperBound.x / m_slices.x, y * m_upperBound.y / m_slices.y);
}

void GLSurface::setInterval(const GLInterval& interval)
{
    m_divisions = interval.divisions;
    m_upperBound = interval.upperBound;
    m_textureCount = interval.textureCount;
    m_slices = m_divisions - ivec2(1, 1);
}

vec3 GLSurface::evaluate(const vec2& domain)
{
	return vec3();
}

bool GLSurface::invertNormal(const vec2& domain)
{
	return false; 
}

GLBuffer& GLSurface::getVertexBuffer()
{
	return m_vertexBuffer;
}

GLBuffer& GLSurface::getIndexBuffer()
{
	return m_indexBuffer;
}

ivec2 GLSurface::getLowerLeft()
{
	return m_lowerLeft;
}

ivec2 GLSurface::getViewportSize()
{
	return m_viewportSize;
}

void GLSurface::setLowerLeft(const ivec2& lowerLeft)
{
	m_lowerLeft = lowerLeft;
}

void GLSurface::setViewportSize(const ivec2& viewportSize)
{
	m_viewportSize = viewportSize;
}

void GLSurface::setColor(const vec3& color)
{
	m_color = color;
}

vec3 GLSurface::getColor()
{
	return m_color;
}
