#include "Projectile.h" 

Projectile::Projectile(glm::vec3 startPosition, float newspeed, bool displayHitbox)
{
    this->position = startPosition;
    _startPosition = startPosition;
    speed = newspeed;

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

void Projectile::SetTarget(glm::vec3 targetPosition)
{
    _endPosition = targetPosition;
    _endPosition.y += 32;
    _direction = glm::normalize(_endPosition - this->position); //get the vector towards the target (player)
}

void Projectile::Update(float deltaTime)
{
    hitBox->position = this->position;
    this->position += _direction * speed * deltaTime; //move towards the target (player)
    this->rotation -= speed / 50 * deltaTime; //rotate the star as visual

    //std::cout << "Pos: " << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;

}


