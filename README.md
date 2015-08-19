#Circles
Circles is a program which interacts with circle packings. This program is designed to aid in the exploration
of approximation of Riemann mappings from a polygon to the complex unit disc. 

The program is not currently feature-complete. 

![screenshot](image_window.png "A screenshot of Circles in action, performing a repack of the 'Owl' packing")

Above is a screenshot of Circles in action, performing a repack of the 'owl' packing. 

##Features
- Supports circle packings in both standard euclidean and poincare-disc hyperbolic geometries
- Support reading of p-files generated with [Ken Stephenson's CirclePack tool](http://www.math.utk.edu/~kens/CirclePack/)
- Toggable view of circles, center marks, indicies, tangency lines, and disc boundary
- Properties explorer to show properties of any circle

##Building
This program was built on Windows using QT5.4 **with OpenGL** for MSVC2013, 32 bit. This program may not compile on earlier compilers since it uses many C++11/14 features. 

Compatibility with other QT-compatible compilers is not guaranteed, but should work just fine.

This program is still a work in progress.
