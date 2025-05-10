#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "entity.h"
#include "timer.h"

#include "Projectile.h"
#include "Item.h"


class GameEntity : public Entity
{
public:
	/// @brief Constructor
	GameEntity(int ID, int Level, float Health, int Power, int Defense, int Speed, float DamageReduction, int CriticalChance, int Money, int ExperiencePoints);

	/// @brief Destructor
	~GameEntity();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime) {}; //update

	virtual void TeleportToPosition(glm::vec3 position);
	virtual void MoveTowardsPosition(glm::vec3 position, float deltaTime);
	virtual void HandleProjectileAction() {};
	void HandleProjectileCollision(Projectile* p, GameEntity* t);
	void DestroyProjectile(Projectile* p);
	void SetStartPos();

	int CalculateAttackStat(int damage, int moveMultiplier);
	int CalculateDefenseStat(int Defense, int DamageReduction);
	void DealDamage(GameEntity* target, int multiplier);

	bool completedTurn;
	bool alive;

	bool choosingAction;
	bool choosingCommand;
	bool choosingTarget;

	int health;
	int power;
	int defense;
	int speed;

	int GetID() { return _id; }
	int GetLevel(){ return _level; }
	float GetDamageReduction() { return _damageReduction; }
	int GetCriticalChance() { return _criticalChance; }
	int GetMoney() { return _money; }
	int GetExperiencePoints() { return _experiencePoints; }
	const int GetMaxHealth() { return _maxHealth; }

	glm::vec3 GetStartPos() { return _startPos; }

	enum State
	{
		attacking,
		choosing,
		defending,
		idle,
	};

	enum State gameEntityState = idle;
	void ToggleHitboxDisplay(bool displaying);

	virtual void UpdateHealthText() {};

	MyEntity* hitBox;

protected:

	Timer* _movingTimer;
	Timer* _waitingTimer;
	std::vector<Projectile*> _projectiles;
	virtual void FireProjectile(GameEntity* target, int amount, float interval);

	virtual void UseItem(int index);

	std::vector<Item*> _items;
	GameEntity* _target;
	glm::vec3 _startPos;

	float _damageReduction;
	int _criticalChance;

	int _selectedEntityCount;
	bool _shotsFired = false;
	bool _shouldDisplayHitboxes = false;

	glm::vec3 ObtainNormalizedVector(glm::vec3 targetPosition);
	glm::vec3 _initialTargetVector;
	int _maxHealth;

private:
	int _id;
	int _level;
	int _money;
	int _experiencePoints;
};
#endif // !GAMEENTITY_H