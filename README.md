# Tomasulo's Algorithm
## CPU simulation using Tomasulo's algorithm.

## Overview
Modern CPUs face challenges where certain instructions, despite appearing to execute sequentially, exhibit significant differences in execution times. For instance, a simple arithmetic logic unit (ALU) addition may take a few cycles, while fetching data from memory on a cache miss can take hundreds of cycles. Although these instructions seem to execute in order from an external perspective (ie the kernel), many contemporary processors support parallel execution through techniques like the Tomasulo algorithm.

The Tomasulo algorithm represents a hardware-based solution enabling out-of-order execution of instructions. It essentially constructs a hardware-level dependency graph, allowing new instructions to proceed through the CPU pipeline based on their data dependencies. This clever approach offers optimizations, but a notable challenge arises with branch instructions, often encountered in loops within high-level programming languages. Branch instructions introduce data dependencies that may block other code paths, potentially diminishing the performance gains achieved by Tomasulo's algorithm.

To address this challenge, a branch predictor is employed. In this project, the branch predictor acts as a state machine, retaining information about whether a specific branch was last taken. By prefetching and executing instructions before determining the branch outcome, this predictor enhances execution speed. It's important to note potential hardware vulnerabilities associated with techniques like branch prediction, including security concerns such as [Meltdown and Spectre attacks](https://meltdownattack.com/), which involve manipulating CPU caches.

This project explores the intricacies of implementing the Tomasulo algorithm, incorporating features such as register renaming, reorder buffer (ROB), speculative execution with branch prediction and misprediction handling, and advanced memory disambiguation techniques. The processor architecture involves five stages: ISSUE, EX, MEM, WB, and COMMIT, with critical components including an instruction buffer, register files, register aliasing table (RAT), reservation stations, a common data bus (CDB), a branch predictor, and a load/store queue.

## Project speifics
The project entails the implementation of the Tomasulo algorithm, a hardware-driven approach enabling out-of-order execution of instructions to enhance CPU performance. The algorithm incorporates crucial features such as register renaming, a reorder buffer (ROB), speculative execution with branch prediction and misprediction handling, and advanced memory disambiguation techniques.

The processor configuration encompasses five distinct stages: ISSUE, EX, MEM, WB, and COMMIT. Integral components of the processor architecture include an instruction buffer, register files (comprising integer and floating-point registers), a register aliasing table (RAT), dedicated reservation stations for various functional units, a common data bus (CDB), a branch predictor (utilizing an eight-entry BTB for target storage), and a load/store queue.

The load/store queue plays a pivotal role, serving as reservation stations for the memory unit. Notably, it handles both loads and stores, with stores writing into memory during the commit stage. Load operations can proceed to memory once the address is calculated, with the load/store queue managing forwarding from stores. The branch unit employs a one-bit predictor for each branch instruction, facilitating efficient branch resolution and mitigating the impact on performance.

## Instructions to compile

### Windows (MSVC w/ Visual Studio)
1. Clone the repository
2. From the directory where the repository is cloned, run:
```
cmake . -G "Visual Studio 16 2019"
```
3. Open the project by double clicking the generated **Tomasulo.sln** file.
4. Click the **Local Windows Debugger** button in the top right to build and run the project.

Note: The project should be run from within the VS enviornment. Double clicking the exe file from the **Debug\\** directory currently does not work.

### Mac (clang with LLBD)
1. Clone the repository
2. From the directory where the repository is cloned, run:
```
cmake . -G "Unix Makefiles"
```
3. Run **make** to build the project
4. Project should currently be attatched to a debugger. Run **lldb** followed by **run** from within the debugger to run the project


### Linux (untested, g++)
1. Clone the repository
2. From the directory where the repository is cloned, run:
```
cmake . -G "Unix Makefiles"
```
3. Run **make** to build the project
4. Run **./Tomasulo** to run the project.
Note: We did not test yet on Linux. I would presume you have to attach to a debugger to run the project successfully as well on a Linux system. This will be fixed once we are feature complete.
