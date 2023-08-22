#include "Texture.h"

Texture::Texture(int width, int height, GLenum format, void* pixels = nullptr) :
	m_width{ width }, m_height{ height }, m_format{ format }
{
	this->allocateVRAM(pixels);
}
void Texture::allocateVRAM(void* pixels = nullptr) {
	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);
	GLenum internalFormat = GL_NONE;
	GLenum pixelFormat = GL_NONE;
	GLenum pixelDataType = GL_NONE;

	//TODO
}


int Texture::GetWidth() const {
	return m_width;
}
int Texture::GetHeight() const {
	return m_height;
}
//GL_RGBA8, GL_RGBA32F, GL_DEPTH_COMPONENT, GL_DEPTH24_STENCIL8
GLenum Texture::GetImageFormat() const {
	return m_format;
}
GLuint Texture::GetHandle() const {
	return m_handle;
}
bool Texture::GetData(std::vector<float>& pixels) const {
	int size = m_width * m_height;
	if (m_format == GL_DEPTH_COMPONENT) {
		std::vector<float> textureData(size);
		//glBindTexture(GL_TEXTURE_2D, handle);
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, textureData.data());
		//glBindTexture(GL_TEXTURE_2D, 0);
		glGetTextureImage(m_handle, 0, GL_DEPTH_COMPONENT, GL_FLOAT, size, textureData.data());
		pixels = textureData;

		return true;
	}
	else if (m_format == GL_RGBA8) {

		std::vector<unsigned char> textureData(size * 4);
		glGetTextureImage(m_handle, 0, GL_RGBA, GL_UNSIGNED_BYTE, size, textureData.data());

		std::vector<float> pixelData(size);
#pragma omp parallel for
		for (int i = 0; i < textureData.size(); i++) {
			float val = static_cast<float>(textureData[i] / 255.0f);
			pixelData[i] = val;
		}

		pixels = pixelData;

		return true;
	}
	else if (m_format == GL_RGBA32F) {
		std::vector<float> textureData(size * 4);
		glGetTextureImage(m_handle, 0, GL_RGBA, GL_FLOAT, size, textureData.data());
		pixels = textureData;

		return true;
	}
	// Can add other formats if needed.
	else {
		return false;
	}

}
void Texture::allocateVRAM(void* pixels) {
	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);
	GLenum internalFormat = m_format;
	GLenum pixelFormat = GL_NONE;
	GLenum pixelDataType = GL_NONE;

	//TODO
}

// static functions

GLenum Texture::GetBasePixelFormat(GLenum format) {
	if (format == GL_RGBA8 || format == GL_RGBA32F) return GL_RGBA;
	else if (format == GL_R32F) return GL_RED;
	else if (format == GL_DEPTH24_STENCIL8) return GL_DEPTH_STENCIL;
	else if (format == GL_DEPTH_COMPONENT32F || format == GL_DEPTH_COMPONENT) return GL_DEPTH_COMPONENT;
	else return GL_RGBA;
}
GLenum Texture::GetPixelFormatDataType(GLenum format) {
	if (format == GL_RGBA8) return GL_UNSIGNED_BYTE;
	else if (format == GL_R32F || format == GL_RGBA32F || format == GL_DEPTH_COMPONENT32F) return GL_FLOAT;
	else if (format == GL_DEPTH24_STENCIL8) return GL_UNSIGNED_INT_24_8;
	else return GL_FLOAT;
}
