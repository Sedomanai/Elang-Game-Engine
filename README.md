# Elang Game Engine (v0.1.0)

Elang Game Engine is a work in progress generic 2d game engine. It's a project being built mostly for software crafting experience, but also to make a lightweight alternative to common game engines, providing some 2d game design mechanics that are not readily available in the market. The source is tailor written for my own convenience, possibly offering everything it can to help me make games that I see fit in the future.

Common files are header-only custom scripts with no external dependencies - none outside the common folder - except for the std library. Tools are heavily reliant on dependencies and/or specifically designed around a true Entity Component System (ECS), which is not only a unique software architectural pattern but also helps immensely with caching and therefore performance. Elements are essentially an Elang alias of ECS components that are meant to be attached to stage objects with world data (though there is nothing technical stopping users from attaching whatever component they want to their stage entities). Finally, apparatus are function-only modules with common inter-elemental interactions, helping with code organization and compile time. The level of dependency is ( common < tools < elements < apparatus < engine ).

There are several design policies worth noting. First of all, encapsulation is not the engine's highest priority. That doesn't mean it was ignored completely, but I did not pursue it at the expense of convenience, readability, and optimization. Almost the entire source heavily depends upon ECS design which is very popular thing to do in game programming. The engine relies exclusively upon the ENTT library to manage this. The problem is that any access to the registry completely lays bare every single component attached to that registry. On top of that, the internal framework often refers to a couple of global pointers to the registry, which the user is expected to bind with their own registry instances. 

This potentially makes everything global, inviting many problems usually associated with it. So far it has not been as dangerous as it sounds and the work flow is very much like working with glew, which is also mostly global bindings. Much of the apparatus classes are stand alone functions, but I may consider makking static classes with encapsulated data if it becomes a problem in the future. As of now, users are discouraged from creating and deleting asset entities manually. 



# Intended Usage

The initial setup begins with creating a game class which inherits the engine class and overriding its virtual methods.. It must contain at least one instance of a Project class and a Stage class. The Project manages all the assets, including texture, texture atlas, fonts, sound (pending), materials, cameras, and painters (which are like draw layers bound to a vertex shader and a fragment shader.) The Stage contains all the world entities i.e. game objects, with qualities like positions, sprites, hitboxes, and so on. The engine internally uses SDL to open and manage windows.


## Loading

The load method must begin by binding an instance of a Project. Then the user can assign an external asset folder directory to the project, then use the apparatus function 'reloadAll' to load all assets from the folder recursively. Alternatively, the user can also save a Project by serializing and archiving it and load that file to the Project. This file is expected to have an .elang extension. 

The .elang file itself has two modes: internal binary, and external tracking. All minor assets are internalized. As of now, the only major asset is texture, since even a couple can quickly build up to megabytes (this may change fairly soon, as of v0.1.0). In internal binary mode, the .elang file itself holds the binary to rgba textures which are specially encoded and decoded with FPNG, taking up even less ram in comparison to the original asset files. The problem with this method is that it duplicates asset data such as png files into its own binary. Depending on the size of the project, this may not be so desirable. This is also not the method to choose if it is unfeasible to load everything to RAM all at once. It's the best mode for certain flash/mini games or mobile projects.

Because of this, the Project can also be serialized in an external tracking mode. Currently this mode only affects textures. Instead of directly containing their binary format, it is assigned an asset directory and made to track every file in it. The user can then load each individual texture manually using a string key which is the relative path from the asset folder. There are helper apparatus functions to assist this matter in many ways. There's not too much overhead in comparison to the other mode, but it must be mentioned that the original png files are re-encoded (still usable by other graphic editing programs) so that they reload much faster the next time. This must be done everytime the png is modified and saved by an external program. In this mode, the assets must be deployed together in the final build, and must be loaded via relative path. 

The best aspect of entwining all these methods is that the project serializes the entities themselves, and the entity index is constant throughout the project. In other words, stage elements can hold direct index reference to asset entities whose lifecycle can be made to survive runtime via serialization. The sprite for example holds uint32_t material and painter handles which are used to derive the associated asset with o(1) time instead of using a string field to fetch from a map with o(logN). Even in external mode, the project keeps track of every key relative to the asset folder and does not delete the index reference until the file itself is deleted, (this includes changing the name of the file) in which all index refernce will point to a null entity. You must account for this when you write for stage elements in the future by alwyas null checking asset entities.

The asset entities can be wrapped in a templated asset entity holder. Beware though that the holders rely solely on the afforementioned global binding pointers. Generally speaking, there is no need to have more than one Project instance... per project.  

### GLSL

The engine has built in shaders but can also load external glsl files in the asset folder. Vertex shaders must have an extension of .vert and fragment shaders of .frag. (This is almost finished as of v0.1.0 - the architecture is there, I just need to employ it in reloadAll.) Elang engine reads glsl and automatically deduces vertex attributes based on its inputs. There are several glsl grammar rules that must be noted. Refer to the following example-

> in vec3 vName;
> in vec4 v8Color;

Declaration must start from its own line (I believe this is glsl standard). There must be no line breaks or tabs in between. There must be no additional spaces between in, type, and variable name. Also, 32bit (8 bit per channel) color input for vec4 must be prefaced with v8. This is because there is no color type in glsl. Currently (v0.1.0) can only parse the following variable types: vec2, vec3, vec4, and vec4 color-coded.


