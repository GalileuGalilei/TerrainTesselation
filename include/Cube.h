#pragma once
#include "Mesh.h"
#include "ShadersPro.h"
#include "Camera.h"
#include "glm/glm.hpp"

//basic rendering cube
class Cube
{
private:
	Mesh* mesh;
	ShaderProgram* shader;
	glm::mat4 model;

public:
	Cube();
	~Cube();
	void Init();
	void Draw();
	void SetPosition(glm::vec3 pos);
	void SetViewProjection(Camera* cam);
};

