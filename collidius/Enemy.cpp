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
			_target->gameEntityState = _target->idle;
			return;
		case choosing:
			//for now default to attacking, healing might come into play later
			ProjectileAttack(TurnManager::Instance()->GetRandomPlayer());
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
	FireProjectile(target, 1,_shouldDisplayHitboxes);
	gameEntityState = attacking;
	_target->gameEntityState = _target->defending;
	_attackingTimer->StartOverTimer();
	TurnManager::Instance()->battleText->text = "WATCH OUT!";
}

void Enemy::HandleProjectileAction()
{
	PerformAttack(1);
}
