#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "entity.h"
#include "timer.h"

#include "Projectile.h"
#include "Item.h"

#include <Audio/Sound.hpp>

///@brief The class of the entities that are engaging in battles
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

	///@brief The entity states
	enum State
	{
		attacking, //The state which attacks will be performed in
		choosing, //The state which handles choosing
		defending, //The state which handles defending for the players
		idle, //Make the entity do nothing
	};

	///@brief Current entity state
	enum State gameEntityState = idle;

	///@brief Set the starting position
	void SetStartPos();

	///@brief Turns the hitboxes on or off
	void ToggleHitboxDisplay(bool displaying);

	///@brief Update the health text on the UI window
	virtual void UpdateHealthText() {};

	///@brief Get the ID from this game entity
	unsigned char GetID() const { return _id; }

	///@brief Get the damage reduction from this game entity
	float GetDamageReduction() { return _damageReduction; }

	///@brief Get the critical chance from this game entity
	unsigned char GetCriticalChance() { return _criticalChance; }

	///@brief Get the maximum amount of health from this game entity
	short GetMaxHealth() const { return _maxHealth; }

	///@brief Get the starting position from this game entity
	glm::vec3 GetStartPos() { return _startPos; }

	///@brief checks if this game entitity its turn has been completed
	bool completedTurn;

	///@brief Checks if this entity is alive or not
	bool alive;

	///@brief Checks if this entity is choosing an action
	bool choosingAction;

	///@briefChecks if this entity is choosing a command
	bool choosingCommand;

	///@brief Checks if the attack is a critical hit
	bool isCriticalHit;

	///@brief The amount of hitpoints this entity has
	short health;

	///@brief The amount of power this entity has
	unsigned char power;

	///@brief The amount of defense this entity has
	unsigned char defense;

	///@brief The amount of speed this entity has
	unsigned char speed;

	///@brief THe hitbox sprite
	Entity* hitBox;

protected:

	///@brief Teleport to the position that you put in as a parameter
	///@param glm::vec3 position, the position you are teleporting towards
	virtual void TeleportToPosition(glm::vec3 position);

	///@brief Move  to the position that you put in as a parameter at the speed you also put in as a parameter
	///@param glm::vec3 position, the position you are teleporting towards
	///@param unsigned short movingSpeed, the speed you are moving towards the position
	///@param float deltaTime, the time between seconds
	virtual void MoveTowardsPosition(glm::vec3 position, unsigned short movingSpeed, float deltaTime);

	///@brief This function handles how a projectile will act upon colission
	virtual void HandleProjectileAction() {};

	///@brief The method for dealing damage
	///@param GameEntity* target, the entity that is being damaged
	///@param float multiplier, the amount the base power is being multiplied with
	void DealDamage(GameEntity* target, float multiplier);

	///@brief Handles the AABB colission between the target and the projectile
	///@param Projectile* p, the projectile that is being fired
	///@param GameEntity* t, the entity that is being targeted
	void HandleProjectileCollision(Projectile* p, GameEntity* t);

	///@brief Fires a projectile, can be overridden
	///@param GameEntity* target, the entity that is being targeted
	virtual void FireProjectile(GameEntity* target);

	///@brief Use an item and apply its effects to this entity
	///@param unsigned char index, the index of the _items list that determines what item you will use
	virtual void UseItem(unsigned char index);

	///@brief Get a normalized vector from this position to the target position
	///@param glm::vec3 targetPosition, the other position you are making a normalized vector between
	glm::vec3 ObtainNormalizedVector(glm::vec3 targetPosition);


	///@brief The target when attacking
	GameEntity* _target;

	///@brief The starting position
	glm::vec3 _startPos;

	///@brief Get a normalized vector from this position to the target position
	Timer* _movingTimer;

	///@brief Timer to wait for certain actions
	Timer* _waitingTimer;

	///@brief Timer for how long the damage sprite stays onscreen
	Timer* _damageSpriteTimer;

	///@brief The list of active projectiles
	std::vector<Projectile*> _projectiles;

	///@brief The list of items
	std::vector<Item*> _items;

	///@brief The amount of damage reduced in percent (0 - 1, 0.01f is 1%)
	float _damageReduction;

	///@brief The amount of chance on a critical hit you have
	unsigned char _criticalChance;

	///@brief Check if the attack has been performed
	bool _attackPerformed = false;

	///@brief Check if the hitboxes should be displayed
	bool _shouldDisplayHitboxes = false;

	///@brief The vector from entity to entity normalized, this gets assigned once
	glm::vec3 _initialTargetVector;

	///@brief The sprite that shows up when dealing damage
	Entity* _damageSprite;

	///@brief The hit sound
	Audio::Sound* _hitSound = new Audio::Sound("assets/audio/hitone.ogg", Audio::Sound::Type::Sound);

	///@brief The critical hit sound
	Audio::Sound* _critHitSound = new Audio::Sound("assets/audio/crithit.ogg", Audio::Sound::Type::Sound);

	///@brief The projectile sound
	Audio::Sound* _projectileSound = new Audio::Sound("assets/audio/projectileone.ogg", Audio::Sound::Type::Sound);

private:
	///@brief Shows the damage on screen as a sprite
	///@param GameEntity* target, the entity that needs to show its damage taken
	///@param unsigned char damageTaken, the amount of damage that has been taken
	void ShowDamage(GameEntity* target, unsigned char damageTaken);

	///@brief Destroys the projectile
	///@param Projectile* p, the projectile that is gonna be destroyed
	void DestroyProjectile(Projectile* p);

	///@brief Calculate the attack stat
	///@param Projectile* p, the projectile that is gonna be destroyed
	unsigned short CalculateAttackStat(unsigned char damage, float moveMultiplier);

	///@brief Randomly decides if the hit is a critical hit
	bool IsCriticalHit();

	///@brief The maximum amount of health the entity can have
	short _maxHealth;

	///@brief The ID of the game entity
	unsigned char _id;
};
#endif // !GAMEENTITY_H