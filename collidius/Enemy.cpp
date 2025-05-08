#include "Enemy.h" 
#include "TurnManager.h" 

Enemy::Enemy(int id, int level, int health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints)
	: GameEntity(id, level, health, power, defense, speed, damageReduction, criticalChance, money, experiencePoints) // Call the GameEntity constructor
{
	_attackingTimer = new Timer();
	_attackingTimer->StopTimer();
	this->text = new Text();
	this->text->text = "Enemy: " + std::to_string(id) + " HP: " + std::to_string(health) + " / " + std::to_string(_maxHealth);
	this->text->pivot = glm::vec2(0.0f, -128.0f);
	this->text->color = BLACK;
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
	_target = target;
	std::cout << "Projectile targetting: " << _target->GetID() << std::endl;
	std::cout << _target->GetStartPos().x << " " << _target->GetStartPos().y << " " << _target->GetStartPos().z << " " << std::endl;
	Projectile* projectile = new Projectile(_target->GetStartPos(), this->position, this->power);
	_projectiles.push_back(projectile);
	projectile->scale = glm::vec3(2.0f, 2.0f, 0.0f);
	this->AddChild(projectile);
	projectile->SetSprite("assets/sprites/bananarang.tga");
	_projectiles.push_back(projectile);
	gameEntityState = attacking;
	_target->gameEntityState = _target->defending;
	_attackingTimer->StartOverTimer();
	//_turnManager->battleText->text = "WATCH OUT!";
}

void Enemy::HandleProjectileAction()
{
	PerformAttack(1);
}
