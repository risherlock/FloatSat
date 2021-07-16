How to build RODOS with CMake
=============================

CMake is a meta build-system suitable for large projects where build steps are
configured declaratevly in CMakeLists.txt. CMake can automatically generate a
Makefile from this build configuration. Afterwards, make is used to compile the
project. The following text explains the required steps to build rodos projects
with CMake.

RODOS supports different target platforms, which require different compilation configuration.
The configuration for all the ports is encapsulated in separate cmake files at `cmake/port`.
There, port configuration files such as `posix.cmake`, `linux-x86.cmake`, and `stm32f4.cmake` exist.
In order to enable a port, we make use of a special feature of CMake: the [`CMAKE_TOOLCHAIN_FILE` variable](https://cmake.org/cmake/help/v3.15/variable/CMAKE_TOOLCHAIN_FILE.html).
If this variable is set on the command line when `cmake` is called, the content of the supplied file is executed before the main `CMakeLists.txt` is processed.

Generate Make Files
-------------------

```shell script
$ mkdir build
$ cd build
# if on x86:
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/port/linux-x86.cmake ..
# other linux based platforms:
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/port/posix.cmake ..
```

First, create a folder, where all the make files and the compiled files should be stored.
If you want to use multiple ports at once, it is sensible to create separate folders for them, e.g. `build-linux` and `build-stm32f4`.
When calling cmake, the appropriate port `.cmake` file must be passed via the variable `CMAKE_TOOLCHAIN_FILE`.

Compile RODOS
-------------
```shell script
# make sure to be in the build directory
$ make -j rodos
```

This commands builds the RODOS library, which is then placed at `build/librodos.a`

Compile RODOS' Support Lib
--------------------------
The support-libs are included automatically when compiling rodos with CMake.

Compile Tutorials
-----------------
For some of the tutorials, there are ready compilation targets in CMake.
In order to enable them, you must set the `EXECUTABLE` variable to `ON`: 

```shell script
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/port/posix.cmake -DEXECUTABLE=ON ..
```

After that, several new targets are available, which can be compiled by using their names, e.g.

```shell script
$ make -j helloworld
```

The compiled executables are then available under `build/tutorials`.

Create Own Application Using CMake
----------------------------------

Instead of installing RODOS globally to the computer, we encourage users to add it as a submodule/subfolder to the user project.
If you are using git, you can add it directly as submodule just like this:
```shell script
git submodule add https://gitlab.com/rodos/rodos.git
```
Otherwise, if you do not use git, you can simply clone (or manually copy) it into a subfolder:
```shell script
git clone https://gitlab.com/rodos/rodos.git
```
In each case, you need to make RODOS available to your application's CMake by adding it as a subdirectory:
```cmake
add_subdirectory(rodos)
```
(Assuming that your CMakeLists.txt and RODOS are in the root directory of your project.)

There are two ways of compiling an own application that makes use of RODOS: either simply create an own CMake target as usual and link RODOS and the support lib if needed, or use our predefined simple macro, does these steps in only one call.

```cmake
add_subdirectory(rodos)
add_rodos_executable(my-application my-source.cpp)
```

does the same thing as

```cmake
add_subdirectory(rodos)
add_executable(my-application my-source.cpp)
target_link_libraries(my-application PUBLIC rodos support-lib)
```

After those lines you can use all the features of CMake. To build the application, it is recommended to create a build-direcotry. Applications can then be built from the directory where your CMakeLists.txt is with:

```shell script
# optionally create the build directory
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../rodos/cmake/port/posix.cmake -DEXECUTABLE=ON ..
make all
```

Your executable should now show up in `ls -l`. If it is called my-application, it can be run with `./my-application`.
