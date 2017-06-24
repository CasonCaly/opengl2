#include <cmath>
#include "SimpleWireframe.h"

SimpleWireframe::SimpleWireframe()
{
	m_currentSurface = 4;
}

void SimpleWireframe::init()
{
	m_buttonSize.y = 480 / 10;
	m_buttonSize.x = 4 * m_buttonSize.y / 3;

	m_screenSize.x = 320;
	m_screenSize.y = 480 - m_buttonSize.y;

	m_surfaces.push_back(new GLCone(3, 1, ivec2(20, 20)));
	m_surfaces.push_back(new GLSphere(2, ivec2(20, 20)));
	m_surfaces.push_back(new GLMobiusStrip(1, ivec2(40, 40)));
	m_surfaces.push_back(new GLTorus(1.4f, 0.3f, ivec2(20, 20)));
	m_surfaces.push_back(new GLTrefoilKnot(1.8f, ivec2(60, 15)));
	m_surfaces.push_back(new GLKleinBottle(0.2f, ivec2(20, 20)));
	
	size_t buttonCount = m_surfaces.size() - 1;
	size_t buttonIndex = 0;
	for (size_t i = 0; i < m_surfaces.size(); i++){
		GLSurface* surface = m_surfaces[i];
		surface->generateVertices();
		surface->generateLineIndices();
		surface->generateTriangleIndices();

		surface->setColor(vec3(1.0f, 1.0f, 1.0f));
		if (i == m_currentSurface){
			surface->setLowerLeft(ivec2(0, m_buttonSize.y));
			surface->setViewportSize(m_screenSize);
		}
		else{
			surface->setLowerLeft(ivec2(buttonIndex * m_buttonSize.x, 0));
			surface->setViewportSize(m_buttonSize);
			buttonIndex++;
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
    glClearColor(0.0f, 0.0f, 0.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	for (auto begin = m_surfaces.begin(); begin != m_surfaces.end(); begin++){
		GLSurface* surface = *begin;
		ivec2 size = surface->getViewportSize();
		ivec2 lowerLeft = surface->getLowerLeft();
		glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);
		
		mat4 rotation = mat4::Rotate(m_rotationAngle, vec3(0.0f, 1.0f, 0.0f));
		//mat4 rotation = Orientation.ToMatrix();
		mat4 modelview = rotation * m_translation;
		m_modelviewUniform->matrix4fv(1, 0, modelview.Pointer());

		// Set the projection transform.
		float h = 4.0f * size.y / size.x;
		mat4 projectionMatrix = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
		m_projectionUniform->matrix4fv(1, 0, projectionMatrix.Pointer());

		// Set the color.
		vec3 color = surface->getColor();
		m_colorSlot->vertexAttrib4f(color.x, color.y, color.z, 1);

		// Draw the wireframe.
		int stride = sizeof(vec3);
		GLBuffer& vertexBuffer = surface->getVertexBuffer();
		GLBuffer& trianlgeIndexBuffer = surface->getTriangleIndexBuffer();
		//GLBuffer& indexBuffer = surface->getIndexBuffer();
		//int indexCount = surface->getLineIndexCount();
		int triangelIndexCount = surface->getTriangleIndexCount();
		vertexBuffer.bind(GL_ARRAY_BUFFER);
		m_positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
		trianlgeIndexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, triangelIndexCount, GL_UNSIGNED_SHORT, 0);
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
