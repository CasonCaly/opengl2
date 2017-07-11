#include <cmath>
#include "Texture.h"
#include "ObjSurface.h"
#include "os/Path.h"
#include "image/GLImage.h"
#include "glclass/GLBuffer.h"
#include "glsurface/GLCone.h"
#include "glsurface/GLSphere.h"
#include "glsurface/GLMobiusStrip.h"
#include "glsurface/GLTorus.h"
#include "glsurface/GLKleinBottle.h"
#include "glsurface/GLTrefoilKnot.h"



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
    //new GLMobiusStrip(1, ivec2(40, 40));
	m_surfaces.push_back(new GLCone(3, 1, ivec2(20, 20)));
	m_surfaces.push_back(new GLTorus(1.4f, 0.3f, ivec2(20, 20)));
	m_surfaces.push_back(new GLSphere(1.4f, ivec2(20, 20)));
	m_surfaces.push_back(new GLKleinBottle(0.2f, ivec2(20, 20)));
	

	GLSurface* toon = new ObjSurface("Models/micronapalmv2.obj");//new GLMobiusStrip(1, ivec2(40, 40));
	//toon->setName("toon");
	m_surfaces.push_back(toon);
    
    m_showedSurface = new GLTrefoilKnot(1.8f, ivec2(60, 15));
    m_showedSurface->setTextureCount(vec2(100, 8) );
    m_showedSurface->setEnableTexture(true);
    m_showedSurface->setName("texture");
    m_surfaces.push_back(m_showedSurface);

//	ObjSurface* objSurface = new ObjSurface("Models/micronapalmv2.obj");
//	objSurface->setName("pixel");
//	m_surfaces.push_back(objSurface);

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

	m_simpleProgram.init();
	m_pixelLightProgram.init();
	m_toonProgram.init();
    m_texProgram.init();
	glEnable(GL_DEPTH_TEST);

	m_translation = mat4::Translate(0, 0, -7);
}

void Texture::updateSurface()
{
}

void Texture::renderSurface()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto begin = m_surfaces.begin(); begin != m_surfaces.end(); begin++){
		GLSurface* surface = *begin;
		std::string& name = surface->getName();
		if (name == "pixel"){
			m_pixelLightProgram.useWith(surface, m_translation);
		}
		else if(name == "toon"){
			m_toonProgram.useWith(surface, m_translation);
		}
        else if(name == "texture"){
            m_texProgram.useWith(surface, m_translation);
        }
		else{
			m_simpleProgram.useWith(surface, m_translation);
		}
	}
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
	this->createPrograme("Shaders/Simple.vert", "Shaders/Simple.frag", m_simpleProgram);
	this->createPrograme("Shaders/PixelLighting.es2.vert", "Shaders/PixelLighting.es2.frag", m_pixelLightProgram);
	this->createPrograme("Shaders/ToonLighting.es2.vert", "Shaders/ToonLighting.es2.frag", m_toonProgram);
	this->createPrograme("Shaders/TexturedLighting.vert", "Shaders/TexturedLighting.frag", m_texProgram);
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
