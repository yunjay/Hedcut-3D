#pragma once
#include <GL/glew.h>
#include <memory>
#include <vector>

typedef std::shared_ptr<Texture> spTexture;

class Texture {
public:
	Texture(int width, int height, GLenum format, void * pixels) :
		m_width{ width }, m_height{ height }, m_format{ format }
	{

	}
	int GetWidth() const;
	int GetHeight() const;
	//GL_RGBA8, GL_RGBA32F, GL_DEPTH_COMPONENT, GL_DEPTH24_STENCIL8
	GLenum GetImageFormat() const;
	GLuint GetHandle() const;

	bool GetData(std::vector<float>& pixels) const;

	void allocateVRAM(void* pixels);

	// static functions
	GLenum GetBasePixelFormat(GLenum format);
	GLenum GetPixelFormatDataType(GLenum format);
private:
	GLuint m_handle;
	int m_width, m_height;
	GLenum m_format; //internal format

};
