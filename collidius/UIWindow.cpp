#include "UIWindow.h"

UIWindow::UIWindow()
{
	//Make the new Texts
	this->text = new Text();
	this->playerOneText = new Text();
	this->playerTwoText = new Text();
	this->enemyText = new Text();

	//This is the urnmanagers battleText
	this->text->pivot = glm::vec2(128.0f, 0.0f);

	//Set the enemy text color to black due to it not being on the UIWindow
	this->enemyText->color = BLACK;

	//Set the text Positions
	this->SetTextPosition(playerOneText, glm::vec3(16, 648, 0));
	this->SetTextPosition(playerTwoText, glm::vec3(16, 688, 0));
	this->SetTextPosition(enemyText, glm::vec3(752, 512, 0));

	//Add them to the textComponents
	textComponents.push_back(text);
	textComponents.push_back(playerOneText);
	textComponents.push_back(playerTwoText);
	textComponents.push_back(enemyText);
}

UIWindow::~UIWindow()
{

}

void UIWindow::Update(float deltaTime)
{

}