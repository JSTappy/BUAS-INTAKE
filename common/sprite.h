#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Sprite
{
public:
	Sprite(const std::string& imagepath);
	virtual ~Sprite();

	const std::string& GetTextureName() { return _textureName; };

	GLuint GetTexture() { return _texture; };
	GLuint GetVertexBuffer() { return _vertexbuffer; };
	GLuint GetUVBuffer() { return _uvbuffer; };

	unsigned int GetWidth() { return _width; };
	unsigned int GetHeight() { return _height; };

	glm::vec3 position;
	float rotation;
	glm::vec3 scale;

	void CreateFromBuffer();

	GLuint loadTGA(const std::string& imagepath);

	void SetUp(GLuint vb, GLuint uvb, GLuint texture, unsigned int width, unsigned int height);
private:

	GLuint _texture;
	GLuint _vertexbuffer;
	GLuint _uvbuffer;

	std::string _textureName;

	unsigned int _width;
	unsigned int _height;

	bool setup = false;
};

#endif /* SPRITE_H */
