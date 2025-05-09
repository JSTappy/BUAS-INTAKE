#include "Projectile.h" 

Projectile::Projectile(glm::vec3 targetPosition, glm::vec3 startPosition, bool displayHitbox)
{
    this->position = startPosition;
    _startPosition = startPosition;
    _endPosition = targetPosition;
    _endPosition.y += 32.0f;
	_speed = 750.0f;
    _direction = glm::normalize(_endPosition - this->position); //get the vector towards the target (player)

    hitBox = new MyEntity();
    hitBox->position = this->position;
    hitBox->scale = glm::vec3(0.5f, 0.5f, 0.0f);
    this->AddChild(hitBox);
    if (displayHitbox)
    { 
        hitBox->SetSprite("assets/sprites/hitbox.tga"); 
    }
    else 
    { 
        hitBox->sprite = nullptr; 
    }
}

Projectile::~Projectile()
{
	
}

void Projectile::Update(float deltaTime)
{
    hitBox->position = this->position;
    this->position += _direction * _speed * deltaTime; //move towards the target (player)
    this->rotation -= _speed / 50.0f * deltaTime; //rotate the star as visual

    //std::cout << "Pos: " << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;

}


