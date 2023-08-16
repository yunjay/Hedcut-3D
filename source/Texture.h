#pragma once
class Texture{
public:
  Texture(int width, int height, GLenum format):
    m_width{width},m_height{height},m_format{format}
  {
    
  }
  int GetWidth() const {
		return m_width;
	}
	int GetHeight() const {
		return m_height;
	}
	GLenum GetImageFormat() const {
		return m_format;
	}
private:
  int m_width, m_height;
  GLenum m_format;
}
