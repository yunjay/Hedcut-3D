#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

using glm::vec3; using glm::vec4; using glm::vec2;
using glm::dot; using glm::cross; using glm::normalize; using glm::sign;
class HedcutRenderer {
public:
// I'd rather not get into coding a scene graph / scene class.
// I'll stick to one mesh at a time.
//	static void Render(GLuint fbo, Scene scene, Camera camera) {}
	static void Render(GLuint fbo, Mesh mesh, Camera camera){
		// Bind fbo
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	}
	static void Render(Mesh mesh, Camera camera) {
		// 0 : default framebuffer
		Render(0, scene, camera);
	}
	static void RenderToImage(Mesh mesh, Camera camera) {
		Framebuffer fboToTexture;
		Texture renderTargetTexture;
		Render(fboToTexture, mesh, camera);
	}
private:
static Framebuffer m_GbufferFramebuffer;
static Texture m_PositionTexture, m_NormalTexture, m_PrincipalDirectionTexture, m_AlbedoSpecular, m_DepthTexture;

};
