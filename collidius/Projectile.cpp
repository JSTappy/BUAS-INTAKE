#include "Projectile.h" 

Projectile::Projectile(glm::vec3 startPosition, float newspeed, bool displayHitbox)
{
    this->position = startPosition;
    _startPosition = startPosition;
    speed = newspeed;

    _direction = glm::vec3(0);
    _endPosition = glm::vec3(0);

    //Set up the hitbox
    hitBox = new Entity();
    hitBox->position = this->position;
    hitBox->scale = glm::vec3(0.5f, 0.5f, 0.0f);
    this->AddChild(hitBox);

    //Set the hitbox sprite if it should be displayed
    if (displayHitbox) { hitBox->SetSprite("assets/sprites/hitbox.tga"); return; }
    hitBox->sprite = nullptr; 
}

Projectile::~Projectile()
{
    //Destroy the hitbox
    this->RemoveChild(hitBox);
    delete hitBox;
    hitBox = nullptr;
}

void Projectile::SetTarget(glm::vec3 targetPosition)
{
    _endPosition = targetPosition;
    _endPosition.y += 32; //Undershoot a tad bit to make it easier for the player to jump over
    _direction = glm::normalize(_endPosition - this->position); //Get the normalized vector towards the target
}

void Projectile::Update(float deltaTime)
{
    //Make sure the hitbox follows this position
    hitBox->position = this->position;
    this->position += _direction * speed * deltaTime; //Move towards the target
    this->rotation -= speed / 50 * deltaTime; //Rotate the sprite for extra visual effect

}


