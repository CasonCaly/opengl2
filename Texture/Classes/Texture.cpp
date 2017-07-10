#include <cmath>
#include "Texture.h"
#include "os/Path.h"
#include "glclass/GLBuffer.h"
#include "glsurface/GLCone.h"
#include "glsurface/GLSphere.h"
#include "glsurface/GLMobiusStrip.h"
#include "glsurface/GLTorus.h"
#include "glsurface/GLKleinBottle.h"
#include "glsurface/GLTrefoilKnot.h"
#include "ObjSurface.h"

#define SurfaceCount (6)
#define ButtonCount (5)

Texture::Texture()
{
    m_showedSurface = nullptr;
	m_pressedButton = -1;
	m_animation.Active = false;
	m_spinning = false;
	m_currentSurface = 3;
	m_animation.Duration = 0.25f;
}

void Texture::init()
{
	m_trackballRadius = 320 / 3;
	m_buttonSize.y = 480 / 10;
	m_buttonSize.x = 4 * m_buttonSize.y / 3;
	m_screenSize.x = 320;
	m_screenSize.y = 480 - m_buttonSize.y;
	m_centerPoint = m_screenSize / 2;

	//new GLSphere(1.4f, ivec2(20, 20)
	//GLTrefoilKnot(1.8f, ivec2(60, 15))
	m_surfaces.push_back(new GLCone(3, 1, ivec2(20, 20)));
	m_surfaces.push_back(new GLTorus(1.4f, 0.3f, ivec2(20, 20)));
	m_surfaces.push_back(new GLSphere(1.4f, ivec2(20, 20)));
	m_surfaces.push_back(new GLKleinBottle(0.2f, ivec2(20, 20)));
	

	GLSurface* toon = new ObjSurface("Models/micronapalmv2.obj");//new GLMobiusStrip(1, ivec2(40, 40));
	//toon->setName("toon");
	m_surfaces.push_back(toon);
    
    //m_showedSurface = new GLTrefoilKnot(1.8f, ivec2(60, 15));
    //m_showedSurface->setName("sphere");
    //m_surfaces.push_back(m_showedSurface);

	ObjSurface* objSurface = new ObjSurface("Models/micronapalmv2.obj");
	objSurface->setName("pixel");
	m_surfaces.push_back(objSurface);

	size_t buttonIndex = 0;
	for (size_t i = 0; i < m_surfaces.size(); i++){
		GLSurface* surface = m_surfaces[i];
		surface->setEnableVertexNormal(true);
		surface->generateVertices();
		surface->generateLineIndices();
		surface->generateTriangleIndices();

		surface->setColor(vec3(1.0f, 1.0f, 0.5f));
		if ((i + 1)== m_surfaces.size()){
			surface->setLowerLeft(vec2(0.0f, m_buttonSize.y));
			surface->setViewportSize(m_screenSize);
		}
		else{
			surface->setLowerLeft(vec2(buttonIndex * m_buttonSize.x, 0.0f));
			surface->setViewportSize(m_buttonSize);
			buttonIndex++;
		}
	}

	this->initVertexAttributeAndUniform();
	this->initPixelAttributeAndUniform();
	this->initToonAttributeAndUnifrom();
	glEnable(GL_DEPTH_TEST);

	m_translation = mat4::Translate(0, 0, -7);
}

void Texture::updateSurface()
{
}

void Texture::initVertexAttributeAndUniform()
{
	m_glProgram.use();
	m_positionSlot = m_glProgram.getAttribute("Position");
	m_colorSlot = m_glProgram.getAttribute("SourceColor");
	m_normalSlot = m_glProgram.getAttribute("Normal");
	m_diffuseMaterialSlot = m_glProgram.getAttribute("DiffuseMaterial");

	//投影有关的uniform变量
	m_projectionUniform = m_glProgram.getUniform("Projection");
	m_modelviewUniform = m_glProgram.getUniform("Modelview");
	//光照有关的uniform变量
	m_normalMatrixUniform = m_glProgram.getUniform("NormalMatrix");
	m_lightPositionUniform = m_glProgram.getUniform("LightPosition");
	m_ambientMaterialUniform = m_glProgram.getUniform("AmbientMaterial");
	m_specularMaterialUniform = m_glProgram.getUniform("SpecularMaterial");
	m_shininessUniform = m_glProgram.getUniform("Shininess");

	m_ambientMaterialUniform->value3f(0.04f, 0.04f, 0.04f);
	m_specularMaterialUniform->value3f(0.5f, 0.5f, 0.5f);
	m_shininessUniform->value1f(50.0f);

	m_positionSlot->enableVertexAttribArray();
	m_normalSlot->enableVertexAttribArray();
}

