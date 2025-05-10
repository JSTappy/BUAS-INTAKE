#ifndef PROJECTILE_H 
#define PROJECTILE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MyEntity.h"

class Projectile : public MyEntity
{
public:
	/// @brief constructor
	Projectile(glm::vec3 startPosition, float speed, bool displayHitbox);

	/// @brief destructor
	virtual ~Projectile();

	void SetTarget(glm::vec3 targetPosition);
	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	float speed;
	MyEntity* hitBox;
private:

	glm::vec3 _endPosition;
	glm::vec3 _startPosition;
	glm::vec3 _direction;

};

#endif // PROJECTILE_H 