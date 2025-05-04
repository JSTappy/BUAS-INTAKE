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
	_startPos = glm::vec3(0, 0, 0);
	alive = true;

	choosingAction = false;
	choosingCommand = false;
	choosingTarget = false;


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
}

void GameEntity::HandleProjectileCollision(Projectile* p, GameEntity* t)
{
	//Divide right and bottom by 2 to get close to the mid point 
	float projLeft = p->position.x;
	float projRight = p->position.x + p->sprite->GetWidth() / 2; 
	float projTop = p->position.y;
	float projBottom = p->position.y + p->sprite->GetHeight() / 2;

	float targetLeft = t->position.x;
	float targetRight = t->position.x + t->sprite->GetWidth() / 2;
	float targetTop = t->position.y;
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


void GameEntity::HandleProjectileAction() {

}

void GameEntity::DestroyProjectile(Projectile* p)
{
	this->RemoveChild(p);
	_projectiles.erase(std::remove(_projectiles.begin(), _projectiles.end(), p),_projectiles.end());
	delete p;
	p = nullptr;

}

void GameEntity::Update(float deltaTime)
{

}

void GameEntity::ReturnToStartPos()
{
	this->position = _startPos;
}

void GameEntity::SetStartPos()
{
	_startPos = this->position;
}