void Texture::initPixelAttributeAndUniform()
{
	m_pixelLightProgram.use();
	m_pixelPosition = m_pixelLightProgram.getAttribute("Position");
	m_pixelNormal = m_pixelLightProgram.getAttribute("Normal");
	m_pixelDiffuseMaterial = m_pixelLightProgram.getAttribute("DiffuseMaterial");

	m_pixelProjection = m_pixelLightProgram.getUniform("Projection");
	m_pixelModelview = m_pixelLightProgram.getUniform("Modelview");
	m_pixelNormalMatrix = m_pixelLightProgram.getUniform("NormalMatrix");

	m_pixelLightPosition = m_pixelLightProgram.getUniform("LightPosition");
	m_pixelAmbientMaterial = m_pixelLightProgram.getUniform("AmbientMaterial");
	m_pixelSpecularMaterial = m_pixelLightProgram.getUniform("SpecularMaterial");
	m_pixelShininess = m_pixelLightProgram.getUniform("Shininess");

    m_pixelPosition->enableVertexAttribArray();
    m_pixelNormal->enableVertexAttribArray();
    
	m_pixelAmbientMaterial->value3f(0.04f, 0.04f, 0.04f);
	m_pixelSpecularMaterial->value3f(0.5f, 0.5f, 0.5f);
	m_pixelShininess->value1f(50.0f);
}

void Texture::initToonAttributeAndUnifrom()
{
	m_toonLightProgram.use();
	m_toonPosition = m_toonLightProgram.getAttribute("Position");
	m_toonNormal = m_toonLightProgram.getAttribute("Normal");
	m_toonDiffuseMaterial = m_toonLightProgram.getAttribute("DiffuseMaterial");

	m_toonProjection = m_toonLightProgram.getUniform("Projection");
	m_toonModelview = m_toonLightProgram.getUniform("Modelview");
	m_toonNormalMatrix = m_toonLightProgram.getUniform("NormalMatrix");

	m_toonLightPosition = m_toonLightProgram.getUniform("LightPosition");
	m_toonAmbientMaterial = m_toonLightProgram.getUniform("AmbientMaterial");
	m_toonSpecularMaterial = m_toonLightProgram.getUniform("SpecularMaterial");
	m_toonShininess = m_toonLightProgram.getUniform("Shininess");

	m_toonPosition->enableVertexAttribArray();
	m_toonNormal->enableVertexAttribArray();

	m_toonAmbientMaterial->value3f(0.04f, 0.04f, 0.04f);
	m_toonSpecularMaterial->value3f(0.5f, 0.5f, 0.5f);
	m_toonShininess->value1f(50.0f);
}

void Texture::renderSurface()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto begin = m_surfaces.begin(); begin != m_surfaces.end(); begin++){
		GLSurface* surface = *begin;
		std::string& name = surface->getName();
		if (name == "pixel"){
			this->renderUsePixel(surface);
		}
		else if(name == "toon"){
			this->renderUseToon(surface);
		}
		else{
			this->renderUseVertex(surface);
		}
	}
}

void Texture::renderUseVertex(GLSurface* surface)
{
	m_glProgram.use();
	vec2 size = surface->getViewportSize();
	vec2 lowerLeft = surface->getLowerLeft();
	glViewport((int)lowerLeft.x, (int)lowerLeft.y, (int)size.x, (int)size.y);

	vec4 lightPosition(0.25f, 0.25f, 1.0f, 0.0f);
	m_lightPositionUniform->vector3fv(1, lightPosition.Pointer());

	Quaternion& orientation = surface->getOrientation();
	mat4 rotation = orientation.ToMatrix();
	mat4 modelview = rotation * m_translation;

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

void Texture::renderUsePixel(GLSurface* surface)
{
	m_pixelLightProgram.use();
	vec2 size = surface->getViewportSize();
	vec2 lowerLeft = surface->getLowerLeft();
	glViewport((int)lowerLeft.x, (int)lowerLeft.y, (int)size.x, (int)size.y);

	vec4 lightPosition(0.25f, 0.25f, 1.0f, 0.0f);
	m_pixelLightPosition->vector3fv(1, lightPosition.Pointer());

	Quaternion& orientation = surface->getOrientation();
	mat4 rotation = orientation.ToMatrix();
	mat4 modelview = rotation * m_translation;

	m_pixelModelview->matrix4fv(1, 0, modelview.Pointer());

	//设置法线矩阵
	mat3 normalMatrix = modelview.ToMat3();
	m_pixelNormalMatrix->matrix3fv(1, 0, normalMatrix.Pointer());

	// 设置投影矩阵
	float h = 4.0f * size.y / size.x;
	mat4 projectionMatrix = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
	m_pixelProjection->matrix4fv(1, 0, projectionMatrix.Pointer());

	vec3 color = surface->getColor();
	color = color * 0.75f;
	m_pixelDiffuseMaterial->vertexAttrib4f(color.x, color.y, color.z, 1);

	// Draw the wireframe.
	int stride = 2 * sizeof(vec3);
	const GLvoid* offset = (const GLvoid*)sizeof(vec3);

	GLBuffer& vertexBuffer = surface->getVertexBuffer();

	GLBuffer& trianlgeIndexBuffer = surface->getTriangleIndexBuffer();
	int triangelIndexCount = surface->getTriangleIndexCount();

	vertexBuffer.bind(GL_ARRAY_BUFFER);
	m_pixelPosition->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
	m_pixelNormal->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, offset);

	trianlgeIndexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
	glDrawElements(GL_TRIANGLES, triangelIndexCount, GL_UNSIGNED_SHORT, 0);
}

