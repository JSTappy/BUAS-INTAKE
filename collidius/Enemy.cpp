#include "Enemy.h" 
#include "TurnManager.h" 

Enemy::Enemy(unsigned char id, short health, unsigned char power, unsigned char defense, unsigned char speed, float damageReduction, unsigned char criticalChance)
	: GameEntity(id,health,power,defense,speed,damageReduction,criticalChance) // Call the GameEntity constructor
{
	_attackingTimer = new Timer();
	_attackingTimer->StopTimer();
	hitBox->scale = glm::vec3(2,2,0);
}

Enemy::~Enemy()
{

}

void Enemy::Update(float deltaTime)
{
	GameEntity::Update(deltaTime);
	hitBox->position = this->position;
	switch (gameEntityState)
	{
		case attacking:
			_starIndicator->rotation += 5 * deltaTime;
			if (_starIndicator->scale.x >= 0)
			{
				_starIndicator->scale -= 1.5f * deltaTime;
			}
			if (_target->GetID() == 1) 
			{
				_starIndicator->position -= glm::vec3(0, 128, 0) * deltaTime;
			}
			if (_waitingTimer->GetSeconds() <= 1 && _waitingTimer->isPlaying)return;
			if (!_attackPerformed)
			{
				_waitingTimer->StopTimer();
				ProjectileAttack(_target);
				_attackPerformed = true;
				return;
			}
			if (_projectiles.size() > 0)
			{
				for (size_t i = 0; i < _projectiles.size(); i++)
				{
					HandleProjectileCollision(_projectiles[i], _target);
				}
				return;
			}
			_waitingTimer->StopTimer();
			_target->gameEntityState = _target->idle;
			completedTurn = true;
			gameEntityState = idle;
			this->RemoveChild(_starIndicator);
			delete _starIndicator;
			_starIndicator = nullptr;
			return;
		case choosing:
			//for now default to attacking, healing might come into play later
			_waitingTimer->StartOverTimer();
			gameEntityState = attacking;
			_target = TurnManager::Instance()->GetRandomPlayer();
			_attackPerformed = false;
			_starIndicator = new MyEntity();
			_starIndicator->SetSprite("assets/sprites/starindicator.tga");
			this->AddChild(_starIndicator);
			_starIndicator->scale = glm::vec3(1.5f, 1.5f, 0);
			if (_target->GetID() == 1)
			{
				_starIndicator->position = this->position - glm::vec3(32,32,0);
				return;
			}
			_starIndicator->position = this->position - glm::vec3(-4, 32, 0);
			return;
		case defending:
			//for now do nothing
			return;
	}
}

void Enemy::HandleAction(){}

void Enemy::UpdateHealthText()
{
	this->text->text = "Enemy: " + std::to_string(this->GetID()) + " HP: " + std::to_string(health) + " / " + std::to_string(GetMaxHealth());
}

void Enemy::PerformAttack(unsigned char attackLevel)
{
	BasicAttack(power, _target);
	completedTurn = true;
	this->gameEntityState = idle;
}

void Enemy::BasicAttack(char damage, GameEntity* target)
{
	_target = target;
	DealDamage(_target, 1);
}

void Enemy::ProjectileAttack(GameEntity* target)
{
	FireProjectile(target);
	_target->gameEntityState = _target->defending;
	TurnManager::Instance()->battleText->text = "WATCH OUT!";
}

void Enemy::FireProjectile(GameEntity* target)
{
	//make eye shine corresponding to the player position
	_target = target;
	bool randomBool = rand() % 2;
	Projectile* projectile = new Projectile(this->position, 700, _shouldDisplayHitboxes); //should jump
	projectile->scale = glm::vec3(2, 2, 0);
	this->AddChild(projectile);
	_projectiles.push_back(projectile);
	std::cout << "Projectile targetting: " << _target->GetID() << std::endl;
	projectile->SetSprite("assets/sprites/bananarang.tga");
	if (randomBool == 0)
	{
		projectile->SetTarget(_target->GetStartPos());
	}
	else
	{
		if (_target->GetID() == 1)
		{
			projectile->SetTarget(_target->GetStartPos() - glm::vec3(0, 172, 0)); return;
		}
		projectile->SetTarget(_target->GetStartPos() - glm::vec3(0, 140, 0));
	}
}

void Enemy::HandleProjectileAction()
{
	PerformAttack(1);
}
