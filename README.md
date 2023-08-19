# maman_14

## Description
This project is a compiler for assembly language to binary code.

## Usage
```
make
./assembler <file_name>.as ... <file_name>.as
```
for example:
```
./assembler ./test_files/test_3.as ./test_files/test_2.as ./test_files/test_1.as 
```
## CI
The is a CI for this project to check the build and run the tests, you can find in `.github/workflows`

## Potential Problems
If the assembler is not working, try to run the following command:
```
rm -f *.o
rm ./assembler
make
```