#include "HedcutRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "Framebuffer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

using glm::vec3; using glm::vec4; using glm::vec2;
using glm::dot; using glm::cross; using glm::normalize; using glm::sign;

void HecutRenderer::Render(GLuint fbo, Mesh mesh, Camera camera){
		// Bind fbo
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    //TODO
	}
void HedcutRenderer::Render(Mesh mesh, Camera camera) {
		// 0 : default framebuffer
		HedcutRenderer::Render(0, scene, camera);
	}
void HedcutRenderer::RenderToImage(Mesh mesh, Camera camera) {
		Framebuffer fboToTexture;
		Texture renderTargetTexture;
		HedcutRenderer::Render(fboToTexture, mesh, camera);
    // TODO
	}
