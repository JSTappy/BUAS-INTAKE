#ifndef ENEMY_H 
#define ENEMY_H

#include "config.h"
#include "GameEntity.h"
#include "Projectile.h"

///@brief The enemy class, the opponent of the players
class Enemy : public GameEntity
{
public:
	/// @brief constructor
	Enemy(unsigned char id, short health, unsigned char power, unsigned char defense, unsigned char speed, float damageReduction, unsigned char criticalChance);

	/// @brief destructor
	virtual ~Enemy();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime) override; //update

	///@brief Update the health text on the UI window
	void UpdateHealthText() override;

private:

	///@brief Reset the player state to idle for the next turn or attack
	void ProjectileAttack(GameEntity* target);

	////@brief This function handles how a projectile will act upon colission
	void HandleProjectileAction() override;

	///@brief Fires a projectile, is overridden from GameEntity
	///@param GameEntity* target, the entity that is being targeted
	void FireProjectile(GameEntity* target) override;

	///@brief Handles the attack
	///@param float deltaTime
	void HandleAttacking(float deltaTime);

	///@brief Handles choosing
	///@param float deltaTime
	void HandleChoosing(float deltaTime);

	//Star indicator sprite
	Entity* _starIndicator;
};

#endif // ENEMY_H 