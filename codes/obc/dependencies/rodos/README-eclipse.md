#How to create a RODOS-Application with Eclipse

1. Download and install Eclipse for C/C++ Developers

2. Create the folder of your project

    ```
        mkdir myProject
        cd myProject
    ```

    
3. Run the following command

    ```
        git clone https://gitlab.com/rodos/rodos.git  
    ```

    
7. Create your first .cpp-file, e. g. myHello.cpp

8. Create CMakeLists.txt in the project folder with the following lines:

    ```
        add_subdirectory(rodos)
        add_rodos_executable(myhello myHello.cpp)
    ```

    
9. Don't name your application *test*, it will cause problems. For more details about Cmake, read *README-cmake.md*.

10. Create build-folder:

    ```
        mkdir build
        cd build
    ```

11. Run the following command:

    ```
        cmake -DCMAKE_TOOLCHAIN_FILE=../rodos/cmake/port/posix.cmake -DEXECUTABLE=ON ..
    ```

12. Now start Eclipse

13. Get your project into Eclipse by doing the following and choose the folder *myProject*:

    File > New > Makefile Project with Existing Code
    
14. Right-click on your project and click on *Properties*

15. Go to C/C++ build and add /build to the build directory.

16. Build the project.

17. Right-click on your project and navigate to *Run as > Run configurations* 

18. Click on *C/C++-Application* and choose your C/C++-Application with *Browse*. It is located in `/myProject/build` and should by called `myhello`.

19. Now you should be able to run your programme.
