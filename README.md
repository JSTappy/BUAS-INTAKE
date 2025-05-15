Collidius
====

Collidius is the game I am making for my intake assignment at the BUAS. The game is a 2D Turn-Based RPG with the theme 'collision'. Its a fairly simple game. Try to take down the enemy without getting hit.

JST2D
---------
JST2D is a 2D framework based on OpenGL(2.1+). It compiles and runs on Windows. This framework will get updated as time goes along. but not via this repository. I will continue this framework via my other repository:

Controls:

- W,A,S,D to navigate through your action blocks and command window. (A and D for Left and Right with the action blocks, W and S for Up and Down in the command window and enemy choosing)

- Q,E To control the players action. (Q for the actions of the top player, player 1. E for the actions of the bottom player, player 2)

- [TAB] to go back out of command window choosing

Compiling
---------

Install the following:

- Visual Studio 2022 Community Edition

Then open the `Buas_Intake.sln` file after you cloned or downloaded this repository. Once you opened this file, you will have to make sure to right click the Buas_Intake directory in Visual Studio and set it as the start up project if it is not done already. After that, you navigate to Build and then press 'Build Solution' or alternatively press 'CTRL + SHIFT + B'. Once its done building, you can either click on the green play arrow in Visual Studio, or you can navigate through the Debug / Release folder to run the '.exe' file there.

Libraries
---------

- glfw: <http://www.glfw.org/>
- glew: <http://glew.sourceforge.net/>
- glm: <http://glm.g-truc.net/>
- Freetype: <https://freetype.org/>
- Audio: <https://github.com/jpvanoosten/Audio/tree/main/inc/Audio>
