#include "Cube.h"
#include <vector>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Cube::Cube()
{
	model = glm::mat4(1.0f);
}

Cube::~Cube()
{
}

std::vector<unsigned int> indices =
{
	/*front*/
	0, 1, 2,
	2, 3, 0,
	/*top*/
	1, 5, 6,
	6, 2, 1,
	/*back*/
	7, 6, 5,
	5, 4, 7,
	/*bottom*/
	4, 0, 3,
	3, 7, 4,
	/*left*/
	4, 5, 1,
	1, 0, 4,
	/*right*/
	3, 2, 6,
	6, 7, 3
};

std::vector<float> vertices = 
{
	/* front */
	-1.0, -1.0,  1.0,
	1.0, -1.0,  1.0,
	1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	/* back */
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0,
};

void Cube::Init()
{
	//create shader
	shader = new ShaderProgram();
	shader->CreateShader(GL_VERTEX_SHADER, "Shaders/cubeVert.glsl");
	shader->CreateShader(GL_FRAGMENT_SHADER, "Shaders/cubeFrag.glsl");
	shader->Use();
	//create mesh
	mesh = new Mesh();
	mesh->AddIndices(indices);
	mesh->AddVerticesAttribute(0, vertices, 3);
}

void Cube::Draw()
{
	shader->Use();
	mesh->DrawMesh(GL_TRIANGLES);
}

void Cube::SetPosition(glm::vec3 pos)
{
	shader->Use();
	model = glm::translate(glm::mat4(1.0), pos);
	model = glm::scale(model, glm::vec3(0.25f));
	int modelLoc = glGetUniformLocation(shader->ShaderProgramID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Cube::SetViewProjection(Camera* cam)
{
	shader->Use();
	cam->SetMatrices(shader);
}
