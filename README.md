# <p align="center"> OpenGl Ray Caster <sub> :suspect: </sub> </p>
<p align="center"><sub>Finger Gunz (working title)</sub></p><br>

## Highlights
- OpenGL rendering pipeline abstraction
     - VBO, EBO, shaders, and VAO abstractions for ease of use
- C++, GLSL
  
## ℹ️ Overview
This is a hardware accelerated ray caster I have made with the OpenGL API. This will be the final project for my computer graphics course and also a cool project to have under my belt.

> [!NOTE]
>LIBRARIES:<br>
>  [GLFW](https://www.glfw.org/download) AND
[GLAD](https://github.com/go-gl/glfw/blob/master/v3.1/glfw/glfw/deps/glad/glad.h)
> >
> `THIS WILL ONLY COMPILE AND RUN ON WINDOWS`


### ✍️ Authors
Weston Lane
## 🚀 Look
![gif](https://github.com/Weston-Lane/RayCaster/blob/main/GitHub%20rsc/raytracer.gif)

### ⬇️ Installation
I use a simple build system I created with some batch scripts. These scripts set up and compile the exe **HOWEVER,** this is dependent on the [MSVC](https://visualstudio.microsoft.com/vs/community/) compiler and where it is located on your local machine.
> [!IMPORTANT]
>`MSVC COMPLILER IS REQUIRED FOR THE BUILD SYSTEM TO WORK`
> # **setup.bat**
> the setup.bat initializes the msvc compiler and loads the VS debugger for my workflow.
> all that is really needed is this call
> ````bat
> rem change to your specific MSVC path
> call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
> ````
> then run the command in the main dir
> ````cmd
>  C:\RayCaster> setup

After setup

># **build.bat**
>Once setup.bat is run and MSVC is initialized. the build script will compile and run the program.
>via this command:
> ````cmd
>  C:\RayCaster> build

Or you could just download the binaries via the releases tab.

### Future :construction:
- add a weapon sprite
- Vertex Attribute abstraction
- projectiles
- enemy logic and destruction
- eventually port to webgl and put up on personal website.......



