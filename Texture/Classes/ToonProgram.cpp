#include "ToonProgram.h"
#include "glsurface/GLSurface.h"

void ToonProgram::init()
{
	this->use();
	m_toonPosition = this->getAttribute("Position");
	m_toonNormal = this->getAttribute("Normal");
	m_toonDiffuseMaterial = this->getAttribute("DiffuseMaterial");

	m_toonProjection = this->getUniform("Projection");
	m_toonModelview = this->getUniform("Modelview");
	m_toonNormalMatrix = this->getUniform("NormalMatrix");

	m_toonLightPosition = this->getUniform("LightPosition");
	m_toonAmbientMaterial = this->getUniform("AmbientMaterial");
	m_toonSpecularMaterial = this->getUniform("SpecularMaterial");
	m_toonShininess = this->getUniform("Shininess");

	m_toonPosition->enableVertexAttribArray();
	m_toonNormal->enableVertexAttribArray();

	m_toonAmbientMaterial->value3f(0.04f, 0.04f, 0.04f);
	m_toonSpecularMaterial->value3f(0.5f, 0.5f, 0.5f);
	m_toonShininess->value1f(50.0f);
}

void ToonProgram::useWith(GLSurface* surface, mat4& translation)
{
	m_toonLightProgram.use();
	vec2 size = surface->getViewportSize();
	vec2 lowerLeft = surface->getLowerLeft();
	glViewport((int)lowerLeft.x, (int)lowerLeft.y, (int)size.x, (int)size.y);

	vec4 lightPosition(0.25f, 0.25f, 1.0f, 0.0f);
	m_toonLightPosition->vector3fv(1, lightPosition.Pointer());

	Quaternion& orientation = surface->getOrientation();
	mat4 rotation = orientation.ToMatrix();
	mat4 modelview = rotation * translation;

	m_toonModelview->matrix4fv(1, 0, modelview.Pointer());

	//设置法线矩阵
	mat3 normalMatrix = modelview.ToMat3();
	m_toonNormalMatrix->matrix3fv(1, 0, normalMatrix.Pointer());

	// 设置投影矩阵
	float h = 4.0f * size.y / size.x;
	mat4 projectionMatrix = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
	m_toonProjection->matrix4fv(1, 0, projectionMatrix.Pointer());

	vec3 color = surface->getColor();
	color = color * 0.75f;
	m_toonDiffuseMaterial->vertexAttrib4f(color.x, color.y, color.z, 1);

	// Draw the wireframe.
	int stride = 2 * sizeof(vec3);
	const GLvoid* offset = (const GLvoid*)sizeof(vec3);

	GLBuffer& vertexBuffer = surface->getVertexBuffer();

	GLBuffer& trianlgeIndexBuffer = surface->getTriangleIndexBuffer();
	int triangelIndexCount = surface->getTriangleIndexCount();

	vertexBuffer.bind(GL_ARRAY_BUFFER);
	m_toonPosition->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
	m_toonNormal->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, offset);

	trianlgeIndexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, triangelIndexCount, GL_UNSIGNED_SHORT, 0);
}