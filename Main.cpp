#include "glad/glad.h" 
#include "GLFW/glfw3.h"
#include "iostream"
#include "ShadersPro.h"
#include "Mesh.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "Cube.h"

int window_width = 720;
int window_heigh = 720;
int forcedQuadTessLevel[9] = { 0,0,0,0,0,0,0,0,0 };
bool useNormalMap = false;
Camera cam(window_width, window_heigh);
GLFWwindow* window;

#pragma region vertices_vectors

std::vector<float> squarePosition =
{          
	//quadrado que chegue at[e as bordas da tela
	-1.0f, 0.0f, 1.0f, 	//0
	1.0f, 0.0f, 1.0f,	//1
	1.0f, 0.0f, -1.0f,	//3
	-1.0f, 0.0f, -1.0f	//2
};

std::vector<float> squareTexture =
{
	0.0f, 1.0f,	//0
	1.0f, 1.0f,	//3
	1.0f, 0.0f,	//2
	0.0f, 0.0f	//1
};

std::vector<GLuint> SquareIndice =
{
	0,1,3,2
};

#pragma endregion

#pragma region Callbacks

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, source = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, source, message);
}

void OnWindowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	cam.OnWindowResize(width, height);
}

void OnMouseInput(GLFWwindow* window, double xpos, double ypos)
{
	cam.OnMouseInput(xpos, ypos);
}

void OnKeyInput(GLFWwindow* window, int, int, int, int)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		useNormalMap = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		useNormalMap = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		useNormalMap = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void numpadCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		// Check if the pressed key is in the numpad range
		if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_9) {
			int numpadNumber = key - GLFW_KEY_KP_0;
			
			if (numpadNumber < 1)
			{
				return;
			}

			if (mods == GLFW_MOD_ALT) 
			{
				forcedQuadTessLevel[numpadNumber - 1] -= 1;
			}
			else 
			{
				forcedQuadTessLevel[numpadNumber - 1] += 1;
			}
		}
	}
}

#pragma endregion

void InitGladAndGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(window_width, window_heigh, "JANELA", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, OnMouseInput);
	glfwSetKeyCallback(window, [] (GLFWwindow * window, int key, int scancode, int action, int mods) -> void
	{
		OnKeyInput(window, key, scancode, action, mods);
		numpadCallback(window, key, scancode, action, mods);
	});

	glfwSetFramebufferSizeCallback(window, OnWindowResize);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
}

void InitOpenGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	//errors
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);

	//Viewport
	glViewport(0, 0, window_width, window_heigh);
}

int main()
{
	InitGladAndGLFW();
	InitOpenGL();		

	ShaderProgram SHADER;
	
	SHADER.CreateShader(GL_VERTEX_SHADER, "shaders/VertexShader.glsl");
	SHADER.CreateShader(GL_TESS_CONTROL_SHADER, "shaders/TessControlShader.glsl");
	SHADER.CreateShader(GL_TESS_EVALUATION_SHADER, "shaders/TessEvaluationShader.glsl");
	SHADER.CreateShader(GL_GEOMETRY_SHADER, "shaders/GeometryShader.glsl");
	SHADER.CreateShader(GL_FRAGMENT_SHADER, "shaders/FragmentShader.glsl");
	SHADER.Use();

	//textures
	Texture* tex = new Texture("resources/heightmap.jpg", true, GL_RGBA32F);
	Texture* tex2 = new Texture("resources/NormalMap.jpg", true, GL_RGBA32F);
	SHADER.SetTexture(tex, "heightMap");
	SHADER.SetTexture(tex2, "normalMap");

	//vertices
	Mesh* mesh = new Mesh();
	mesh->AddVerticesAttribute(0, squarePosition, 3);
	mesh->AddVerticesAttribute(1, squareTexture, 2);
	mesh->AddIndices(SquareIndice);
		
	//transformations
	glm::mat4 model_matrix(1.0f);
	int model_location = glGetUniformLocation(SHADER.ShaderProgramID, "model");
	int id_loc = glGetUniformLocation(SHADER.ShaderProgramID, "id");
	int tess_block_loc = glGetUniformLocation(SHADER.ShaderProgramID, "tessLevel");
	int light_loc1 = glGetUniformLocation(SHADER.ShaderProgramID, "worldLightPos1");
	int light_loc2 = glGetUniformLocation(SHADER.ShaderProgramID, "worldLightPos2");
	int useNormalMap_loc = glGetUniformLocation(SHADER.ShaderProgramID, "useNormalMap");

	float* tessLevel = new float[9];
	float timer = 0.0f;

	//lights
	Cube* light1 = new Cube();
	light1->Init();
	Cube* light2 = new Cube();
	light2->Init();
	

	//Update loop
	while (!glfwWindowShouldClose(window))
	{
		//background_color
		glClearColor(0.75f, 0.23f, 0.46f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		//binding all
		SHADER.ActivateTexture(tex);
		SHADER.ActivateTexture(tex2);
		SHADER.Use();

		//camera inputs
		cam.OnKeyInput(window);
		cam.SetMatrices(&SHADER);

		//uniforms
		int id = 0;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++, id++)
			{
				glm::vec3 meshPos = glm::vec3(x * 4.0f, 0.0f, y * 4.0f);
				tessLevel[id] = cam.CalculateTesselationLevel(meshPos) + forcedQuadTessLevel[id];
			}
		}

		glUniform1fv(tess_block_loc, 9, tessLevel);
		glUniform1i(useNormalMap_loc, useNormalMap);

		id = 0;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++, id++)
			{
				glm::vec3 meshPos = glm::vec3(x * 4.0f, 0.0f, y * 4.0f);
				model_matrix = glm::translate(glm::mat4(1.0f), meshPos);
				model_matrix *= glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
				

				glUniformMatrix4fv(model_location, 1, false, glm::value_ptr(model_matrix));
				glUniform1i(id_loc, id);
				mesh->DrawMesh(GL_PATCHES);
			}
		}

		//lights
		glUniform3f(light_loc1, std::cos(timer) * 8, 6, std::sin(timer) * 8);
		glUniform3f(light_loc2, std::cos(timer + 1.6) * 8, 6, std::sin(timer + 1.6) * 8);
		
		light1->SetPosition(glm::vec3(std::sin(timer) * 8, 6, std::sin(timer) * 8));
		light1->SetViewProjection(&cam);
		light2->SetPosition(glm::vec3(std::cos(timer + 1.6) * 8, 6, std::sin(timer + 2.6) * 8));
		light2->SetViewProjection(&cam);
		
		timer += 0.01;

		light1->Draw();
		light2->Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	mesh->DeleteMesh();
	SHADER.Delete();
	glfwTerminate();

	return 0;
}




