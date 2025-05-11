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

///@brief Controllable Player GameEntity
class Player : public GameEntity
{
public:
	///@brief Player Constructor
	Player(int id, int level, float health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints);

	///@brief Player Destructor
	virtual ~Player();

	// --- Functions --- //

	///@brief Update method
	///@param deltaTime
	virtual void Update(float deltaTime) override;

	///@brief Assigns the actionKey to the player. This will be the button that this specific player will use to perform actions
	///@param int actionKey
	void AssignActionKey(int actionKey);

	///@brief Updating the health text to the player health
	void UpdateHealthText() override;

	///@brief Getter for the _isGrounded bool
	///@return _isGrounded
	bool GetIsGrounded() { return _isGrounded; }

	///@brief Getter for the _turnsToSkip bool
	///@return _turnsToSkip
	int GetTurnsToSkip() { return _turnsToSkip; }

private:

	// --- Functions --- //

	///@brief Applies the velocity and gravity to the player
	///@param float deltaTime
	void Move(float deltaTime);

	///@brief Launch upwards from your current position
	///@param float jumpForce, the force that will be applied to the velocity
	///@param float jumpforceMultiplier, the multiplier of jumpForce
	void Jump(float jumpForce, float jumpforceMultiplier);

	///@brief Gives the player the ability to jump
	///@param deltaTime
	void EnableJump(float deltaTime); 

	///@brief Handles what the player should do if it is touching the ground
	void GroundCheck();

	///@brief Handles the command window
	void SelectBlock();

	///@brief Handles the the selected action chosen by the player
	void HandleAction(); 

	///@brief Handles what the projectile should do when hitting its target
	void HandleProjectileAction() override;

	///@brief Handles the attack to perform
	///@param int attackLevel, the type of attack that will be performed
	void PerformAttack(int attackLevel);

	///@brief Handles the defending action to perform
	///@param int attackLevel, the type of attack that will be performed
	void PerformDefenseAction(int defenseLevel);

	///@brief Basic function for dealing damage
	///@param int damage for the amount of damage dealt, 
	///@param GameEntity* target for who recieves the damage
	void BasicAttack(int damage, GameEntity* target);

	///@brief Sets up the punch attack for this player towards the target
	///@param GameEntity* target for who recieves the damage
	void PunchAttack(GameEntity* target);

	///@brief Sets up the jump attack for this player towards the target
	///@param GameEntity* target for who recieves the damage
	void JumpAttack(GameEntity* target);

	///@brief Sets up the projectile attack for this player towards the target
	///@param GameEntity* target for who recieves the damage
	void MashProjectileAttack(GameEntity* target);

	///@brief Sets up the dash attack for this player towards the target
	///@param GameEntity* target for who recieves the damage
	void DashAttack(GameEntity* target);

	///@brief Handles how the punch will be performed
	///@param float deltaTime
	void HandlePunch(float deltaTime);

	///@brief Handles how the jump will be performed
	///@param float deltaTime
	void HandleJumpAttack(float deltaTime);

	///@brief Handles the properties of the projectile before creating it
	void HandleProjectileMash();

	///@brief Handles how the dash attack will be performed
	///@param float deltaTime
	void HandleDashAttack(float deltaTime);

	///@brief Use the item based on the index in the _items vector
	///@param int index, the index in the _items vector
	void UseItem(int index) override;

	///@brief Check which attack was chosen in the command window
	///@param float deltaTime, to wait and update the _waitingTimer
	void SwitchAttackType(float deltaTime);

	///@brief Handles how the player acts when it is the players' turn choosing
	void HandleChoosing();

	///@brief Initiating the visual slider. This slider will follow you and show you how strong your attack is
	void InitiateVisualSlider();


	///@brief Clears the HUD like the actionblock, visual slider and command window
	void ClearHUD();

	///@brief Reset the player state to idle for the next turn or attack
	void ResetToIdle();

	// --- Variables --- //

	///@brief Visual Slider to display the attacks strength
	VisualSlider* _visualSlider;

	///@brief Action block for the player to choose actions
	ActionBlock* _actionBlock;

	///@brief Timer to check if you should ascend after jumping
	Timer* _ascendTimer;

	///@brief Timer to check how far along specific attacks you are
	Timer* _attackingTimer;

	///@brief Timer to check if the action should already be performed or not
	Timer* _waitingTimer;

	///@brief Check if the player should fall
	bool _shouldFall;

	///@brief Check if the player is grounded
	bool _isGrounded;

	///@brief Check if the player is performing an attack
	bool _isCharging;

	///@brief Check how many turns need to be skipped
	int _turnsToSkip;

	///@brief Check how many times the player has jumped on the enemy
	int _jumpAttacksHit = 0;

	///@brief See what type of attack the player should perform
	int _attackType;

	///@brief The ASCII KeyCode for the player to perform actions
	int _actionKey;

	///@brief The amount of energy you gained
	double _energyStored = 0.0;

	///@brief The amount of energy you gain from a single charge
	double _energyGain = 0.25;

	///@brief The for the maximum amount of energy
	double _energyMax = 3.0;

	///@brief Gravity, the downwards force
	float _gravity = 5000.0f;

	/// @brief The velocity of the player
	glm::vec3 _velocity = glm::vec3(0,0,0);
};

#endif // PLAYER_H 