void Texture::renderUseToon(GLSurface* surface)
{
	m_toonLightProgram.use();
	vec2 size = surface->getViewportSize();
	vec2 lowerLeft = surface->getLowerLeft();
	glViewport((int)lowerLeft.x, (int)lowerLeft.y, (int)size.x, (int)size.y);

	vec4 lightPosition(0.25f, 0.25f, 1.0f, 0.0f);
	m_toonLightPosition->vector3fv(1, lightPosition.Pointer());

	Quaternion& orientation = surface->getOrientation();
	mat4 rotation = orientation.ToMatrix();
	mat4 modelview = rotation * m_translation;

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

void Texture::render()
{
	this->updateSurface();
	this->renderSurface();
}

void Texture::onTouchBegan(float x, float y)
{
	vec2 location(x, y);
	m_fingerStart = location;
	m_previousOrientation = m_orientation;
	m_pressedButton = this->mapToButton(location);
	if (m_pressedButton == -1)
		m_spinning = true;

}

void Texture::onTouchMove(float x, float y)
{
	vec2 location(x, y);
	if (m_spinning) {
		vec3 start = this->mapToSphere(m_fingerStart);
		vec3 end = this->mapToSphere(location);
		Quaternion delta = Quaternion::CreateFromVectors(start, end);
		m_orientation = delta.Rotated(m_previousOrientation);
		m_surfaces[m_surfaces.size() - 1]->setOrientation(m_orientation);
	}
}

void Texture::onTouchEnd(float x, float y)
{
	if (!m_spinning && m_pressedButton != -1) {
		GLSurface* button = m_surfaces[m_pressedButton];
		GLSurface* sphere = m_surfaces[m_surfaces.size() - 1];

		Quaternion sphereOrien = sphere->getOrientation();
		vec2 sphereLowerLeft =	sphere->getLowerLeft();
		vec2 sphereViewSize = sphere->getViewportSize();

		Quaternion buttonOrien = button->getOrientation();
		vec2 buttonLowerLeft = button->getLowerLeft();
		vec2 buttonViewSize = button->getViewportSize();

		button->setLowerLeft(sphereLowerLeft);
		button->setViewportSize(sphereViewSize);
		button->setOrientation(sphereOrien);

		sphere->setLowerLeft(buttonLowerLeft);
		sphere->setViewportSize(buttonViewSize);
		sphere->setOrientation(buttonOrien);
        
        
        m_surfaces[m_surfaces.size() - 1] = button;
        m_surfaces[m_pressedButton] = sphere;
 		m_currentSurface = m_pressedButton;
	}
	m_pressedButton = -1;
	m_spinning = false;
}

void Texture::initProgram()
{
	GLApp::initProgram();
	this->createPrograme("Shaders/PixelLighting.es2.vert", "Shaders/PixelLighting.es2.frag", m_pixelLightProgram);
	this->createPrograme("Shaders/ToonLighting.es2.vert", "Shaders/ToonLighting.es2.frag", m_toonLightProgram);
}

//该函数的含义在于把一个屏幕的一个点，映射成一个球体表面的点，
//球体的半径为可以自己定义，这边取为屏幕的3分之一，同时球体的中心在屏幕中心。
//通过球体参数方程 x*x + y*y + z*z = r*r这个公式来逆推出z的坐标
vec3 Texture::mapToSphere(vec2 touchpoint)
{
	vec2 p = touchpoint - m_centerPoint;
	// Flip the Y axis because pixel coords increase towards the bottom.
	p.y = -p.y;

	const float radius = m_trackballRadius;
	const float safeRadius = radius - 1;

	if (p.Length() > safeRadius) {
		float theta = atan2f((float)p.y, (float)p.x);
		p.x = (safeRadius * cos(theta));
		p.y = (safeRadius * sin(theta));
	}

	float z = sqrt(radius * radius - p.LengthSquared());//这个写法相当于  r*r - (x*x + y*y)这种写法，开平方之后得出就是z
	vec3 mapped = vec3((float)p.x, (float)p.y, z);
	return mapped / radius;//这个结果和归一化结果类似，因为 x*x/r*r + y*y/r*r + z*z/r*r = r*r/r*r 也就是结果为1
}
 
int Texture::mapToButton(vec2 touchpoint)
{
	if (touchpoint.y < m_screenSize.y - m_buttonSize.y)
		return -1;

	int buttonIndex = (int)touchpoint.x / (int)m_buttonSize.x;
	if (buttonIndex >= ButtonCount)
		return -1;

	return buttonIndex;
}
