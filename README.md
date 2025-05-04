JST2D
====

JST2D is a 2D framework based on OpenGL(2.1+). It compiles and runs on Mac, Linux and Windows. This framework will later be used for my intake assignment for BUAS

Compiling
---------

Use cmake to create your project files in a separate `build` directory.

> You should always be able to delete the `build` directory without losing your work. Whatever you do, work in your `src` directory.

Linux
-----

On Debian-based Linux, install the following libraries:

	sudo apt install git cmake doxygen g++
	sudo apt install libgl1-mesa-dev libglu1-mesa-dev
	sudo apt install libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev

Run these commands:

	mkdir build
	cd build
	cmake ..
	make

Then you can run the demo:

	./demo

Make sure to add any new files you create to the `CMakeLists.txt` file.

Mac OS X
--------

Install the following:

- XCode + commandline developer tools
- git
- cmake
- doxygen

Run these commands:

	mkdir build
	cd build
	cmake -Wno-dev ..
	make

Then you can run the demo:

	./demo

Make sure to add any new files you create to the `CMakeLists.txt` file.

MS Windows (VS2015)
-------------------

Install the following:

- Visual Studio 2015 Community Edition (VS2013 also works fine) + Visual C++ (do a custom install)
- git
- cmake
- doxygen

In the powershell, run:

	mkdir build
	cd build
	cmake ..

Then doubleclick the `jst2d.sln` file in the `build` directory. Right-click the 'start' directory in Visual Studio and select 'Set as StartUp Project'. Then press ctrl-F5.

Make sure that when you add new files within Visual Studio, you move them to the correct directory in `src` and add them to the `CMakeLists.txt` file.

Documentation
-------------

To generate the documentation, run doxygen (with Doxyfile as input) in the root directory of this project:

	doxygen Doxyfile

Then open `documentation.html` in the `doc` directory.

Libraries
---------

- glfw: <http://www.glfw.org/>
- glew: <http://glew.sourceforge.net/>
- glm: <http://glm.g-truc.net/>


The `CMakeLists.txt` file is based on the one found here: <http://www.opengl-tutorial.org/>.

If you're having trouble compiling this project, follow instructions on <http://www.opengl-tutorial.org/>. Once that compiles, this will probably also compile.