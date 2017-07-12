#include "GLTexture.h"

void GLTexture::activeTexture(GLenum texture)
{
	glActiveTexture(texture);
}

void GLTexture::generateMipmap(GLenum target)
{
	glGenerateMipmap(target);
}

void GLTexture::genTextures(){
	glGenTextures(1, &m_textures);
}

void GLTexture::bindTexture(GLenum target){
	glBindTexture(target, m_textures);
}

void GLTexture::texParameteri(GLenum target, GLenum pname, GLint param)
{
	glTexParameteri(target, pname, param);
}

void GLTexture::texImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}