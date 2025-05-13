#include "Item.h" 
#include "GameEntity.h" 

Item::Item(char itemType, char uses)
{
	_uses = uses;
	_itemType = itemType;
}

Item::~Item()
{

}

void Item::ApplyItem(GameEntity* target) 
{
	if (_uses <= 0)return; //destroy later
	_uses--;
	_itemSound->play();
	switch (_itemType)
	{
		case 0: //heal 20 % HP if you can
			std::cout << "Helth higher lowl" << std::endl;

			if ((target->health + target->GetMaxHealth() / 5) > target->GetMaxHealth())
			{
				target->health = target->GetMaxHealth();
				target->UpdateHealthText();
				return;
			}
			target->health += (target->GetMaxHealth() / 5);
			target->UpdateHealthText();
			return;
		case 1: //more power, less speed. 25 % both
			target->power += (target->power / 4);
			target->speed -= (target->speed / 4);
			return;
		case 2: //more speed, less power. 25 % both
			target->speed += (target->speed / 4);
			target->power -= (target->power / 4);
			return;
		case 3: //more power at the cost of HP 50%
			target->power += target->power / 2;
			if ((target->health) > target->GetMaxHealth() / 2)
			{
				target->health -= (target->GetMaxHealth() / 2);
				std::cout << "Helth higher lowl"  << std::endl;
				target->UpdateHealthText();
				return;
			}
			target->health = 1;
			target->UpdateHealthText();
			std::cout << "Helth lowler ha" << std::endl;

			return;
	}
}