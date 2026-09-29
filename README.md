# Decoding-elf-RISC-v-to-assembly

```
# ELF Instruction Decoder

A C++ instruction decoder designed to parse real Executable and Linkable Format (ELF) binary files and print decoded instructions.

## Overview

This project provides a robust C++ implementation for parsing and decoding instruction sets from ELF binaries. It features a complete development environment using standard `Makefiles`, comprehensive testing via Google Test and LLVM LIT, and an automated CI/CD pipeline for build and test stages.

## Key Features

- **ELF File Parsing:** Ingests real ELF binary files and decodes raw bytes into readable instructions.
- **C++ Development Workflow:** Built using modern C++ and standard `Makefiles`.
- **Unit Testing:** Integrated with **Google Test** framework for component-level verification.
- **Regression Testing:** Automated regression testing suite powered by LLVM **LIT** (LLVM Integrated Tester).
- **CI/CD Support:** Pipeline integration for continuous integration supporting automated **build** and **test** execution stages.

## Repository Structure

```text
.
├── Makefile              # Build rules and targets
├── src/                  # C++ decoder source files
├── include/              # Header files
├── tests/
│   ├── unit/             # Google Test unit tests
│   └── regression/       # LIT regression test suite
└── .github/workflows/    # CI/CD configuration (Build &amp; Test stages)

```

## Prerequisites

Ensure you have the following tools installed:

* **C++ Compiler:** `g++` or `clang++` (supporting C++17 or later)
* **Build Tool:** `make`
* **Unit Test Framework:** [Google Test](https://www.google.com/url?sa=E&amp;q=https%3A%2F%2Fgithub.com%2Fgoogle%2Fgoogletest)
* **Regression Testing Tool:** [LLVM LIT](https://www.google.com/url?sa=E&amp;q=https%3A%2F%2Fllvm.org%2Fdocs%2FCommandGuide%2Flit.html) (`pip install lit`)

## Getting Started

### 1\. Clone the Repository

```
git clone https://github.com/your-username/elf-decoder.git
cd elf-decoder

```

### 2\. Build the Decoder

Compile the decoder binary using the provided `Makefile`:

```
make

```

### 3\. Usage

Run the decoder executable by providing the path to a target ELF binary:

```
./decoder path/to/binary.elf

```

## Testing

### Running Unit Tests (Google Test)

Execute component unit tests using Google Test:

```
make test-unit

```

### Running Regression Tests (LIT)

Execute end-to-end regression tests using LIT:

```
lit tests/regression

```

### Running All Tests

To execute both unit and regression suites:

```
make test

```

## Continuous Integration (CI/CD)

The project includes continuous integration workflows that automatically trigger on push and pull requests:

1. **Build Stage:** Compiles the C++ source code using `make`.
2. **Test Stage:** Runs Google Test unit tests and LIT regression tests to prevent regressions.
