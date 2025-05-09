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
	Enemy(int id, int level, int health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints);

	/// @brief destructor
	virtual ~Enemy();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	void BasicAttack(int damage, GameEntity* target);
	void ProjectileAttack(GameEntity* target);
	void HandleProjectileAction() override;
	void FireProjectile(GameEntity* target, int amount, float interval) override;
	void HandleAction();
	void PerformAttack(int attackLevel);
	void UpdateHealthText() override;

private:

	Timer* _attackingTimer;
};

#endif // ENEMY_H 