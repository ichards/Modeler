# Modeler
This is a program that allows the user to create a 3D model. Currently all you can do is add points and make a few faces.

Building:
currently, you just have to compile everything in src and dynamic-arrays with the raylib.dll. For example, building with GCC in the main directory in experimental branch would look like this:
gcc src/\*.c dynamic_arrays/\*.c raylib.dll

the master branch would not have dynamic-arrays, so just ignore that.

![Alt Text](https://github.com/Ichards/Modeler/blob/master/demo.gif)
