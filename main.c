#include <stdio.h>
#include <assert.h>

#include "tests/binary-io.h"
#include "tests/wav-format.h"
#include "tests/functionality.h"

int main(int argc, char *argv[]) {
    
    test_bin_io();
    test_wav_format();
    test_functionality();
    
    return 0;
}
