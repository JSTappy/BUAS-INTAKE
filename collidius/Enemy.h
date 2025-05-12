#ifndef ENEMY_H 
#define ENEMY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "MyEntity.h"
#include "GameEntity.h"
#include "Projectile.h"

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

	void BasicAttack(char damage, GameEntity* target);
	void ProjectileAttack(GameEntity* target);
	void HandleProjectileAction() override;
	void FireProjectile(GameEntity* target, unsigned char amount, float interval) override;
	void HandleAction();
	void PerformAttack(unsigned char attackLevel);
	void UpdateHealthText() override;

private:

	Timer* _attackingTimer;

	MyEntity* _starIndicator;
};

#endif // ENEMY_H 