#include "Enemy.h" 
#include "TurnManager.h" 

Enemy::Enemy(int id, int level, int health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints)
	: GameEntity(id, level, health, power, defense, speed, damageReduction, criticalChance, money, experiencePoints) // Call the GameEntity constructor
{
	_attackingTimer = new Timer();
	_attackingTimer->StopTimer();
	hitBox->scale = glm::vec3(2.0f,2.0f,0.0f);
}

Enemy::~Enemy()
{

}

void Enemy::Update(float deltaTime)
{
	hitBox->position = this->position;
	switch (gameEntityState)
	{
		case attacking:
			if (_waitingTimer->GetSeconds() <= 1.0f && _waitingTimer->isPlaying)return;
			if (!_shotsFired)
			{
				ProjectileAttack(TurnManager::Instance()->GetRandomPlayer());
				_waitingTimer->StopTimer();
				_shotsFired = true;
				return;
			}
			if (_projectiles.size() > 0)
			{
				for (int i = 0; i < _projectiles.size(); i++)
				{
					HandleProjectileCollision(_projectiles[i], _target);
				}
				return;
			}
			_waitingTimer->StopTimer();
			_target->gameEntityState = _target->idle;
			completedTurn = true;
			gameEntityState = idle;
			return;
		case choosing:
			//for now default to attacking, healing might come into play later
			_waitingTimer->StartOverTimer();
			gameEntityState = attacking;
			_shotsFired = false;
			return;
		case defending:
			//for now do nothing
			return;
	}
}

void Enemy::HandleAction(){}

void Enemy::UpdateHealthText()
{
	this->text->text = "Enemy: " + std::to_string(this->GetID()) + " HP: " + std::to_string(health) + " / " + std::to_string(_maxHealth);
}

void Enemy::PerformAttack(int attackLevel)
{
	BasicAttack(power, _target);
	completedTurn = true;
	this->gameEntityState = idle;
}

void Enemy::BasicAttack(int damage, GameEntity* target)
{
	_target = target;
	DealDamage(_target, 1);
}

void Enemy::ProjectileAttack(GameEntity* target)
{
	FireProjectile(target, 1,1);
	_target->gameEntityState = _target->defending;
	TurnManager::Instance()->battleText->text = "WATCH OUT!";
}

void Enemy::FireProjectile(GameEntity* target, int amount, float interval)
{
	//make eye shine corresponding to the player position
	_target = target;
	bool randomBool = rand() % 2;
	if (randomBool == 0)
	{
		Projectile* projectile = new Projectile(_target->GetStartPos(), this->position, _shouldDisplayHitboxes); //shouldnt jump
		projectile->scale = glm::vec3(2.0f, 2.0f, 0.0f);
		this->AddChild(projectile);
		_projectiles.push_back(projectile);
		std::cout << "Projectile targetting: " << _target->GetID() << std::endl;
		projectile->SetSprite("assets/sprites/bananarang.tga");
	}
	else
	{
		Projectile* projectile = new Projectile(_target->GetStartPos() - glm::vec3(0, 150, 0) , this->position, _shouldDisplayHitboxes); //shouldnt jump
		projectile->scale = glm::vec3(2.0f, 2.0f, 0.0f);
		this->AddChild(projectile);
		_projectiles.push_back(projectile);
		std::cout << "Projectile targetting: " << _target->GetID() << std::endl;
		projectile->SetSprite("assets/sprites/bananarang.tga");

	}
}

void Enemy::HandleProjectileAction()
{
	PerformAttack(1);
}
