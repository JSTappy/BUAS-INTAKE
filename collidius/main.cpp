// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include "myscene.h"

int main( void )
{
	Game game;
	MyScene* myscene = new MyScene();
	do
	{
		game.Run(myscene);
	}
	// Check if the ESC key was pressed or the window was closed in the IsRunning Function
	while (game.IsRunning());


	delete myscene;

	return 0;
}
