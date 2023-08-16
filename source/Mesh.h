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
	GLuint VAO;
	GLuint EBO;
	GLuint VBO; //Pack everything together for cache hits
	// GLuint SSBO;
	vector<vec3> vertices, normals, maxPds, minPds;
	vector<vec2> textureCoordinates;
	vector<array <unsigned int, 3>> faces;
	vector<GLuint> indices;
	
	// Mesh properties
	vec3 center = vec3(0.0f);
	GLfloat diagonalLength = 0.0f;
	GLfloat modelScaleFactor = 1.0f;

	// Scene properties
	mat4 modelToWorld = mat4(1.0f);



};