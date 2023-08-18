#include <memory>
#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <unordered_map>
#include <functional>

#include <omp.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>      
#include <assimp/scene.h>          
#include <assimp/postprocess.h>     

using glm::vec2; using glm::vec3; using glm::vec4; 
using glm::vec3; using glm::mat4; 
using std::cout; using std::vector; using std::array;

// Debug tools
void printVec(glm::vec3 v) {
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
}
void printVec(glm::vec4 v) {
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ") ";
}

class Mesh {
public:
	struct Vertex {
		vec3 position, normal, maxPd, minPd;
		vec2 texCoords;
	};
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
	GLfloat m_diagonalLength = 0.0f;
	GLfloat m_modelScaleFactor = 1.0f;

	// Scene properties
	mat4 m_modelToWorld = mat4(1.0f);


	
};