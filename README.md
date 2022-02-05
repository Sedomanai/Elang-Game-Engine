# Elang Game Engine

Elang Game Engine is a work in progress intended to make generic 2d games in the future. It's a project being built mostly for software architecture studying purposes but also a lightweight alternative that provides some 2d game design mechanics that are not readily available in common engines. The source is tailor written for my own convenience, possibly offering everything it can to helpe me make games that I see fit in the future.

Common files are header-only custom scripts with no external dependencies, none outside the common folder, except for the std library. Tools are heavily reliant on dependencies and/or specifically designed around the entity component system. Elements are essentially an Elang alias of ECS components that are meant to be attached to stage objects with world data (though there is nothing technical stopping entt from attaching whatever users want to their stage entities). Finally, apparatus are function-only modules with common inter-elemental interactions, helping with code organization and compile time. The level of dependency is ( common < tools < elements < apparatus < engine ).

There are several design policies worth noting. First of all, encapsulation is not the engine's highest priority. That doesn't mean it was ignored completely, but I did not pursue it at the expense of convenience, readability, and optimization. Almost the entire source heavily depends upon ECS design (entity component system) which is very popular in gaming architecture. The engine relies exclusively upon the ENTT library to manage this. The problem is that any access to the registry completely lays bare every single component attached to that registry. On top of that, the internal framework often refers to a couple of global pointers to the registry, which the user is expected to bind with their own registry instances. 

This potentially makes everything global, inviting many problems usually associated with it. So far, it has not been as dangerous as it sounds, and the work flow is very much like working with opengl, which is also mostly global bindings. Much of the apparatus classes are stand alone functions, but I may consider makking static classes with encapsulated data, if it becomes a problem in the future. As of now, users are discouraged from creating and deleting asset entities manually. 


# Intended Usage

The initial setup begins with creating a game class which inherits the engine class and overriding its virtual methods. It must contain at least one instance of a Project class and a Stage class. The Project manages all the assets, including texture, texture atlas, fonts, sound (pending), materials, cameras, and painters (which are like draw layers bound to a vertex shader and a fragment shader.) The Stage contains all the world entities i.e. game objects, with qualities like positions, sprites, hitboxes, and so on. 


# Loading

The load method must begin by binding an instance of a Project. The user can either load assets from a file path directly, or use the apparatus function 'reloadAll' to load all assets from an external folder recursively. Alternatively, the user can also save a Project by serializing and archiving it and load that file to the Project. This file is expected to have an .elang extension. 

The .elang file itself has two modes: internal binary, and external tracking. All minor assets are internalized. As of now, the only major asset is texture (this may change fairly soon, as of v0.1.0), since even a couple can quickly build up to megabytes. In internal modes, the .elang file itself holds the binary to rgba textures which are encoded and decoded with super fast algorithms taking up even less ram in comparison to the original asset files. The problem with this method is that it duplicates asset data such as png files into its own binary. Depending on the size of the project, this may not be so desirable. This is also not the method to choose if it is undesirable to load everything to RAM all at once. 

Because of this, the Project can also be saved in an external tracking mode. Currently this mode only affects textures. Instead of directly containing their binary format, it is assigned an asset directly and made to track every file in it. The user can then load each individual texture manually using a string key which is the relative path from the asset folder OR use the above 'reloadAll' method. There is not too much overhead in comparison to the other mode, but it must be mentioned that the original png files are re-encoded (still usable by other programs) so that they reload much faster the next time. This must be done everytime the png is modified and saved by an external program. And they must be deployed together in the final versions. 

The best aspect entwining all these methods is that the project serializes the entities themselves, and the entity index is constant throughout the project. In other words, stage elements can hold direct index reference to asset entities which lifecycle survives runtime via serialization. The sprite for example holds a uint32_t material and painter key which is used to derive the associated asset with o(1) time instead of using a string field to fetch from a map with o(logN). Even in external mode, the project keeps track of every key relative to the asset folder and does not delete the index reference until the file itself is deleted, (this includes changing the name of the file) in which all index refernce will point to a null entity. You must account for this when you write for stage elements in the future. )


# Gmae Logic

This is the most straightforward..etc. Too tired... for tomorrow.


# Batch & Paint

The paint method deals with graphics. Currently the Elang Engine only supports the OpenGL pipeline. Tomorrow.

