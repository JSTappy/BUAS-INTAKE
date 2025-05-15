#include "GameEntity.h"
#include "TurnManager.h"

GameEntity::GameEntity(unsigned char ID, short Health, unsigned char Power, unsigned char Defense, unsigned char Speed, float DamageReduction, unsigned char CriticalChance)
{
	//Assign the things in the constructor to your own values
	_id = ID;
	health = Health;
	power = Power;
	defense = Defense;
	speed = Speed;
	_damageReduction = DamageReduction;
	_criticalChance = CriticalChance;
	_maxHealth = health;

	alive = true;
	choosingAction = false;
	choosingCommand = false;

	//Make the moving timer
	_movingTimer = new Timer();
	this->AddChild(_movingTimer);
	_movingTimer->StopTimer();

	//Make the waiting timer
	_waitingTimer = new Timer();
	this->AddChild(_waitingTimer);
	_waitingTimer->StopTimer();

	//Make the damage sprite timer
	_damageSpriteTimer = new Timer();
	this->AddChild(_damageSpriteTimer);
	_damageSpriteTimer->StopTimer();

	//Make the hitbox
	hitBox = new Entity();
	this->AddChild(hitBox);

	//Assign make new items,each with 2 uses
	Item* itemA = new Item(0, 2);
	Item* itemB = new Item(1, 2);
	Item* itemC = new Item(2, 2);
	Item* itemD = new Item(3, 2);

	//Add the items to the list
	_items.push_back(itemA);
	_items.push_back(itemB);
	_items.push_back(itemC);
	_items.push_back(itemD);
}


GameEntity::~GameEntity()
{
	this->RemoveChild(_movingTimer);
	this->RemoveChild(_movingTimer);
	this->RemoveChild(_damageSpriteTimer);
	this->RemoveChild(_damageSprite);

	delete _damageSprite;
	delete _movingTimer;
	delete _waitingTimer;
	delete _damageSpriteTimer;

	_damageSprite = nullptr;
	_movingTimer = nullptr;
	_waitingTimer = nullptr;
	_damageSpriteTimer = nullptr;

	for (Item* item : _items)
	{
		delete item;
	}
	 _items.clear();

	for (Projectile* projectile : _projectiles)
	{
		delete projectile;
	}
	_projectiles.clear();


	 _target = nullptr;

	 delete _hitSound;
	 delete _critHitSound;
	 delete _projectileSound;

	 _hitSound = nullptr;
	 _critHitSound = nullptr;
	 _projectileSound = nullptr;
}

void GameEntity::Update(float deltaTime)
{
	//Update is only used for the damage sprite for now
	if (!_damageSpriteTimer->isPlaying) return; //Return if the damage timer is not playing
	if (_damageSpriteTimer->GetSeconds() > 1) //Stop the timer when the damage sprite is active for one second
	{
		//Stop the timer and remove the damage sprite
		_damageSpriteTimer->StopTimer();
		_damageSpriteTimer->isPlaying = false;
		this->RemoveChild(_damageSprite);
		delete _damageSprite;
		_damageSprite = nullptr;
	}
}

unsigned short GameEntity::CalculateAttackStat(unsigned char Power, float moveMultiplier)
{
	short totalDamageOutput = Power * moveMultiplier;
	if (totalDamageOutput >= 255)
	{
		totalDamageOutput = 255; //Cap damage at 255 because unsigned chars do not go higher
	}
	return totalDamageOutput; //Return the total damage
}

bool GameEntity::IsCriticalHit()
{
	float critChance = _criticalChance / 100.0f;
	float roll = static_cast<float>(rand()) / RAND_MAX;
	return roll < critChance;
}

