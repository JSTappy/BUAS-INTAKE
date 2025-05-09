#ifndef PLAYER_H 
#define PLAYER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "camera.h"
#include "inputmanager.h"
#include "GameEntity.h"
#include "ActionBlock.h"
#include "VisualSlider.h"

class Player : public GameEntity
{
public:
	/// @brief constructor
	Player(int id, int level, float health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints);

	/// @brief destructor
	virtual ~Player();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update
	void EnableJump(float deltaTime);

	void AssignActionKey(int jumpKey);

	void BasicAttack(int damage, GameEntity* target);
	void PunchAttack(GameEntity* target);
	void MashProjectileAttack(GameEntity* target);
	void JumpAttack(GameEntity* target);

	void SelectBlock();
	void HandleAction();

	void HandleProjectileAction() override;
	void HandlePunch();
	void HandleProjectileMash();
	void HandleJumpAttack(float deltaTime);

	void PerformAttack(int attackLevel);
	void InitiateVisualSlider();
	void UpdateHealthText() override;

	bool GetIsGrounded() { return _isgrounded; }

private:
	Timer* _ascendTimer;

	Timer* _attackingTimer;

	Timer* _waitingTimer;

	Timer* _multipleProjectileTimer;

	int _jumpAttacksHit = 0;
	bool _shouldFall;
	bool _isgrounded;
	bool _completedMash = false;

	int _attackType;
	int _actionKey;

	float _projectileChargeStored = 0.0f; //the amount of 'charge' stored
	float _projectileChargeGain = 0.25f; //the amount of 'charge' gained
	float _projectileChargeMax = 3.0f; //the maximum amount of 'charge' 1 projectile will be spawned per full number


	float _gravity = 5000.0f;

	glm::vec3 _velocity = glm::vec3(0,0,0);

	void Move(float deltaTime);
	void Jump(float jumpForcefloat, float jumpforceMultiplier);
	void GroundCheck();

	void SwitchAttackType(float deltaTime);
	void HandleChoosing();

	void ClearHUD();
	void ResetToIdle();

	VisualSlider* _visualSlider;
	ActionBlock* _actionBlock;
};

#endif // PLAYER_H 