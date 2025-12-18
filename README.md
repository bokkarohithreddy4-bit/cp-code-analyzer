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


