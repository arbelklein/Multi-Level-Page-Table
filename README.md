# Multi-Level-Page-Table

## Description
This repository contains a simulated operating system code for handling multi-level (trie-based) page tables. The project implement two key functions:
1. creating/destroying virtual memory mappings in a page table
2. querying if an address is mapped in a page table
The code simulates OS functionality and targets an imaginary 64-bit x86-like CPU.

## Usage
1. Compile the code using `gcc`:
    ```sh
    gcc -O3 -Wall -std=c11 os.c pt.c -o page_table_sim
    ```
2. Run the compiled program:
    ```sh
    ./page_table_sim
    ```
