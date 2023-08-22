#pragma once
#include"Texture.h"


class Framebuffer {
public:
	enum AttachmentType {
		ColorAttachment,
		DepthAttachment,
		DepthStencilAttachment
	};
	
	Framebuffer(int width, int height);
	Framebuffer(int width, int height, int sampleCount);
	~Framebuffer();
	// This both binds the fbo and sets draw buffers.
	// Calls glBindFramebuffer, glDrawBuffers
	void Bind();
	// Attach texture to framebuffer
	// Calls glFramebufferTexture2D
	void AttachTexture(spTexture texture, AttachmentType attachmentType);

	int getNumColorAttachments();
	// Getters
	int GetHeight() {
		return m_height;
	}
	int GetWidth() {
		return m_width;
	}

	// static functions
	static void BindDefaultFramebuffer(int width, int height);
private:
	GLuint m_handle;
	int m_width, m_height;
	int m_sampleCount;
	// Renderbuffers aren't necessary are they?
	std::map <AttachmentType, spTexture> m_attachedTextures;
};
