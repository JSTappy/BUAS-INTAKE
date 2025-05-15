#include "Item.h" 
#include "GameEntity.h" 

Item::Item(char itemType, char uses)
{
	//Assign the variables
	_uses = uses;
	_itemType = itemType;
}

Item::~Item()
{
	delete _itemSound;
	_itemSound = nullptr;
}

void Item::ApplyItem(GameEntity* target)
{
	if (_uses <= 0) return; //If no uses, do nothing
	_uses--; //Subtract uses
	_itemSound->play(); // Play sound

	switch (_itemType)
	{
	case 0: //Heal 20% HP
	{
		short healAmount = target->GetMaxHealth() / 5;
		int healed = target->health + healAmount; //Promote to int
		int maxHealth = target->GetMaxHealth(); //Promote to int
		target->health = static_cast<short>(std::min(healed, maxHealth)); //Cast it to a short again
		target->UpdateHealthText();
		return;
	}

	case 1: //More power, less speed (25% both)
	{
		int powerGain = target->power / 4;
		int speedLoss = target->speed / 4;

		target->power = std::min(target->power + powerGain, 255); //Cap at 255
		target->speed = std::max(target->speed - speedLoss, 1);   //Avoid dropping to 0 or negative
		return;
	}

	case 2: //More speed, less power (25% both)
	{
		int speedGain = target->speed / 4;
		int powerLoss = target->power / 4;

		target->speed = std::min(target->speed + speedGain, 255); //Cap at 255
		target->power = std::max(target->power - powerLoss, 1); //Ensure minimum power
		return;
	}

	case 3: //More power at cost of 50% HP
	{
		int powerGain = target->power / 2;
		target->power = std::min(target->power + powerGain, 255);
		int hpLoss = target->GetMaxHealth() / 2;

		if (target->health > hpLoss)
		{
			target->health -= hpLoss;
		}
		else
		{
			target->health = 1; //Avoid death
		}
		target->UpdateHealthText();
		return;
	}
	}
}