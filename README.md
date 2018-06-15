# Assembler-20465
A two passes assembler that was written as a final project for the course “System Programming Lab” (20465) at The Open University of Israel.

## Overview
The assembler gets assembly files (.as) as input and generate 3 files for each:

The object file (.ob) which contains the encoding of the assembly code commands and the encoding of the data defined in the assembly file.

The entry file (.ent) which contains the encoding of the variables that are used by other files and their location in memory.

The external file (.ext) which contains the external variables that are used by the assembly code commands and the respective locations of the commands.

The assembly language used in this program is defined in the Project Definition document which can be found in the repository in Hebrew. The project is written in C in the ANSI C standard. The encoding of the output is done in hexadecimal base.


## Usage
To run the assembler, provide the assembly files names (without the ".as" suffix) as parameters and run the program. The following code will run the program on the files "file1.as", "file2.as", "file3.as":

```
./assembler file1 file2 file3
```


## The Program Files 
The program consists of 5 code files and 4 header files:
* **main.c:** Deals with the first pass of the assembler and the general framework of the program including the first pass, the call for the second pass, the creation of the output and the work with the files.
* **table.c:** Deals with constructing and maintaining the main data structures of the program (commandssArray and dataArray)
* **analysis.c:** Consists of functions that are used to analyze the input data and provide useful information for building the data structures of the program.
* **secondPass.c:** Use the data structures built in the first pass to conduct the second pass in which the secondary commands are being built and transformed to binary representation toward the building of the final strings that will be printed to the object file (will be done in the main.c file)
* **binary.c:** Consists of function that are used for transformations between decimal and binary representations and work with the binary strings.

* **utils.h:** Consists of all of the constants used in the program.
* **table.h:** Consists of all of the functions declarations and useful information regards to the first pass files (analysis.c and table.c)
* **binary.h:** Consists of all of the functions declarations and useful information regards to the second pass files (secondPass.c and binary.c)
* **main.h:** Contains the function declarations and needed macros for the main.c file, containing the general framework of the program.

More details about the program can be found in Hebrew in the Project Definition.

## Credit
The project was made in cooperation with **Shmulik Cohen**. (**anuk8**)
