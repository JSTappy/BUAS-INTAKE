#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "entity.h"
#include "timer.h"

#include "Projectile.h"
#include "Item.h"

#include <Audio/Sound.hpp>

class GameEntity : public Entity
{
public:
	/// @brief Constructor
	GameEntity(unsigned char ID, short Health, unsigned char Power, unsigned char Defense, unsigned char Speed, float DamageReduction, unsigned char CriticalChance);

	/// @brief Destructor
	~GameEntity();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	virtual void TeleportToPosition(glm::vec3 position);
	virtual void MoveTowardsPosition(glm::vec3 position, unsigned short movingSpeed, float deltaTime);
	virtual void HandleProjectileAction() {};
	void HandleProjectileCollision(Projectile* p, GameEntity* t);
	void DestroyProjectile(Projectile* p);
	void SetStartPos();

	unsigned short CalculateAttackStat(unsigned char damage, float moveMultiplier);
	unsigned short CalculateDefenseStat(unsigned char defense, float damageReduction);
	void DealDamage(GameEntity* target, float multiplier);

	void ShowDamage(GameEntity* target, unsigned char damageTaken);

	bool completedTurn;
	bool alive;

	bool choosingAction;
	bool choosingCommand;
	bool choosingTarget;
	bool isCriticalHit;

	short health;
	unsigned char power;
	unsigned char defense;
	unsigned char speed;

	bool IsCriticalHit();
	unsigned char GetID() { return _id; }
	float GetDamageReduction() { return _damageReduction; }
	unsigned char GetCriticalChance() { return _criticalChance; }
	const short GetMaxHealth() { return _maxHealth; }
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

	Entity* hitBox;

protected:

	Timer* _movingTimer;
	Timer* _waitingTimer;
	Timer* _damageSpriteTimer;
	std::vector<Projectile*> _projectiles;
	virtual void FireProjectile(GameEntity* target, unsigned char amount, float interval);

	virtual void UseItem(unsigned char index);

	std::vector<Item*> _items;
	GameEntity* _target;
	glm::vec3 _startPos;

	float _damageReduction;
	unsigned char _criticalChance;

	unsigned char _selectedEntityCount;
	bool _attackPerformed = false;
	bool _shouldDisplayHitboxes = false;

	glm::vec3 ObtainNormalizedVector(glm::vec3 targetPosition);
	glm::vec3 _initialTargetVector;
	short _maxHealth;
	Entity* _damageSprite;

	Audio::Sound* _hitSound = new Audio::Sound("assets/audio/hitone.ogg", Audio::Sound::Type::Sound);
	Audio::Sound* _critHitSound = new Audio::Sound("assets/audio/crithit.ogg", Audio::Sound::Type::Sound);
	Audio::Sound* _projectileSound = new Audio::Sound("assets/audio/projectileone.ogg", Audio::Sound::Type::Sound);
private:
	unsigned char _id;
};
#endif // !GAMEENTITY_H