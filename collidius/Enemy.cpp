#include "Enemy.h" 
#include "TurnManager.h" 

Enemy::Enemy(int id, int level, int health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints)
	: GameEntity(id, level, health, power, defense, speed, damageReduction, criticalChance, money, experiencePoints) // Call the GameEntity constructor
{
	_attackingTimer = new Timer();
	_attackingTimer->StopTimer();
}

Enemy::~Enemy()
{

}

void Enemy::Update(float deltaTime)
{
	switch (gameEntityState)
	{
		case attacking:
			if (_projectiles.size() != 0)
			{
				for (int i = 0; i < _projectiles.size(); i++)
				{
					HandleProjectileCollision(_projectiles[i], _target);
				}
				return;
			}
			_attackingTimer->StopTimer();
			completedTurn = true;
			return;
		case choosing:
			//for now default to attacking, healing might come into play later
			ProjectileAttack(_turnManager->GetRandomPlayer());
			return;
		case defending:
			//for now do nothing
			return;
	}
}

void Enemy::HandleAction()
{

}

void Enemy::PerformAttack(int attackLevel)
{
	//decide random target between players later
	BasicAttack(power, _turnManager->GetRandomPlayer());
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
	_target = target;
	std::cout << "Projectile targetting: " << _target->GetID() << std::endl;
	std::cout << _target->GetStartPos().x << " " << _target->GetStartPos().y << " " << _target->GetStartPos().z << " " << std::endl;
	Projectile* projectile = new Projectile(_target->GetStartPos(), this->position, this->power);
	projectile->scale = glm::vec3(2.0f, 2.0f, 0.0f);
	this->AddChild(projectile);
	projectile->SetSprite("assets/sprites/bananarang.tga");
	_projectiles.push_back(projectile);
	gameEntityState = attacking;
	_target->gameEntityState = _target->defending;
	_attackingTimer->StartOverTimer();
}

void Enemy::HandleProjectileAction()
{
	PerformAttack(1);
}
