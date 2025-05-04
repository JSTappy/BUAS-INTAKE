#ifndef MYENTITY_H
#define MYENTITY_H

#include "entity.h"

class MyEntity : public Entity
{
public:
	/// @brief Constructor
	MyEntity();

	/// @brief Destructor
	~MyEntity();

	/// @brief Changing Sprites
	/// @param imagepath the path to the image
	/// @return void
	void ChangeSprite(const std::string& imagepath);

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

private:
};
#endif // !MYENTITY_H