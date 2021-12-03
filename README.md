# tomasulo
CPU simulation using Tomasulo's algorithm

## Instructions to compile

### Windows (MSVC w/ Visual Studio)
1. Clone the repository
2. From the directory where the repository is cloned, run:
```
cmake . -G "Visual Studio 16 2019"
```
3. Open the project by double clicking the generated **Tomasolu.sln** file.
4. Click the **Local Windows Debugger** button in the top right to build and run the project.

Note: The project should be run from within the VS enviornment. Double clicking the exe file from the **Debug\** directory currently does not work.

### Mac (clang with LLBD)
1. Clone the repository
2. From the directory where the repository is cloned, run:
```
cmake . -G "Unix Makefiles"
```
3. Run **make** to build the project
4. Project should currently be attatched to a debugger. Run **lldb** followed by **run** from within the debugger to run the project


### Linux (untested) (g++)
1. Clone the repository
2. From the directory where the repository is cloned, run:
```
cmake . -G "Unix Makefiles"
```
3. Run **make** to build the project
4. Run **./Tomasulo** to run the project.
Note: We did not test yet on Linux. I would presume you have to attach to a debugger to run the project successfully as well on a Linux system. This will be fixed once we are feature complete.
