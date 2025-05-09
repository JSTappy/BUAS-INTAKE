#ifndef PROJECTILE_H 
#define PROJECTILE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MyEntity.h"

class Projectile : public MyEntity
{
public:
	/// @brief constructor
	Projectile(glm::vec3 targetPosition, glm::vec3 startPosition, bool displayHitbox);

	/// @brief destructor
	virtual ~Projectile();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	MyEntity* hitBox;
private:

	float _speed;
	glm::vec3 _endPosition;
	glm::vec3 _startPosition;
	glm::vec3 _direction;

};

#endif // PROJECTILE_H 