#include "UIWindow.h"

UIWindow::UIWindow()
{
	this->text = new Text();
	this->text->text = "Are you ready to fight?";
	this->playerTextA = new Text();
	this->playerTextB = new Text();
	this->enemyText = new Text();

	this->text->pivot = glm::vec2(128.0f, 0.0f);
	this->enemyText->color = BLACK;

	this->SetTextPosition(playerTextA, glm::vec3(16, 648, 0));
	this->SetTextPosition(playerTextB, glm::vec3(16, 688, 0));
	this->SetTextPosition(enemyText, glm::vec3(752, 512, 0));

	textComponents.push_back(text);
	textComponents.push_back(playerTextA);
	textComponents.push_back(playerTextB);
	textComponents.push_back(enemyText);
}

UIWindow::~UIWindow()
{

}

void UIWindow::Update(float deltaTime)
{

}