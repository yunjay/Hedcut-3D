#include "Framebuffer.h"
#include<GL/glew.h>
#include<map>
#include<memory>
#include<vector>


Framebuffer::Framebuffer(int width, int height) :
	m_width{ width }, m_height{ height } {
	glGenFramebuffers(1, &m_handle);
}
Framebuffer::Framebuffer(int width, int height, int sampleCount) :
	m_width{ width }, m_height{ height }, m_sampleCount{ sampleCount } {
	glGenFramebuffers(1, &m_handle);
}
Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &m_handle);
	m_attachedTextures.clear();
}

int Framebuffer::getNumColorAttachments() {
	int numColorAttachments = 0;
	for (auto attachedTexture : m_attachedTextures) {
		if (attachedTexture.first == ColorAttachment) numColorAttachments++;
	}
	return numColorAttachments;
}

void Framebuffer::AttachTexture(spTexture texture, AttachmentType attachmentType) {

}

void Framebuffer::BindDefaultFramebuffer(int width, int height) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	return;
}