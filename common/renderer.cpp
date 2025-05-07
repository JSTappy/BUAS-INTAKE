#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "config.h"
#include "camera.h"
#include "renderer.h"


Renderer::Renderer()
{
    //get this in the scene later
    _camera = nullptr;

    _viewMatrix = glm::mat4(1.0f);
	_projectionMatrix = glm::mat4(1.0f);

	// Create window with OpenGL context
	this->init();
}

Renderer::~Renderer()
{
	// Cleanup VBO and shader
	glDeleteProgram(_programID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

int Renderer::init()
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	// Set OpenGL version (2.1)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Open a window and create its OpenGL context
	_window = glfwCreateWindow( WIDTH, HEIGHT, "Demo", NULL, NULL);
	if( _window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(_window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	_programID = this->LoadShaders("assets/shaders/sprite.vert" , "assets/shaders/sprite.frag");
	_textShaderID = this->LoadShaders("assets/shaders/text.vert", "assets/shaders/text.frag");

	// Use our shader
	glUseProgram(_programID);

	return 0;
}

float Renderer::UpdateDeltaTime() {
	// lastTime is initialised only the first time this function is called
	static double lastTime = glfwGetTime();
	// get the current time
	double currentTime = glfwGetTime();

	// Compute time difference between current and last time
	float deltaTime = float(currentTime - lastTime);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
	return deltaTime;
}

void Renderer::RenderScene(Scene* scene)
{
	// Set glViewport to the width and height of the window
	glViewport(0, 0, scene->GetInput()->GetWindowWidth(), scene->GetInput()->GetWindowHeight());

	// get camera from scene and update
	_camera = scene->camera();

	_viewMatrix = _camera->GetViewMatrix();
	_projectionMatrix = _camera->GetProjectionMatrix();

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render all the entities
	for (Entity* entity : scene->GetAllChildren())
	{
		this->RenderEntity(entity);
	}

	// Swap buffers
	glfwSwapBuffers(this->GetWindow());
	glfwPollEvents();
}

void Renderer::RenderEntity(Entity* entity)
{
	// Build the Model matrix from Sprite transform
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(entity->position.x, entity->position.y, 0.0f));
	glm::mat4 rotationMatrix = glm::eulerAngleYXZ(0.0f, 0.0f, entity->rotation);
	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(entity->scale.x, entity->scale.y, 1.0f));

	glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;

	// Build MVP matrix
	glm::mat4 MVP = _projectionMatrix * _viewMatrix * modelMatrix;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	GLuint matrixID = glGetUniformLocation(_programID, "MVP");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, 2 * 3); // 2*3 indices starting at 0 -> 2 triangles
	if (entity->sprite != nullptr)
	{
		this->RenderSprite(entity->sprite, MVP);
	}
	if (entity->text != nullptr)
	{
		this->RenderText(entity->text);
	}
}

void Renderer::RenderSprite(Sprite* sprite, glm::mat4 MVP)
{
	this->ChooseShader(_programID);
	// Send our transformation to the currently bound shader, in the "MVP" uniform
	GLuint matrixID = glGetUniformLocation(_programID, "MVP");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

	Sprite* s = _resMan.GetTarga(sprite->GetTextureName());
	sprite->SetUp(s->GetVertexBuffer(), s->GetUVBuffer(), s->GetTexture(), s->GetWidth(), s->GetHeight());
	s = nullptr;

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->GetTexture());
	// Set our "textureSampler" sampler to use Texture Unit 0
	GLuint textureID  = glGetUniformLocation(_programID, "textureSampler");
	glUniform1i(textureID, 0);

	// 1st attribute buffer : vertices
	GLuint vertexPositionID = glGetAttribLocation(_programID, "vertexPosition");
	glEnableVertexAttribArray(vertexPositionID);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->GetVertexBuffer());
	glVertexAttribPointer(
		vertexPositionID, // The attribute we want to configure
		3,          // size : x,y,z => 3
		GL_FLOAT,   // type
		GL_FALSE,   // normalized?
		0,          // stride
		(void*)0    // array buffer offset
	);

	// 2nd attribute buffer : UVs
	GLuint vertexUVID = glGetAttribLocation(_programID, "vertexUV");
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->GetUVBuffer());
	glVertexAttribPointer(
		vertexUVID, // The attribute we want to configure
		2,          // size : U,V => 2
		GL_FLOAT,   // type
		GL_FALSE,   // normalized?
		0,          // stride
		(void*)0    // array buffer offset
	);

	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, 2*3); // 2*3 indices starting at 0 -> 2 triangles

	// cleanup
	glDisableVertexAttribArray(vertexPositionID);
	glDisableVertexAttribArray(vertexUVID);
}

//this was mostly AI Generated
void Renderer::RenderText(Text* text)
{
	if (text->text == "")return;
	this->ChooseShader(_textShaderID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Get glyph map from FontManager
	std::map<char, glyph*> characters = _fontMan.GetFont(text->GetFontName(), text->GetSize());

	GLuint colorID = glGetUniformLocation(_textShaderID, "textColor");
	GLuint matrixID = glGetUniformLocation(_textShaderID, "projection");
	GLuint samplerID = glGetUniformLocation(_textShaderID, "text");
	glUniform1i(samplerID, 0); // Tells the shader to use GL_TEXTURE0

	// Create orthographic projection for 2D text overlay
	glm::mat4 proj = glm::ortho(0.0f, float(WIDTH), 0.0f, float(HEIGHT));
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &proj[0][0]);

	glUniform4f(colorID,
		text->color.r / 255.0f,
		text->color.g / 255.0f,
		text->color.b / 255.0f,
		text->color.a / 255.0f
	);

	float x = text->pivot.x;
	float y = text->pivot.y;

	for (const char& c : text->text)
	{
		glyph* ch = characters[c];

		float xpos = x + ch->bearing.x;
		float ypos = y - (ch->size.y - ch->bearing.y);

		float w = ch->size.x;
		float h = ch->size.y;

		// Vertices for character quad
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch->texture);

		// Create temporary VAO/VBO (or better: reuse a global one)
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		// Draw the quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);

		// Advance cursor for next glyph (advance is in 1/64 pixels)
		x += (ch->advance >> 6); // Bitshift by 6 to get pixels
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_BLEND);
}


GLuint Renderer::LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertex_file_path.c_str(), std::ios::in);
	if (vertexShaderStream.is_open()){
		std::string line = "";
		while (getline(vertexShaderStream, line)) {
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	} else {
		printf("Can't to open %s.\n", vertex_file_path.c_str());
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
	if (fragmentShaderStream.is_open()){
		std::string line = "";
		while (getline(fragmentShaderStream, line)) {
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	} else {
		printf("Can't to open %s.\n", fragment_file_path.c_str());
		return 0;
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path.c_str());
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path.c_str());
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

GLuint Renderer::ChooseShader(GLuint shaderID)
{
	if (_activeID == shaderID)
	{
		return _activeID;
	}
	glUseProgram(shaderID);
	_activeID = shaderID;
	return _activeID;
}
