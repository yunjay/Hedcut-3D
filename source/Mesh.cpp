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

// Debug tools
void printVec(glm::vec3 v) {
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ") ";
}
void printVec(glm::vec4 v) {
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ") ";
}

// Getters
vec3 Mesh::GetCenter(){ return m_center; }
float Mesh::GetDiagonalLength(){ return m_diagonalLength; }
float Mesh::GetModelScaleFactor(){ return m_modelScaleFactor; }
mat4 Mesh::GetModelToWorld(){ return m_modelToWorld; }
