# System Programming Project

This repository contains a collection of programs that demonstrate various **system programming** concepts and techniques. The project is intended to provide hands-on experience with low-level programming tasks, such as process management, memory allocation, file handling, threading, and inter-process communication (IPC).

## Table of Contents

1. [Overview](#overview)
2. [Technologies Used](#technologies-used)
3. [Key Features](#key-features)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Project Structure](#project-structure)
7. [License](#license)

## Overview

**System programming** involves writing code that interacts closely with the operating system, handling tasks that require direct management of hardware and system resources. This project explores the following core concepts:

- Process creation and management
- Memory allocation and management
- File I/O and system calls
- Threading and synchronization
- Signals and signal handling
- Inter-process communication (pipes, message queues, shared memory)
- Network programming using sockets

This project is designed to provide a deep understanding of how system-level programming works on platforms like **Linux/Unix**.

## Technologies Used

- **C/C++**: The primary programming languages used in this project.
- **POSIX API**: Used for interacting with the operating system's kernel to manage processes, threads, and communication.
- **Linux/Unix**: The target platform for the system programming examples.
- **GCC/G++**: The compiler used for building the C/C++ code.

## Key Features

1. **Process Management**:
   - Demonstrates how to create and manage child processes using `fork()`, and how to wait for process termination with `wait()`.
   - Example of executing other programs within a process using `exec()` family functions.

2. **Memory Management**:
   - Explains dynamic memory allocation using `malloc()`, `calloc()`, and `realloc()`.
   - Demonstrates stack and heap memory management techniques.
   - Shows how to use `mmap()` for memory-mapped file I/O and shared memory.

3. **File I/O**:
   - Low-level file manipulation using system calls such as `open()`, `read()`, `write()`, and `close()`.
   - File descriptors and file pointer management, including buffered and unbuffered I/O.
   - Demonstrates reading/writing binary and text files.

4. **Threading and Synchronization**:
   - Introduction to multi-threading with **POSIX threads** (pthreads).
   - Synchronization using mutexes, condition variables, and semaphores.
   - Explains race conditions and critical section handling in a multi-threaded environment.

5. **Signals**:
   - Demonstrates signal handling with `signal()` and `sigaction()`.
   - Example of catching and handling system signals (e.g., `SIGINT`, `SIGKILL`, `SIGCHLD`).

6. **Inter-Process Communication (IPC)**:
   - Demonstrates communication between processes using:
     - **Pipes**: Named and unnamed pipes for data transfer between processes.
     - **Message Queues**: Implementing message passing between processes.
     - **Shared Memory**: Using `shmget()` and `shmat()` to allocate and access shared memory between processes.
     - **Semaphores**: Controlling access to shared resources in concurrent processes.

7. **Network Programming**:
   - Demonstrates socket programming, including:
     - Creating server and client applications using TCP and UDP protocols.
     - Handling connections and data transmission over a network.
     - Asynchronous I/O and multiplexing with `select()` and `poll()`.

## Installation

To run this project on your local machine, follow these steps:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/system-programming.git
   cd system-programming
