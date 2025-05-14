#ifndef PROJECTILE_H 
#define PROJECTILE_H

#include "Entity.h"

class Projectile : public Entity
{
public:
	/// @brief constructor
	Projectile(glm::vec3 startPosition, float speed, bool displayHitbox);

	/// @brief destructor
	virtual ~Projectile();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	///@brief Set the target position this projectile needs to go towards
	///@param glm::vec3 targetPosition, the position of the target
	void SetTarget(glm::vec3 targetPosition);

	///@brief Set the target position this projectile needs to go towards
	float speed;

	///@brief Hitbox Sprite
	Entity* hitBox;
private:

	///@brief The position this entity has to reach
	glm::vec3 _endPosition;

	///@brief The position this entity starts from
	glm::vec3 _startPosition;

	///@brief The direction this entity should go
	glm::vec3 _direction;

};

#endif // PROJECTILE_H 