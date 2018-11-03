# *lwirth-lib*

The **_lwirth-lib_** is a general purpose library written in **C++17** by **Luis Wirth**. It includes a game-/simulation-engine and uses the **Vulkan-API**.

## Purpose
The lwirth-lib is general purpose. It is made up of the following separate components: Math, Graphics, Physics, Memory management and Artifical Intelligence. All of this combines to form a **game-engine**.

## Components
### Graphics (Vulkan)
**GLFW** is in charge of creating the window and fetching all keyboard and mouse inputs. The responsible class for this is the *lw::Frame*.

As previously stated the library utilizes Vulkan for rendering. For most Vulkan modules there are **wrappers**, e.g *VK::Pipeline* or *VK::Device* to simplify their use. In the core, higher-level classes use these wrappers to form actual **renderers**, e.g. the **_lw::SimpleBrush2D_**.

The **_lw::RenderWindow_** takes care of handling all of these abilities. It is very easy to create an instance of this class and get started on rendering.

### Math
#### Linear Algebra
As of this time, it is not recommended to use the linear algebra build into the *lwirth-lib* but instead to utilize **Eigen**, which is innately available.

The *lwirth-lib* supports basic **linear algebra** like **vectors** and **matrices**. These can be of **any dimensions** due to the classes being **templated**. For the *lw::Vec2*, *lw::Vec3* and *lw::Vec4* template **specializations** do exist and provide extra functionality.


#### (Vector-)Geometry
Some basic geometry is also built into the library, which mainly is **vector-geometry**. You can perform many actions on __*lw::Line*s__ and __*lw::Plane*s__ and compute useful data.

## How to get started/Setting up the project

The project uses **CMake** therefor you can build the project on any platform. As of now CMake works fine for Windows but not yet for Mac. Mac support is currently work in progress.

All the necessary **dependencies** for the *lwirth-lib* are in the libraries directory. They are automatically included and linked by the CMake-Project. Therefor there is no need to download any third-party dependencies.

Third-party libraries include the following:
* [**Vulkan**](https://vulkan.lunarg.com/sdk/home) as graphics API
* [**GLFW**](http://www.glfw.org/) as window API
* [**Eigen**](http://eigen.tuxfamily.org) as linear algebra API
* [**stb_image**](https://github.com/nothings/stb) as image API



## Code example

```C++
#include "lwirth.hpp"

int main()
{
  lw::println("Code example coming soon");
}
```

## Plans for the future
* MacOS support
* Linux support
* finish CMake setup
* work on 2D-rendering (textures, shaders, more components, ...)
* start 3D-rendering
* implement more math
* implement physics (based on physics studies)
* expand machine learning

## How you can help

## Contributors
Up to now the only contributor to this project has been Luis Wirth.
