
# Banker's Algorithm Implementation in C++

## Overview

This repository contains a C++ implementation of the **Banker's Algorithm**, a deadlock avoidance algorithm used in operating systems. The program simulates resource allocation to determine if the system is in a "Safe State" (where a sequence of processes exists such that all can finish execution).

Unlike basic implementations, this version allows for **interactive resource requesting**, where the system tentatively grants resources and rolls back if the resulting state is unsafe.

## Features

* **Standard Template Library (STL):** Utilizes `std::vector` for dynamic matrix handling, making the code cleaner and more flexible than static arrays.
* **Safety Algorithm:** Calculates the 'Need' matrix and checks if a valid execution sequence exists based on current Available resources.
* **Resource Request Algorithm:** Allows the user to simulate a specific process requesting additional resources at runtime.
* Checks if the request is valid (within Max Need and Available limits).
* Tentatively allocates resources.
* Runs the Safety Check; if unsafe, the system **rolls back** the allocation.


* **Menu-Driven Interface:** A loop allows users to view matrices, check safety, and make requests multiple times in a single session.

## Code Structure

The implementation is contained in a single C++ file and consists of the following key functions:

* `calcNeed()`: Computes the remaining resource needs (`Max - Allocation`).
* `showMatrix()`: Utility to display Allocation, Max, and Need matrices.
* `checkSafe()`: The core algorithm that attempts to find a "Safe Sequence."
* `requestResource()`: Handles dynamic requests and performs the safety rollback if necessary.

## How to Run

1. **Prerequisites:** Ensure you have a C++ compiler installed (e.g., `g++`).
2. **Compile:**
```sh
g++ banker.cpp -o banker

```


3. **Execute:**
```sh
./banker

```



## Usage & Example Interaction

The program first requires the setup of system parameters (Processes, Resources, and Matrices). Once initialized, it enters a menu loop.

### 1. Initialization Input

*(User enters 5 processes, 3 resources, and the respective matrices)*

```text
Enter number of processes and resources: 5 3

Enter allocation matrix:
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2

Enter max requirement matrix:
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3

Enter available resources:
3 3 2

```

### 2. Menu Interaction

**Option 1: View Matrices**
The program automatically calculates the Need matrix upon startup.

```text
1. Show matrices
2. Check safe state
3. Request resources
4. Exit
Choice: 1

Allocation Matrix
P0: 0 1 0
P1: 2 0 0
...

Max Need Matrix
P0: 7 5 3
...

Need Matrix
P0: 7 4 3
P1: 1 2 2
...

```

**Option 2: Check Safety**
Runs the safety algorithm on the current state.

```text
Choice: 2

System is SAFE.
Safe sequence: P1 P3 P4 P0 P2

```

**Option 3: Request Resources**
Simulate Process 1 requesting resources `1 0 2`.

```text
Choice: 3
Enter process number: 1
Enter resource request: 1 0 2

Request granted.

```

*(Note: If the request leads to an unsafe state, the program will output: `Request denied (unsafe). Rolling back.`)*

## Algorithm Logic

1. **Input:** Number of processes (), Number of resources ().
2. **Matrices:**
* `Allocation[n][m]`: Resources currently held.
* `Max[n][m]`: Maximum resources a process might need.
* `Available[m]`: Resources currently free.
* `Need[n][m]`: Calculated as `Max - Allocation`.


3. **Safety Check:**
* Work = Available.
* Find a process where `Need[i] <= Work` and `Finish[i] == false`.
* If found: `Work += Allocation[i]`, mark `Finish[i] = true`.
* Repeat until all are finished (Safe) or no process can satisfy the condition (Unsafe).