## Game Logic

The game logic consists of overriding the start, input, and update method. 

The start method is the initialization phase and begins by binding a Stage instance. The Stage consists of various Objects which in turn are composed of Elements. Otherwise, the same rules apply as with the Project instance, except in this case all the Elements are expected to be serialized in a single binary. Currently (v0.1.0) there is no in-built method to export Element data. Each stage is exported as a .stage binary file. However they may soon become part of the Project binary in the future, since a stage can never be seperated from its Project.

The Stage is the practical equivalent to scenes in Unity. Unlike scenes however, there are ways to update and display multiple stages in a single frame. This works via rebinding the global stage pointer every frame coupled with some inter-stage communication. This is because, contrary to the Project instance, there will always be multiple stage instances. Users can employ this method to use hold on to some stages while swapping others. For example, a platformer stage can be separated from its UI, so that the UI objects can hold persistent data and do not have to be copied into every platformer stage. 

This is not impossible with Project instances - which is a technique I use for my Engine GUI where I need to separate Engine assets from the Project game assets. Generally speaking however, one Project and one-or-two Stage per loop is recommended for safe practice. This is the intended usage. The update method is that same loop which happens every frame. (Currently the game loop is in its rawest stage unready for deployment. This will be dealt with in the near future.)

Stage objects can also be wrapped in a templated object entity holder. The same warnings apply as with the Project asset wrappers. In the aforementioned case of binding multiple stages, it is highly discouraged to use this wrapper that refer to the global Stage pointer. Instead, the user is encouraged to use the actual Stage instances as ENTT registries to attain inter-stage communication. The intefrace is the same as that of orthodox ENTT.

There are many helper functions from the apparatus folder that abbreviates many common stage actions such as camera movement or sprite updating. They too rely heavily on the global bindings so users must beware during inter-stage communication within a single frame.

### Key and Mouse input

The input method is where all the input actions are recommended. This means all functions and methods which work with the gKey and gMouse instance. They internally store the state of input of every key and clicks. Below is the grammar for inputs. I use my own lingo for the states, which may seem cryptic at first, but it's easy to become used to it pretty fast.

> if (gKey.state(eKeyCode::A) == eInput::ONCE) // Only true once the moment A is pressed
> if (gMouse.state(0) == eInput::HOLD) // True as long as left mouse is held
> if (gMouse.state(1) == eInput::LIFT) // Only true once the moment right mouse is released.


## Batch & Paint

The paint method deals with graphics. Currently the Elang Engine only supports the OpenGL pipeline. It will be very long before I employ other graphics APIs such as DirectX other than OpenGL. If I ever get around to this, I will try to keep the interface as same as possible. 

The painters and materials are responsible for managing graphic data. Painters act intuitvely like draw layers, which state can be modified to accomodate many individual needs such as Z-buffer usage, sorting, and/or static batching (by locking the layer). It also has some common uniforms like color (the vertex shader must have a matching uniform name, however). Unlike other game engines, the "materials" do not hold reference to shaders. The painters deal directly with shaders, and accepts only batches with matching vertex size. This means it could potentially accept unfit vertices, but at the very least it will not break other batches.

Painters holds reference to one camera and one shader program pipeline, which in turn is constructed by one vertex shader and one fragment shader. This means a fragment shader can be reused for multiple vertex shaders and vice versa. GLSL does not force the two shaders to match, so it is up to the user to provide matching sets. A painter will accept batches and use the paint method to display and flush them. Each paint is not limited to one draw call. Depending on the painter, each batch can hold the handle to different materials, and depending on how it's sorted, there can be a few or many multiple draw calls because of them.
 
In Elang Engine, materials only hold handles to textures and uniforms and not shaders. This means a material can potentially be used across many different painters with different combination of shaders. This by no means would break anything, but it would be rendered null if the shaders cannot accept the material. A material can hold multiple textures which bind to the shader sampler2d array in the same order as the vector index. Materials also hold uniforms, which the painter automatically parses and relays to either the vertex or fragment uniform of the same name, depending on the uniform state. The interface is intuitive so it shouldn't be too much of an issue. Currently (v0.1.0) uniforms only accept the following data types: float, vec2, vec3, vec4, matrix3x3, and matrix4x4.

The intended architecture is to separate a static painter from a dynamic painter and then lock the static upon first batch. Conversely, a single painter can use Frustrum Culling to only receieve batches that are part of the camera field. 

### Batch Related Elements

While there are many batch related Elements such as Sprites, it is very easy to create a custom element that handles its own draw layer. It only needs enough data to populate a Batch class and a target Vertex type. There are common Vertex types in "common/vertex.h" but it is easy to create custom types.

### Shape Painters

A ShapePainter instance is a specialized debug class which is actually composed of 3 different debug painters with debug shaders. Together they make debug shapes (currently only in ed, v0.1.0.) Debug painters are only set in its state and only receives a camera to detect where it should draw. Debug batches are created during batch and destroyed during paint flush so they do not eat up RAM when they are not in use. (Organization maybe necessary, v0.1.0)