void GameEntity::DealDamage(GameEntity* target, float multiplier)
{
	short attackStat = CalculateAttackStat(this->power, multiplier); //Get the attack stat
	short hpBeforeAttack = 0; //Short of the damage before the attack
	short hpAfterAttack = 0; //Short of the damage after the attack

	hpBeforeAttack = target->health; //Current health
	if (target->defense >= attackStat) //If the defense is higher or equal to the attack stat
	{
		target->health -= 1;
		isCriticalHit = false; //No crit

	}
	else if (attackStat >= 255) //If the attack stat is equal to or higher than 255
	{
		target->health -= 255;
		isCriticalHit = true; //Crit hit
	}
	else if (IsCriticalHit() && attackStat * 1.5f <= 255) //If the attack is a crit, and the total damage output is lower than 255
	{
		target->health -= attackStat * 1.5f - target->defense - (attackStat - target->defense) * target->GetDamageReduction(); //Apply crit damage
		isCriticalHit = true;
	}
	else
	{
		//Main calculation
		target->health -= attackStat - target->defense - (attackStat - target->defense) * target->GetDamageReduction(); //Apply normal damage
		isCriticalHit = false;
	}

	hpAfterAttack = target->health; //Health after taking the hit
	std::cout << "######################################################################### " << std::endl;
	std::cout << "Target HP Before Attack: " << hpBeforeAttack << " / " << target->_maxHealth << std::endl;
	std::cout << "Target HP After Attack: " << target->health << " / " << target->_maxHealth << std::endl;
	std::cout << "######################################################################### " << std::endl;
	ShowDamage(_target, hpBeforeAttack - hpAfterAttack); //Make the sprite and show the damage
	_target->UpdateHealthText(); //Update the targets health text
}

void GameEntity::HandleProjectileCollision(Projectile* p, GameEntity* t)
{
	//Calculate half-width and half-height of the projectile sprite
	float projHalfWidth = (p->sprite->GetWidth() * p->scale.x) / 2;
	float projHalfHeight = (p->sprite->GetHeight() * p->scale.y) / 2;

	//Compute the projectile's AABB edges in world space
	float projLeft = p->position.x - projHalfWidth;
	float projRight = p->position.x + projHalfWidth;
	float projTop = p->position.y - projHalfHeight;
	float projBottom = p->position.y + projHalfHeight;

	//Calculate half-width and half-height of the target sprite
	float targetHalfWidth = (t->sprite->GetWidth() * t->scale.x) / 2;
	float targetHalfHeight = (t->sprite->GetHeight() * t->scale.y) / 2;

	//Compute the targets AABB edges in world space
	float targetLeft = t->position.x - targetHalfWidth;
	float targetRight = t->position.x + targetHalfWidth;
	float targetTop = t->position.y - targetHalfHeight;
	float targetBottom = t->position.y + targetHalfHeight;

	//Perform AABB collision detection
	//If any part of the projectile's bounding box overlaps with the target's, it's a hit
	if (projRight >= targetLeft &&
		projLeft <= targetRight &&
		projBottom >= targetTop &&
		projTop <= targetBottom)
	{
		DestroyProjectile(p); //Destroy the projectile
		HandleProjectileAction(); //Execute the action of the projectile, this is declared in the player or enemy class
		return;
	}
	//Check if the projectile is out of bounds
	if (p->position.x + 32 < 0 || //Completely off left side
		p->position.x > GetInput()->GetWindowWidth() || //Completely off right side
		p->position.y + 32 < 0 || //Completely off top
		p->position.y > GetInput()->GetWindowHeight()) //Completely off bottom
	{
		DestroyProjectile(p); //Destroy the projectile
	}
}

void GameEntity::ToggleHitboxDisplay(bool displaying) 
{

	_shouldDisplayHitboxes = displaying;
	if (_shouldDisplayHitboxes) //If the hitboxes should be displayed
	{
		for (size_t i = 0; i < this->_projectiles.size(); i++) //Set projectile hitboxes active
		{
			if (_projectiles[i]->hitBox->sprite == nullptr) //If the projectile hitbox sprite is not a sprite
			{
				_projectiles[i]->hitBox->SetSprite("assets/sprites/hitbox.tga");
				continue;
			}
		}
		if (this->hitBox->sprite == nullptr) //If this entity hitbox sprite is not a sprite
		{
			this->hitBox->SetSprite("assets/sprites/hitbox.tga");
		}
		return;
	}
	this->hitBox->sprite = nullptr; //Disable gameEntity hitboxes
	for (size_t i = 0; i < this->_projectiles.size(); i++) //Disable projectile hitboxes
	{
		_projectiles[i]->hitBox->sprite = nullptr;
	}
}

