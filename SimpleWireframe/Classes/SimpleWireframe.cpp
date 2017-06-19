#include <cmath>
//#include "Interfaces.hpp"
#include "SimpleWireframe.h"

SimpleWireframe::SimpleWireframe()
{
}

void SimpleWireframe::init()
{
	m_screenSize.x = 320;
	m_screenSize.y = 480;

	m_buttonSize.y = 480 / 10;
	m_buttonSize.x = 4 * m_buttonSize.y / 3;

	m_sphere = new GLSphere(2, ivec2(20, 20));
	m_cone = new GLCone(3, 1, ivec2(4, 4));

	m_surfaces.push_back(m_cone);
	m_surfaces.push_back(m_sphere);
    
	for (auto begin = m_surfaces.begin(); begin != m_surfaces.end(); begin++)
	{
		GLSurface* surface = *begin;
		vector<float> vertices;
		surface->generateVertices(vertices);

		GLBuffer vertexBuffer;
		vertexBuffer.gen();
		vertexBuffer.bind(GL_ARRAY_BUFFER);
		vertexBuffer.setData(GL_ARRAY_BUFFER,
					 vertices.size() * sizeof(vertices[0]),
					 &vertices[0],
					 GL_STATIC_DRAW);
    
		// Create a new VBO for the indices if needed.
		int indexCount = surface->getLineIndexCount();
		vector<GLushort> indices(indexCount);
		surface->generateLineIndices(indices);
    
		GLBuffer indexBuffer;
		indexBuffer.gen();
		indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		indexBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLushort),
							&indices[0],
							GL_STATIC_DRAW);

		Drawable drawble = { vertexBuffer, indexBuffer, indexCount };
		m_drawables.push_back(drawble);

		if (begin == m_surfaces.begin())
		{
			Visual visual = { vec3(0.5f, 0.5f, 0.5f), ivec2(0, m_buttonSize.y), ivec2(m_screenSize.x, m_screenSize.y) };
			m_visuals.push_back(visual);
		}
		else
		{
			Visual visual = { vec3(0.5f, 0.5f, 0.5f), ivec2(0, 0), m_buttonSize };
			m_visuals.push_back(visual);
		}
	}
    
    m_positionSlot = m_glProgram.getAttribute("Position");
    m_colorSlot = m_glProgram.getAttribute("SourceColor");
    
    m_positionSlot->enableVertexAttribArray();
    m_translation = mat4::Translate(0, 0, -7);
    m_projectionUniform = m_glProgram.getUniform("Projection");
    m_modelviewUniform = m_glProgram.getUniform("Modelview");
}


void SimpleWireframe::render()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    int visualIndex = 0;
	auto  visual = m_visuals.begin();
	for (; visual != m_visuals.end(); visual++, visualIndex++)
	{
		ivec2 size = visual->ViewportSize;
		ivec2 lowerLeft = visual->LowerLeft;
		glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);
		
		mat4 rotation = mat4::Rotate(m_rotationAngle, vec3(0.0f, 1.0f, 0.0f));
		//mat4 rotation = Orientation.ToMatrix();
		mat4 modelview = rotation * m_translation;
		m_modelviewUniform->matrix4fv(1, 0, modelview.Pointer());

		// Set the projection transform.
		float h = 4.0f * 480 / 320;
		mat4 projectionMatrix = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
		m_projectionUniform->matrix4fv(1, 0, projectionMatrix.Pointer());

		// Set the color.
		vec3 color = visual->Color;
		m_colorSlot->vertexAttrib4f(color.x, color.y, color.z, 1);

		// Draw the wireframe.
		int stride = sizeof(vec3);
		Drawable& drawable = m_drawables[visualIndex];
		drawable.vertex.bind(GL_ARRAY_BUFFER);
		m_positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
		drawable.index.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_LINES, drawable.indexCount, GL_UNSIGNED_SHORT, 0);
	}
}

void SimpleWireframe::onTouchBegan(float x, float y)
{
	m_previousX = x;
	m_previousY = y;
	m_rotationAngle = m_lastRotationAngle;
}

void SimpleWireframe::onTouchMove(float x, float y)
{
	float detha = x - m_previousX;
	float anglePerPix = 30/15; //30¶ÈÃ»15ÏñËØ
	m_rotationAngle = m_lastRotationAngle - (detha * anglePerPix);
}

void SimpleWireframe::onTouchEnd(float x, float y)
{
	m_lastRotationAngle = m_rotationAngle;
}
