#include <cmath>
#include "SimpleWireframe.h"
#define SurfaceCount (6)
#define ButtonCount (5)

SimpleWireframe::SimpleWireframe()
{
	m_pressedButton = -1;
	m_animation.Active = false;
	m_spinning = false;
	m_currentSurface = 2;
    m_rotationAngle = 0.0f;
    m_lastRotationAngle = 0.0f;

	m_animation.Duration = 0.25f;
}

void SimpleWireframe::init()
{
	m_trackballRadius = 320 / 3;

	m_buttonSize.y = 480 / 10;
	m_buttonSize.x = 4 * m_buttonSize.y / 3;

	m_screenSize.x = 320;
	m_screenSize.y = 480 - m_buttonSize.y;

	m_centerPoint = m_screenSize / 2;

	m_surfaces.push_back(new GLCone(3, 1, ivec2(20, 20)));
	m_surfaces.push_back(new GLSphere(2, ivec2(20, 20)));
	m_surfaces.push_back(new GLTorus(1.4f, 0.3f, ivec2(20, 20)));
	m_surfaces.push_back(new GLTrefoilKnot(1.8f, ivec2(60, 15)));
	m_surfaces.push_back(new GLKleinBottle(0.2f, ivec2(20, 20)));
	m_surfaces.push_back(new GLMobiusStrip(1, ivec2(40, 40)));
	
	size_t buttonIndex = 0;
	for (size_t i = 0; i < m_surfaces.size(); i++){
		GLSurface* surface = m_surfaces[i];
		surface->generateVertices();
		surface->generateLineIndices();
		surface->generateTriangleIndices();

		surface->setColor(vec3(1.0f, 1.0f, 0.5f));
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

void SimpleWireframe::updateSurface()
{
	if (m_animation.Active){

	}
	else{
		float t = m_animation.Elapsed / m_animation.Duration;
	}
}

void SimpleWireframe::renderSurface()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto begin = m_surfaces.begin(); begin != m_surfaces.end(); begin++){
		GLSurface* surface = *begin;
		ivec2 size = surface->getViewportSize();
		ivec2 lowerLeft = surface->getLowerLeft();
		glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);

		Quaternion& orientation = surface->getOrientation();
		//mat4 rotation = mat4::Rotate(m_rotationAngle, vec3(0.0f, 1.0f, 0.0f));
		mat4 rotation = orientation.ToMatrix();
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
		//GLBuffer& trianlgeIndexBuffer = surface->getTriangleIndexBuffer();
		//int triangelIndexCount = surface->getTriangleIndexCount();

		GLBuffer& indexBuffer = surface->getIndexBuffer();
		int indexCount = surface->getLineIndexCount();
		
		vertexBuffer.bind(GL_ARRAY_BUFFER);
		m_positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
		//trianlgeIndexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		//glDrawElements(GL_TRIANGLES, triangelIndexCount, GL_UNSIGNED_SHORT, 0);
		glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_SHORT, 0);
	}
}

void SimpleWireframe::render()
{
	this->updateSurface();
	this->renderSurface();
}

void SimpleWireframe::onTouchBegan(float x, float y)
{
	ivec2 location((int)x, (int)y);
	m_fingerStart = location;
	m_previousOrientation = m_orientation;
	m_pressedButton = this->mapToButton(location);
	if (m_pressedButton == -1)
		m_spinning = true;

}

void SimpleWireframe::onTouchMove(float x, float y)
{
	ivec2 location((int)x, (int)y);
	if (m_spinning) {
		vec3 start = this->mapToSphere(m_fingerStart);
		vec3 end = this->mapToSphere(location);
		Quaternion delta = Quaternion::CreateFromVectors(start, end);
		m_orientation = delta.Rotated(m_previousOrientation);
		m_surfaces[m_currentSurface]->setOrientation(m_orientation);
	}
}

void SimpleWireframe::onTouchEnd(float x, float y)
{
	if (!m_spinning && m_pressedButton != -1) {
		GLSurface* button = m_surfaces[m_pressedButton];
		GLSurface* sphere = m_surfaces[m_currentSurface];

		Quaternion sphereOrien = sphere->getOrientation();
		ivec2 sphereLowerLeft =	sphere->getLowerLeft();
		ivec2 sphereViewSize = sphere->getViewportSize();

		Quaternion buttonOrien = button->getOrientation();
		ivec2 buttonLowerLeft = button->getLowerLeft();
		ivec2 buttonViewSize = button->getViewportSize();

		button->setLowerLeft(sphereLowerLeft);
		button->setViewportSize(sphereViewSize);
		button->setOrientation(sphereOrien);

		sphere->setLowerLeft(buttonLowerLeft);
		sphere->setViewportSize(buttonViewSize);
		sphere->setOrientation(buttonOrien);
		m_currentSurface = m_pressedButton;
	}
	m_pressedButton = -1;
	m_spinning = false;
}

vec3 SimpleWireframe::mapToSphere(ivec2 touchpoint)
{
	ivec2 p = touchpoint - m_centerPoint;

	// Flip the Y axis because pixel coords increase towards the bottom.
	p.y = -p.y;

	const float radius = m_trackballRadius;
	const float safeRadius = radius - 1;

	if (p.Length() > safeRadius) {
		float theta = atan2(p.y, p.x);
		p.x = safeRadius * cos(theta);
		p.y = safeRadius * sin(theta);
	}

	float z = sqrt(radius * radius - p.LengthSquared());
	vec3 mapped = vec3(p.x, p.y, z);
	return mapped / radius;
}

int SimpleWireframe::mapToButton(ivec2 touchpoint)
{
	if (touchpoint.y < m_screenSize.y - m_buttonSize.y)
		return -1;

	int buttonIndex = touchpoint.x / m_buttonSize.x;
	if (buttonIndex >= ButtonCount)
		return -1;

	return buttonIndex;
}