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

}

void Item::ApplyItem(GameEntity* target) 
{
	if (_uses <= 0)return; //If there are no users, don't use it
	_uses--; //uses - 1
	_itemSound->play(); //Play the sound
	switch (_itemType)
	{
		case 0: //Heal 20 % HP if you can
			if (target->health + target->GetMaxHealth() / 5 > target->GetMaxHealth())
			{
				target->health = target->GetMaxHealth();
				target->UpdateHealthText(); 
				return;
			}
			target->health += (target->GetMaxHealth() / 5); //Add the max hp divided by 5
			target->UpdateHealthText(); 
			return;
		case 1: //More power, less speed. 25 % both
			target->power += (target->power / 4);
			target->speed -= (target->speed / 4);
			return;
		case 2: //More speed, less power. 25 % both
			target->speed += (target->speed / 4);
			target->power -= (target->power / 4);
			return;
		case 3: //More power at the cost of HP 50%
			target->power += target->power / 2;
			if (target->health > target->GetMaxHealth() / 2)
			{
				target->health -= (target->GetMaxHealth() / 2);
				target->UpdateHealthText();
				return;
			}
			target->health = 1; //Set health to 1 if the damage exceeds 0
			target->UpdateHealthText();
			return;
	}
}