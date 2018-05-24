# *lwirth-lib*

The **_lwirth-lib_** is a general purpose library written in **C++17** by **Luis Wirth**. It includes a game-/simulation-engine and uses the **Vulkan-API**.

## Purpose
The lwirth-lib is general purpose. It is made up of the following separate components: Math, Graphics, Physics, Memory management and Artifical Intelligence. All of this combines to form a **game-engine**.

It also serves the purpose of practice, since it is the first game-engine made by Luis Wirth.

## Components
### Graphics (Vulkan)
**GLFW** is in charge of creating the window and fetching all keyboard and mouse inputs. The responsible class for this is the *lw::Frame*. In the future, the plan is to remove GLFW and instead use only the *lwirth-lib* for these tasks.

As previously stated the library utilizes Vulkan for rendering. For most Vulkan modules there are **wrappers**, e.g *VK::Pipeline* or *VK::Device* to simplify their use. In the core, higher-level classes use these wrappers to form actual **renderers**, e.g. the **_lw::SimpleBrush2D_**.

The **_lw::RenderWindow_** takes care of handling all of these abilities. It is very easy to create an instance of this class and get started on rendering.

### Math
#### Linear Algebra
The *lwirth-lib* supports basic **linear algebra** like **vectors** and **matrices**. These can be of **any dimensions** due to the classes being **templated**. For the *lw::Vec2*, *lw::Vec3* and *lw::Vec4* template **specializations** do exist and provide extra functionality.

#### (Vector-)Geometry
Some basic geometry is also built into the library, which mainly is **vector-geometry**. You can perform many actions on __*lw::Line*s__ and __*lw::Plane*s__ and compute useful data.

## How to get started/Setting up the project

It is recommended to use Visual Studio to build and use the project since it was developed in this very IDE. Further, there have been no tests on other compilers, so there is no guarantee of running on other compilers.

The necessary **dependencies** for the *lwirth-lib* to run are the following:
* [**Vulkan**](https://vulkan.lunarg.com/sdk/home)
* [**GLFW**](http://www.glfw.org/)



## Code example

```C++
#include "lwirth.hpp"

int main()
{
  lw::println("Code example coming soon");
}
```

## Plans for the future
* 3D rendering
* Physics

## How you can help

## Contributors
Up to now the only contributor to this project has been Luis Wirth.
