#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "camera.h"
#include "entity.h"

class Scene : public Entity
{
public:
	/// @brief constructor
	Scene(); 

	/// @brief destructor
	virtual ~Scene(); 

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime) = 0;

	/// @brief _camera getter
	/// @return _camera
	Camera* camera() { return _camera; }; 

	std::vector<Sprite*>& GetSprites() { return _sprites; };
	void AddSprites(); 
	void AddSprite(Sprite* sprite);

protected:
	std::vector<Sprite*> _sprites;

private:
	/// @brief private camera, the only camera that will be made
	Camera* _camera; 
};

#endif /* SCENE_H */
