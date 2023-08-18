#pragma once
class Framebuffer; class Camera; class Mesh; class Texture;
class HedcutRenderer {
public:
// I'd rather not get into coding a scene graph / scene class.
// I'll stick to one mesh at a time.
//	static void Render(GLuint fbo, Scene scene, Camera camera) {}
	static void Render(GLuint fbo, Mesh mesh, Camera camera);
	static void Render(Mesh mesh, Camera camera);
	static void RenderToImage(Mesh mesh, Camera camera);
private:
static Framebuffer m_GbufferFramebuffer;
static Texture m_PositionTexture, m_NormalTexture, m_PrincipalDirectionTexture, m_AlbedoSpecular, m_DepthTexture;

};
