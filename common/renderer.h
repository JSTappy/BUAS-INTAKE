#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "camera.h"
#include "sprite.h"
#include "entity.h"
#include "resourcemanager.h"
#include "fontmanager.h"
#include "text.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

	void RenderScene(Scene* scene);
	void RenderEntity(Entity* entity);
	void RenderSprite(Sprite* sprite, glm::mat4 modelMatrix);
	void RenderText(Text* text);
	GLFWwindow* window() { return _window; };

	float UpdateDeltaTime();

private:
	int init();

	GLuint ChooseShader(GLuint shaderID);

	ResourceManager _resMan;
	FontManager _fontMan;

	GLFWwindow* _window;

	GLuint LoadShaders(
		const std::string& vertex_file_path,
		const std::string& fragment_file_path
	);

	GLuint _programID;
	GLuint _textShaderID;
	GLuint _activeID;

	Camera* _camera; // reference to scene->camera

	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;

};

#endif /* RENDERER_H */
