#include "SimpleProgram.h"
#include "glsurface/GLSurface.h"

void SimpleProgram::init()
{
	this->use();
	m_positionSlot = this->getAttribute("Position");
	m_colorSlot = this->getAttribute("SourceColor");
	m_normalSlot = this->getAttribute("Normal");
	m_diffuseMaterialSlot = this->getAttribute("DiffuseMaterial");

	//投影有关的uniform变量
	m_projectionUniform = this->getUniform("Projection");
	m_modelviewUniform = this->getUniform("Modelview");
	//光照有关的uniform变量
	m_normalMatrixUniform = this->getUniform("NormalMatrix");
	m_lightPositionUniform = this->getUniform("LightPosition");
	m_ambientMaterialUniform = this->getUniform("AmbientMaterial");
	m_specularMaterialUniform = this->getUniform("SpecularMaterial");
	m_shininessUniform = this->getUniform("Shininess");

	m_ambientMaterialUniform->value3f(0.04f, 0.04f, 0.04f);
	m_specularMaterialUniform->value3f(0.5f, 0.5f, 0.5f);
	m_shininessUniform->value1f(50.0f);

	m_positionSlot->enableVertexAttribArray();
	m_normalSlot->enableVertexAttribArray();
}

void SimpleProgram::useWith(GLSurface* surface, mat4& translation)
{
	this->use();
	vec2 size = surface->getViewportSize();
	vec2 lowerLeft = surface->getLowerLeft();
	glViewport((int)lowerLeft.x, (int)lowerLeft.y, (int)size.x, (int)size.y);

	vec4 lightPosition(0.25f, 0.25f, 1.0f, 0.0f);
	m_lightPositionUniform->vector3fv(1, lightPosition.Pointer());

	Quaternion& orientation = surface->getOrientation();
	mat4 rotation = orientation.ToMatrix();
	mat4 modelview = rotation * translation;

	m_modelviewUniform->matrix4fv(1, 0, modelview.Pointer());

	//设置法线矩阵
	mat3 normalMatrix = modelview.ToMat3();
	m_normalMatrixUniform->matrix3fv(1, 0, normalMatrix.Pointer());

	// 设置投影矩阵
	float h = 4.0f * size.y / size.x;
	mat4 projectionMatrix = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
	m_projectionUniform->matrix4fv(1, 0, projectionMatrix.Pointer());

	vec3 color = surface->getColor();
	color = color * 0.75f;
	m_diffuseMaterialSlot->vertexAttrib4f(color.x, color.y, color.z, 1);

	// Draw the wireframe.
	int stride = 2 * sizeof(vec3);
	const GLvoid* offset = (const GLvoid*)sizeof(vec3);

	GLBuffer& vertexBuffer = surface->getVertexBuffer();

	GLBuffer& trianlgeIndexBuffer = surface->getTriangleIndexBuffer();
	int triangelIndexCount = surface->getTriangleIndexCount();


	vertexBuffer.bind(GL_ARRAY_BUFFER);
	m_positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
	m_normalSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, offset);

	trianlgeIndexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, triangelIndexCount, GL_UNSIGNED_SHORT, 0);
}