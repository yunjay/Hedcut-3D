#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec3;
class Camera {
public:

private:
	vec3 m_position;
	vec3 m_up;
	vec3 m_front;

};