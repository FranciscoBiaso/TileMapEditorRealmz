# Tile Map Editor - Realmz :earth_americas:
> ### A scenario editor for creating 2D [tile](https://en.wikipedia.org/wiki/Tile-based_video_game) maps using C++ language and [GTK](https://www.gtk.org/) library.
> The purpose of the software is to allow the end user to be able to create scenarios and export them for use in games.
---
### Software Modules:
* #### Graphics module:
> The graphics module is intended to assist in the manipulation of images. Textures atlases can be loaded and their images can be sliced and edited to later be transformed into important scenario data.
* #### Thing creator module:
> The module of manipulation of things aims to create a list of all objects (things) that can fill the scenario.
* #### Utility module:
> The utility module, no less important, contains tools that help other modules to achieve their purposes.
* #### Artistic module:
> The artistic module provides tools to allow visualization of the scene and facilitates the creation process.
---
### How to compile under windows?
* [Download vckpg](https://github.com/Microsoft/vcpkg)
* [Execute file] bootstrap-vcpkg.bat
* open cmd, go into vcpkg dir and install packages.
* vcpkg.exe install jsoncpp
* vcpkg.exe install glm
* vcpkg.exe install gtk
* Inside visual studio 2019 open TME.sln
* Select project properties. Under Tab "Configuration Properties", select Tab "General". Under Menu "C++ Language Standard", choose "Preview - Features the Latest C++ Working Draft (std:C++latest).
* Check if your configuration is release and target plataform x86.
* Select Tab "Linker". Select Tab "input". Go to Menu "Additional Dependencies" them add opengl32.lib.
* Build!
---
### Additional Info?
* [Gtk+](https://www.gtk.org/docs/installations/windows/);
* [Download Microsoft Visual C++ 2019](https://support.microsoft.com/pt-br/help/2977003/the-latest-supported-visual-c-downloads).
* [JSON LIB](https://github.com/nlohmann/json)
* [GLM](https://glm.g-truc.net/0.9.9/index.html)
---
### How to update user interface ?
* [Download Glade Interface 3.8.5](https://glade.gnome.org/).
* Import a file "builder.ui".
---
### How-to-use document
[LINK](https://docs.google.com/document/d/1b8dCI-x1lf8KnOY9ViKGYOPtrx2xBoiCGlwxvzwZAoQ/edit?usp=sharing)
---
### App Download
[LINK](https://gofile.io/d/P9r7fH)
---
