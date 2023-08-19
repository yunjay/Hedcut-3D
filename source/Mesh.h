#pragma once
#include <memory>
#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>

#include <omp.h>

#include <GL/glew.h>
#include <glm/glm.hpp>  

using glm::vec2; using glm::vec3; using glm::vec4; 
using glm::vec3; using glm::mat4; 
using std::cout; using std::vector; using std::array;

class Mesh {
public:
	struct Vertex {
		vec3 position, normal, maxPd, minPd;
		vec2 texCoords;
	};
	// Setters
	mat4 SetModelToWorld();
	// Getters
	vec3 GetCenter() const;
	float GetDiagonalLength() const;
	float GetModelScaleFactor() const;
	mat4 GetModelToWorld() const;
	void AllocateGPUMemory(); // allocate initially or update gpu memory
	
private:
	// Should the Renderer both allocate and bind, or only bind...?
	// I think it's fine to allocate within the renderable (mesh) but bind in the renderer.
	GLuint m_VAO;
	GLuint m_EBO;
	GLuint m_VBO; //Pack everything together for cache hits
	// GLuint SSBO;
	vector<vec3> m_vertices, m_normals, m_maxPds, m_minPds;
	vector<vec2> m_textureCoordinates;
	vector<array <unsigned int, 3>> m_faces;
	vector<GLuint> m_indices;
	
	// Mesh properties
	vec3 m_center = vec3(0.0f);
	float m_diagonalLength = 0.0f;
	float m_modelScaleFactor = 1.0f;

	// Scene properties
	mat4 m_modelToWorld = mat4(1.0f);


	
};
