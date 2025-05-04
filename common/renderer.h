#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "camera.h"
#include "sprite.h"
#include "entity.h"
#include "resourcemanager.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

	void RenderScene(Scene* scene);
	void RenderEntity(Entity* entity);
	void RenderSprite(Sprite* sprite, glm::mat4 modelMatrix);
	GLFWwindow* window() { return _window; };

	float updateDeltaTime();

private:
	int init();


	ResourceManager _resMan;

	GLFWwindow* _window;

	GLuint loadShaders(
		const std::string& vertex_file_path,
		const std::string& fragment_file_path
	);

	GLuint _programID;

	Camera* _camera; // reference to scene->camera

	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;

};

#endif /* RENDERER_H */
