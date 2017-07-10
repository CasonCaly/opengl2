#ifndef GL_Image_h
#define GL_Image_h
#include <string>
#include "GLCore.h"


class GLCORE GLImage{

public:

	bool initWithImage(const std::string& path);

protected:

	bool initWithPngData(const unsigned char * data, size_t dataLen);

protected:

	size_t m_width;
	size_t m_height;
	size_t m_dataLen;
	unsigned char* m_data;
};
#endif