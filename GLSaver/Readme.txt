The GLSaver Project

The GLSaver project is a simple screensaver project which is written in the C langage. The code runs on Windows operating systems, and require certain Windows components to run (SCRNSAVE.LIB, Windows.h). Beyond the dependency on Windows itself, the project has no external dependencies.

As of 17 October 2013 the project will be using bitmap resources for textures, and these are simple raw files. If the format changes, then the utility to generate the new format will be attached to some portion of the code, or within the same repository. The projects aims to create the best looking, most compatible screensaver using the simplest code and the easiest to understand/code approach. The project is intended as a step-ladder for new coders who want to get their hands dirty with OpenGL coding, without having to deal with controls or input handling.

This project serves as a standalone method of learning how algorithms operate as well as becoming familiar with different Windows and OpenGL concepts, without being immersed in the complexities that come with other topics such as Input and Event handling.

Goals:

  To provide a simple OpenGL screensaver which newbies can learn from
  To provide the best looking screensaver of its type.
  To provide a free alternative to commercial savers out there
  
This project uses no "special" libraries, and everything required to compile this code can be accomplished with a default install of the Code::Blocks IDE. The OpenGL libraries are bundled with the IDE, and require no external dependencies apart from those which the operating system provides.

Current details:

  Requires a 16Mb graphics card to run, although software rendering is possible with a strong CPU.
  Requires less than 50KB for the mathematical information (without textures)
  Uses simple trigonometric functions for effects, therefore does not require anything more than high school maths.
  Written in C, so it's as fast as we can get it without resorting to Assembly.
  Uses modular approach to code separation, which simplifies refactoring.
  Most modules keep track of objects internally, so no reference counting is needed when using modules.

Obviously, it would be advantageous to get more programmers involved as I don't have the time to iron out all the details. There are a few things which are needed - such as:
  
  Texture loading code needs improvement.
  Code should be made more generic to encourage proper coding practices.
  Animation effect could use a bit more complexity.
  Better comments could improve readability for some functions.
  Graphic assets are required, and a better file format is needed, without resorting to external libs.
  Configuration dialog and configurable parameters for the screensaver.
  
Technical specs:
  C code.
  OpenGL (no specific version since no extensions are used).
  Written to be C99 compliant.
  Uses no external libs beyond what are bundled with all Windows operating systems.
  Uses simple code to accomplish the task - no optimisations have been added to the code.