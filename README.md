# Elang Game Engine (v0.1.0)

Elang Game Engine is a work-in-progress generic 2d game engine. It's mostly being crafted for programming experience, but also to make a lightweight alternative to common game engines, providing some 2d game design mechanics that are not readily available in the market. While it is a game engine, the elements can be used to make any sort of application. It is also used (with the help of QT) to make the GUI for the game engine.

Common files are custom scripts with no external dependencies - none outside the common folder - except for the std library. 

Tools are heavily reliant on dependencies and/or specifically designed around a true Entity Component System (ECS), which is not only a unique software architectural pattern but also helps immensely with caching and therefore performance. 

Elements are essentially an Elang alias for components in ECS that are meant to be attached to assets or objects. 

Apparatus are mostly logic-only modules with common inter-elemental interactions, helping with code organization and compile time. 
The level of dependency is ( common < tools < elements < apparatus < engine ).

Almost the entire source heavily depends upon ECS design which is very popular in game programming. The engine relies exclusively upon the ENTT library to manage this. The problem is that any access to the registry completely lays bare every single component attached to the registry. On top of that, the internal framework often refers to a couple of global references, specifically to the project and "current stage", which the user is expected to bind manually. 

This means that encapsulation is not the engine's highest priority. That doesn't mean it was ignored completely, but it was not pursued at the expense of convenience, readability, and optimization. The above potentially makes every asset/stage objects global, inviting many problems usually associated with it. So far it has not been as dangerous as it sounds and the work flow is very much like working with glew, which is also mostly global bindings. As of now, users are discouraged from creating and deleting asset entities manually. You have the AssetLoader apparatus to do that for you. 


# Intended Usage

The initial setup begins with creating a game class which inherits the engine class and overriding its virtual methods. 

## Loading

The term sync refers to an asset entity matching a ROM file's path and metadata without actually reading its data into RAM.
Creation and erasure of the ROM file should be same as that of this asset entity, and vice versa (provided they are synced properly)
Modification (on either side) is synced only when the asset is already imported.

The terms load/import/open are used interchangibly to refer to asset entity actually opening its ROM file and importing it to RAM.
The terms unload/close are used interchangilby to refer to an asset entity deleting all its RAM data (not touching the ROM).
The terms save/export are used interchangibly to refer to an asset entity creating or modifying a ROM file with its RAM data.

First create an AssetLoader object and using its syncAllAssets method. If you are to create, modify, or erase an asset externally then you may have to call this more than once, preferably every time you refocus on your application/game. Synced assets can be referenced in the asset database of a matching category (gTextures for texture, etc.). Asset database are global at the moment.

If you somehow need to sync a custom external file with your own extension, use the individual sync.syncAsset method. You need to pass a template parameter of your asset type and the meta data of the asset (NullData if none). The function parameters take the file path and an asset database of your choosing. You can choose an existing database or pass a custom-created one. There's no "error" involved if you choose a wrong asset database but it can create confusion if you're not careful.

Synced assets are not yet loaded however, and depending on your needs you can manually use the openAll function or import each necessary assset manually via sync.open. The template and function parameters are the same as synd.syncAsset. You need to pass the correct asset database you used to sync the asset, otherwise you may experience unwanted behavior.

If this is too complicated, simply syncAllAssets then sync.openAll. 

### Alternative: Snapshots

Another way to load is via snapshots (which is not yet implemented in v(0.4.0)). This snapshot is a single binary data containing almost everytihng of a project and its stages, including but not excluded to shaders, texture, atlas, stages, sprites, buttons, position, transforms, etc. It does not have to parse individiual text/binary files and keeps entity indices alive such that the objects do not have to be re-referenced in any way - a sprite for example is serialized already containing the entity that directly refers to the texture. In other words, especially for very small games, this can reduce load time immensely. However due to its lack of portability and little to no support for backwards compatibility, it is not a very stable method. So this way is only reserved for making final builds. 

## Game Logic

The game logic consists of overriding the start, input, and update method. 

The start method is the initialization phase. Technically, this method isn't necessary; all of it can work just as fine in the load method. However, the load method was reserved for syncing project files and loading necessary imports in the Asset Database. The start method is used to either create asset/objects manually and/or link existing assets/objects together.

### Key and Mouse input

The input method is where all the input actions are recommended. This means all functions and methods which work with the gKey and gMouse instance. They internally store the state of input of every key and clicks. Below is the grammar for inputs. I use my own lingo for the states, which may seem cryptic at first, but it's easy to get used to in no time. 

> if (gKey.state(eKeyCode::A) == eInput::ONCE) // Only true once the moment A is pressed
> if (gMouse.state(0) == eInput::HOLD) // True as long as left mouse is held
> if (gMouse.state(1) == eInput::LIFT) // Only true once the moment right mouse is released.

Other built-in functions such as those in apparatus/ui.h can also belong here, such as the zoomCamera function or moveWASDCamera function for example.

## Batch & Paint

The paint method deals with graphics. Currently the Elang Engine only supports the OpenGL pipeline. It will be plenty long before other graphics APIs such as DirectX are employed. The interface will hopefully remain unchanged should this happen.

The painters and materials are responsible for managing graphic data. Painters act intuitvely like draw layers, which state can be modified to accomodate many individual needs such as Z-buffer usage, sorting, and/or static batching (by locking the layer). It also has some common uniforms like color (the vertex shader must have a matching uniform name, however). Unlike other game engines, the "materials" do not hold reference to shaders. The painters deal directly with shaders, and accepts only batches with matching vertex size. This means it could potentially accept unfit vertices, but at the very least it will not break other batches.

Painters hold reference to one camera and one shader program pipeline, which in turn is constructed by one vertex shader and one fragment shader. This means a fragment shader can be reused for multiple vertex shaders and vice versa. GLSL does not force the two shaders to match, so it is up to the user to provide matching sets. A painter will accept batches and use the paint method to display and flush them. Each paint is not limited to one draw call. Depending on the painter, each batch can hold the handle to different materials, and depending on how it's sorted, there can be a few or many multiple draw calls because of them.
 
In Elang Engine, materials only hold handles to textures and uniforms and not shaders. This means a material can potentially be used across many different painters with different combination of shaders. A material can hold multiple textures which bind to the shader sampler2d array in the same order as the vector index. Materials also hold uniforms, which the painter automatically parses and relays to either the vertex or fragment uniform of the same name, depending on the uniform state. The interface is intuitive so it shouldn't be too much of an issue. Currently (v0.4.1) uniforms only accept the following data types: float, vec2, vec3, vec4, and matrix4x4.

The intended architecture is to separate a static painter from a dynamic painter and then lock the static upon first batch. Conversely, a single painter can use Frustrum Culling to only receieve batches that are part of the camera field. 

#### Batch Related Elements

While there are many batch related Elements such as Sprites, it is very easy to create a custom element that handles its own draw layer. It only needs enough data to populate a Batch class and a target Vertex type. There are common Vertex types in "common/vertex.h" but it is easy to create custom types.

#### Shape Painters

A ShapePainter instance is a specialized debug class which is actually composed of 3 different debug painters with debug shaders. Together they make debug shapes (currently only in ed, v0.1.0.) Debug painters are only set in its state and only receives a camera to detect where it should draw. Debug batches are created during batch and destroyed during paint flush so they do not eat up RAM when they are not in use. (Organization maybe necessary, v0.1.0)

