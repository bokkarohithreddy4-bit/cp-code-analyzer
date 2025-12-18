# CP Analyzer

CP Analyzer is a small C++ command-line tool to analyze competitive programming solutions for basic code-quality signals.

It takes a `.cpp` file as input and prints simple metrics about the code, along with warnings for long and deeply nested functions.

## Features

- Reads a C++ source file (`.cpp`) and reports:
  - Total number of lines in the file.
  - Number of lines containing loops (`for` / `while`) as a rough complexity hint.
  - Functions detected (using very naive parsing) with:
    - Function name
    - Line range (start and end lines)
    - Function length in lines (including header and body)
    - Maximum nesting depth inside the function (based on `{` and `}`).

- Prints warnings when:
  - A function is longer than 60 lines.
  - A function has nesting depth greater than 3.

The goal is not to be a perfect parser, but to give a quick signal when a competitive programming solution is getting too long or too deeply nested.

g++ -std=gnu++17 main.cpp -o cp-analyzer
This will create an executable named `cp-analyzer` in the same folder.

## Usage

Run the tool on any `.cpp` file:

./cp-analyzer your_solution.cpp

Example:

./cp-analyzer main.cpp

The program will print:

- The total number of lines.
- How many lines contain `for` or `while`.
- A list of detected functions with their lengths and maximum nesting depth.
- Warnings for long or deeply nested functions.

## Limitations and Future Work

- Function detection is naive and based on simple patterns; it may miss some functions or mis-detect some blocks.
- It does not understand full C++ syntax or templates.
- In the future, it could be extended with:
  - More checks (e.g., number of globals, macros).
  - Configurable thresholds for length and nesting depth.
  - Better parsing using a proper C++ parser or library.

