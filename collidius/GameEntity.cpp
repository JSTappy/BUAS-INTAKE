#include "GameEntity.h"
#include "TurnManager.h"

GameEntity::GameEntity(int ID, int Level, int Health, int Power, int Defense, int Speed, float DamageReduction, int CriticalChance, int Money, int ExperiencePoints)
{
	_id = ID;
	_level = Level;
	health = Health;
	power = Power;
	defense = Defense;
	speed = Speed;
	_damageReduction = DamageReduction;
	_criticalChance = CriticalChance;
	_money = Money;
	_experiencePoints = ExperiencePoints;
	_maxHealth = health;
	alive = true;

	choosingAction = false;
	choosingCommand = false;
	choosingTarget = false;

	_movingTimer = new Timer();
	this->AddChild(_movingTimer);
	_movingTimer->StopTimer();

	hitBox = new MyEntity();
	this->AddChild(hitBox);

	_turnManager = TurnManager::Instance();


}


GameEntity::~GameEntity()
{

}

int GameEntity::CalculateAttackStat(int Power, int moveMultiplier)
{
	int totalDamageOutput = Power * moveMultiplier;
	return totalDamageOutput;
}

int GameEntity::CalculateDefenseStat(int Defense, int DamageReduction)
{
	int defenseOutput = Defense;
	return defenseOutput;
}

void GameEntity::DealDamage(GameEntity* target, int multiplier)
{
	int attackStat = CalculateAttackStat(this->power, multiplier);
	int defenseStat = CalculateDefenseStat(target->defense, 0);
	int hpBeforeAttack = 0;
	int hpAfterAttack = 0;

	hpBeforeAttack = target->health;
	if (defenseStat >= attackStat)
	{
		target->health -= 1;
	}
	else
	{
		//main calculation
		//target->health -= ((attackStat /* * CritIfHit */)-(attackStat * target->GetDamageReduction()) - defenseStat);
		target->health -= attackStat - defenseStat - ((attackStat - defenseStat) * target->GetDamageReduction());
	}

	hpAfterAttack = target->health;
	std::cout << "######################################################################### " << std::endl;
	std::cout << "Target HP Before Attack: " << hpBeforeAttack << " / " << target->_maxHealth << std::endl;
	std::cout << "Entity with ID: " << this->_id << " Attacked Entity with ID: " << target->GetID() << " !" << std::endl;
	std::cout << "Damage Dealt: " << hpAfterAttack - hpBeforeAttack << std::endl;
	std::cout << "Target HP After Attack: " << target->health << " / " << target->_maxHealth << std::endl;
	std::cout << "######################################################################### " << std::endl;

	_turnManager->battleText->text = "Damage Dealt: " + std::to_string(hpAfterAttack - hpBeforeAttack);
	_target->UpdateHealthText();


}

void GameEntity::HandleProjectileCollision(Projectile* p, GameEntity* t)
{
	//Divide right and bottom by 2 to get close to the mid point

	float projLeft = p->position.x - p->sprite->GetWidth() / 2;
	float projRight = p->position.x + p->sprite->GetWidth() / 2; 
	float projTop = p->position.y - p->sprite->GetHeight() / 2;
	float projBottom = p->position.y + p->sprite->GetHeight() / 2;

	float targetLeft = t->position.x - t->sprite->GetWidth() / 2;
	float targetRight = t->position.x + t->sprite->GetWidth() / 2;
	float targetTop = t->position.y - t->sprite->GetHeight() / 2;
	float targetBottom = t->position.y + t->sprite->GetHeight() / 2;

	if (projRight >= targetLeft &&
		projLeft <= targetRight &&
		projBottom >= targetTop &&
		projTop <= targetBottom)
	{
		DestroyProjectile(p);
		HandleProjectileAction();
		return;
	}
	// Check if projectile is outside the screen bounds
	if (p->position.x + 32.0f < 0 || // completely off left side
		p->position.x > GetInput()->GetWindowWidth() || // completely off right side
		p->position.y + 32.0f < 0 || // completely off top
		p->position.y > GetInput()->GetWindowHeight()) // completely off bottom
	{
		DestroyProjectile(p);
		return;
	}
}

void GameEntity::SetTextPosition(glm::vec3 textPosition)
{
	this->text->moveWithEntity = false;
	this->text->centered = false;
	this->text->position = textPosition;
}

void GameEntity::ToggleHitboxDisplay(bool displaying) 
{
	if (displaying)
	{
		this->hitBox->sprite = nullptr; //disable gameEntity hitboxes
		for (int i = 0; i < this->_projectiles.size(); i++) //disable projectile hitboxes
		{
			_projectiles[i]->hitBox->sprite = nullptr;
		}
		return;
	}
	for (int i = 0; i < this->_projectiles.size(); i++) //set projectile hitboxes active
	{
		if (_projectiles[i]->hitBox->sprite == nullptr)
		{
			_projectiles[i]->hitBox->SetSprite("assets/sprites/hitbox.tga");
			continue;
		}
	}
	if (this->hitBox->sprite == nullptr) //set gameEntity hitboxes active
	{
		this->hitBox->SetSprite("assets/sprites/hitbox.tga");
	}
}

void GameEntity::DestroyProjectile(Projectile* p)
{
	this->RemoveChild(p);
	_projectiles.erase(std::remove(_projectiles.begin(), _projectiles.end(), p),_projectiles.end());
	delete p;
	p = nullptr;

}

void GameEntity::TeleportToPosition(glm::vec3 position)
{
	this->position = position;
}

glm::vec3 GameEntity::ObtainNormalizedVector(glm::vec3 targetPosition)
{
	// Calculate the distance between positions
	glm::vec3 direction = targetPosition - this->position;

	// Normalize the direction vector
	glm::vec3 directionNormalized = glm::normalize(direction);

	return directionNormalized;
}

void GameEntity::MoveTowardsPosition(glm::vec3 targetPosition, float deltaTime)
{
	float distanceToTarget = glm::length(_initialTargetVector);

	// Calculate the movement for this frame
	float distanceThisFrame = 600.0f * deltaTime;

	// Apply movement
	this->position += _initialTargetVector * distanceThisFrame;
}

void GameEntity::SetStartPos()
{
	_startPos = this->position;
}