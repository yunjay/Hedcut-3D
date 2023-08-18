#pragma once
#include<GL/glew.h>
#include<map>
#include<memory>
#include<vector>

#include"Texture.h"

typedef std::shared_ptr<Texture> spTexture;

class Framebuffer {
public:
	enum AttachmentType {
		ColorAttachment,
		DepthAttachment,
		DepthStencilAttachment
	};
	
	Framebuffer(int width, int height) :
		m_width{ width }, m_height{ height } {
		glGenFramebuffers(1, &m_handle);
	}
	Framebuffer(int width, int height, int sampleCount) :
		m_width{ width }, m_height{ height }, m_sampleCount{ sampleCount } {
		glGenFramebuffers(1, &m_handle);
	}
	~Framebuffer() {
		glDeleteFramebuffers(1, &m_handle);
	}
	// This both binds the fbo and sets draw buffers.
	// Calls glBindFramebuffer, glDrawBuffers
	void Bind() {

		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
		glViewport(0, 0, m_width, m_height);

		std::vector<GLenum> attachments;
		for (auto attachedTexture : m_attachedTextures)
		{
			AttachmentType attachmentType = attachedTexture.first;
			if (attachmentType==ColorAttachment)
			{
				attachments.push_back(attachedTexture.second->GetHandle());
			}
		}
		glDrawBuffers(static_cast<int>(attachments.size()), attachments.data());
	}
	// Attach texture to framebuffer
	// Calls 
	void AttachTexture(spTexture texture, AttachmentType attachmentType) {
		// As glFramebufferTexture takes a target, GL_DRAW_FRAMEBUFFER is safer as it is less used outside of this function. (Kind of an unnecessary detail)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);

		if (attachmentType == ColorAttachment)
			// GL_COLOR_ATTACHMENT0 + num of attached color attachment textures
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + getNumColorAttachments(), GL_TEXTURE_2D, texture->GetHandle(), 0);
		else if (attachmentType == DepthAttachment)
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->GetHandle(), 0);
		else if (attachmentType == DepthStencilAttachment)
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture->GetHandle(), 0);
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		// Add texture
		m_attachedTextures[attachmentType] = texture;

		return;
	}






	int getNumColorAttachments() {
		int numColorAttachments = 0;
		for (auto attachedTexture : m_attachedTextures) {
			if (attachedTexture.first == ColorAttachment) numColorAttachments++;
		}
		return numColorAttachments;
	}

	static void BindDefaultFramebuffer(int width, int height) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,width, height);
	}
	// Getters
	int GetHeight() {
		return m_height;
	}
	int GetWidth() {
		return m_width;
	}
private:
	GLuint m_handle;
	int m_width, m_height;
	int m_sampleCount;
	// Renderbuffers aren't necessary are they?
	std::map <AttachmentType, spTexture> m_attachedTextures;
};