void GameEntity::DestroyProjectile(Projectile* p)
{
	//Remove the child, erase the projectile from the list, delete the projectile and dereference the projectile
	this->RemoveChild(p);
	_projectiles.erase(std::remove(_projectiles.begin(), _projectiles.end(), p),_projectiles.end());
	delete p;
	p = nullptr;
}

void GameEntity::TeleportToPosition(glm::vec3 position)
{
	//This position is the new position
	this->position = position;
}

glm::vec3 GameEntity::ObtainNormalizedVector(glm::vec3 targetPosition)
{
	//Calculate the distance between positions
	glm::vec3 direction = targetPosition - this->position;

	//Normalize the direction vector
	glm::vec3 directionNormalized = glm::normalize(direction);

	return directionNormalized;
}

void GameEntity::MoveTowardsPosition(glm::vec3 targetPosition, unsigned short movingSpeed, float deltaTime)
{
	//Get the length to the initial vectpr
	float distanceToTarget = glm::length(targetPosition);

	//Calculate the movement for this frame
	float distanceThisFrame = movingSpeed * deltaTime;

	//Apply movement
	this->position += targetPosition * distanceThisFrame;
}

void GameEntity::ShowDamage(GameEntity* target, unsigned char damageTaken)
{
	//If the damage sprite is already active
	if (_damageSprite != nullptr) 
	{ 
		//Start over the timer so the entity does not get deleted too soon
		_damageSpriteTimer->StartOverTimer();
		_damageSprite->text->text = std::to_string(damageTaken); 
		if (isCriticalHit)//If its a critical hit, change the sprite to the crit sprite and replay the crit hit sound
		{
			_damageSprite->SetSprite("assets/sprites/critimpact.tga");
			_critHitSound->replay();
			return;
		}

		//Set the sprite to the normal impact sprite and replay the normal hit sound
		_damageSprite->SetSprite("assets/sprites/impact.tga");
		_hitSound->replay();
		return;
	}
	_damageSprite = new Entity();
	this->AddChild(_damageSprite);

	_damageSprite->position = target->GetStartPos() - glm::vec3(-128, 128, 0); //Set the position to the top right of the target sprite
	_damageSprite->text = new Text((char*)"assets/fonts/impact.ttf", 64);

	//Add the correct text to the sprite
	_damageSprite->textComponents.push_back(_damageSprite->text);
	_damageSprite->text->text = std::to_string(damageTaken);
	_damageSprite->SetTextPosition(_damageSprite->text, _damageSprite->position);
	_damageSprite->text->centered = true;

	//Start the damage timer
	_damageSpriteTimer->StartTimer();
	_damageSpriteTimer->isPlaying = true;

	//If its a critical hit, play the crit sound and set the crit sprite
	if (isCriticalHit)
	{
		_damageSprite->SetSprite("assets/sprites/critimpact.tga");
		_critHitSound->play();
		return;
	}

	//If its a normal hit, play the hit sound and set the impact sprite
	_damageSprite->SetSprite("assets/sprites/impact.tga");
	_hitSound->play();
}

void GameEntity::UseItem(unsigned char index)
{
	//If the item is out of uses, you cant use it, do nothing
	if (_items[index]->GetUses() == 0)
	{
		TurnManager::Instance()->battleText->text = "You do not have this item anymore! Pick something else";
		return;
	}

	//Use the item
	_items[index]->ApplyItem(this);
}

void GameEntity::FireProjectile(GameEntity* target)
{
	
	_projectileSound->play(); //Play the projectile sound
	_target = target; //Assign the target

	//Create the projectile
	Projectile* projectile = new Projectile(this->position, 800.0f, _shouldDisplayHitboxes);
	_projectiles.push_back(projectile);
	projectile->SetTarget(_target->GetStartPos());
	this->AddChild(projectile);
	projectile->SetSprite("assets/sprites/kblast.tga");
	_projectiles.push_back(projectile);
}

void GameEntity::SetStartPos()
{
	//This gets called at the beginning of the game, so the start position is the current position
	_startPos = this->position;
}