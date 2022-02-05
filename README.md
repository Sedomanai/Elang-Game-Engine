# Elang Game Engine

Elang Game Engine is a work in progress intended to make generic 2d games in the future. It's a project continued to built mostly for studying purposes but also to provide some mechanics that are not 

Source for building the engine, including all the qt modules. Dependencies not included; you must set them up yourself.

common files are header-only custom scripts with no external dependencies (none outside the common folder) except for std
tools are heavily reliant on dependencies and/or specifically designed around the entity component system
elements are attached as components to stage objects with world data
apparatus are function-only modules with common inter-elemental interactions, helping organization and compile time

the level of dependency is common < tools < elements < apparatus < engine


dependency list:

	header-only
entt <entt/entt.h>
glew32 <Gl/glew.h>


fpng <fpng/fpng.h>
