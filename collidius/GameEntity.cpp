#include "GameEntity.h"
#include "TurnManager.h"

GameEntity::GameEntity(int ID, int Level, float Health, int Power, int Defense, int Speed, float DamageReduction, int CriticalChance, int Money, int ExperiencePoints)
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

	_waitingTimer = new Timer();
	this->AddChild(_waitingTimer);
	_waitingTimer->StopTimer();

	_damageSpriteTimer = new Timer();
	this->AddChild(_damageSpriteTimer);
	_damageSpriteTimer->StopTimer();

	hitBox = new MyEntity();
	this->AddChild(hitBox);

	Item* itemA = new Item(0, 2);
	Item* itemB = new Item(1, 2);
	Item* itemC = new Item(2, 2);
	Item* itemD = new Item(3, 2);

	_items.push_back(itemA);
	_items.push_back(itemB);
	_items.push_back(itemC);
	_items.push_back(itemD);
}


GameEntity::~GameEntity()
{

}

void GameEntity::Update(float deltaTime)
{
	if (!_damageSpriteTimer->isPlaying) return;
	if (_damageSpriteTimer->GetSeconds() > 1)
	{
		_damageSpriteTimer->StopTimer();
		_damageSpriteTimer->isPlaying = false;
		this->RemoveChild(_damageSprite);
		delete _damageSprite;
		_damageSprite = nullptr;
	}
}

double GameEntity::CalculateAttackStat(double Power, double moveMultiplier)
{
	double totalDamageOutput = Power * moveMultiplier;
	return totalDamageOutput;
}

double GameEntity::CalculateDefenseStat(double Defense, double DamageReduction)
{
	double defenseOutput = Defense;
	return defenseOutput;
}

bool GameEntity::IsCriticalHit(double critChancePercent)
{
	double critChance = critChancePercent / 100.0;
	double roll = static_cast<double>(rand()) / RAND_MAX;
	return roll < critChance;
}

void GameEntity::DealDamage(GameEntity* target, double multiplier)
{
	int attackStat = CalculateAttackStat(this->power, multiplier);
	int hpBeforeAttack = 0;
	int hpAfterAttack = 0;

	hpBeforeAttack = target->health;
	if (target->defense >= attackStat)
	{
		target->health -= 1;
	}
	else if (IsCriticalHit(this->_criticalChance))
	{
		target->health -= attackStat * 2 - target->defense - ((attackStat - target->defense) * target->GetDamageReduction());
		isCriticalHit = true;
	}
	else
	{
		//main calculation
		//target->health -= ((attackStat /* * CritIfHit */)-(attackStat * target->GetDamageReduction()) - defenseStat);
		target->health -= attackStat - target->defense - ((attackStat - target->defense) * target->GetDamageReduction());
	}

	hpAfterAttack = target->health;
	std::cout << "######################################################################### " << std::endl;
	std::cout << "Target HP Before Attack: " << hpBeforeAttack << " / " << target->_maxHealth << std::endl;
	std::cout << "Entity with ID: " << this->_id << " Attacked Entity with ID: " << target->GetID() << " !" << std::endl;
	std::cout << "Damage Dealt: " <<  hpBeforeAttack - hpAfterAttack << std::endl;
	std::cout << "Target HP After Attack: " << target->health << " / " << target->_maxHealth << std::endl;
	std::cout << "######################################################################### " << std::endl;
	ShowDamage(_target, hpBeforeAttack - hpAfterAttack);
	_target->UpdateHealthText();


}

void GameEntity::HandleProjectileCollision(Projectile* p, GameEntity* t)
{
	//handle AABB collission
	float projHalfWidth = (p->sprite->GetWidth() * p->scale.x) / 2.0f;
	float projHalfHeight = (p->sprite->GetHeight() * p->scale.y) / 2.0f;

	float projLeft = p->position.x - projHalfWidth;
	float projRight = p->position.x + projHalfWidth;
	float projTop = p->position.y - projHalfHeight;
	float projBottom = p->position.y + projHalfHeight;

	float targetHalfWidth = (t->sprite->GetWidth() * t->scale.x) / 2.0f;
	float targetHalfHeight = (t->sprite->GetHeight() * t->scale.y) / 2.0f;

	float targetLeft = t->position.x - targetHalfWidth;
	float targetRight = t->position.x + targetHalfWidth;
	float targetTop = t->position.y - targetHalfHeight;
	float targetBottom = t->position.y + targetHalfHeight;

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

void GameEntity::ToggleHitboxDisplay(bool displaying) 
{
	_shouldDisplayHitboxes = displaying;
	if (_shouldDisplayHitboxes)
	{
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
		return;
	}
	this->hitBox->sprite = nullptr; //disable gameEntity hitboxes
	for (int i = 0; i < this->_projectiles.size(); i++) //disable projectile hitboxes
	{
		_projectiles[i]->hitBox->sprite = nullptr;
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

void GameEntity::MoveTowardsPosition(glm::vec3 targetPosition, float movingSpeed, float deltaTime)
{
	float distanceToTarget = glm::length(_initialTargetVector);

	// Calculate the movement for this frame
	float distanceThisFrame = movingSpeed * deltaTime;

	// Apply movement
	this->position += _initialTargetVector * distanceThisFrame;
}

void GameEntity::ShowDamage(GameEntity* target, int damageTaken)
{
	if (_damageSprite != nullptr) 
	{ 
		_damageSpriteTimer->StartOverTimer();
		_damageSprite->text->text = std::to_string(damageTaken); 
		if (isCriticalHit)
		{
			_damageSprite->SetSprite("assets/sprites/critimpact.tga");
			return;
		}
		_damageSprite->SetSprite("assets/sprites/impact.tga");
		return;
	}
	_damageSprite = new MyEntity();
	this->AddChild(_damageSprite);
	_damageSprite->position = target->GetStartPos() - glm::vec3(-128, 128, 0);
	_damageSprite->text = new Text((char*)"assets/fonts/impact.ttf", 64);
	_damageSprite->textComponents.push_back(_damageSprite->text);
	_damageSprite->text->text = std::to_string(damageTaken);
	_damageSprite->SetTextPosition(_damageSprite->text, _damageSprite->position);
	_damageSprite->text->centered = true;
	_damageSpriteTimer->StartTimer();
	_damageSpriteTimer->isPlaying = true;
	if (isCriticalHit)
	{
		_damageSprite->SetSprite("assets/sprites/critimpact.tga");
		return;
	}
	_damageSprite->SetSprite("assets/sprites/impact.tga");
}

void GameEntity::UseItem(int index)
{
	if (_items[index]->GetUses() == 0)
	{
		TurnManager::Instance()->battleText->text = "You do not have this item anymore! Pick something else";
		return;
	}
	_items[index]->ApplyItem(this);
}

void GameEntity::FireProjectile(GameEntity* target, int amount, float interval)
{
	_target = target;
	std::cout << "Projectile targetting: " << _target->GetID() << std::endl;
	std::cout << _target->GetStartPos().x << " " << _target->GetStartPos().y << " " << _target->GetStartPos().z << " " << std::endl;
	Projectile* projectile = new Projectile(this->position, 800.0f, _shouldDisplayHitboxes);
	_projectiles.push_back(projectile);
	projectile->SetTarget(_target->GetStartPos());
	this->AddChild(projectile);
	projectile->SetSprite("assets/sprites/kblast.tga");
	_projectiles.push_back(projectile);
}

void GameEntity::SetStartPos()
{
	_startPos = this->position;
}