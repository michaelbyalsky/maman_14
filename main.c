#include "pre_assembler.h"

int main(int argc, char* argv[]) {
    const char* filename = argv[1];
    const char* outputFilename = "test_files/after_pre.as";
    pre_assemble(filename, outputFilename);

    const char* filename2 = "test_files/after_pre.as";

    return 0;
}