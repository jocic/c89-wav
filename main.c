#include <stdio.h>
#include <assert.h>

#include "tests/binary-io.h"

/**
 * - Add unit tests - covering everything
 * - Alter the code removing use of any data types larger than a single byte
 * - Replace "get/set" functions with macro functions
 * - Document everything
 * - Update readme file including examples
 * - Release version 1.0.0
 */

int main(int argc, char *argv[]) {
    
    test_bin_io();
    test_wav_format();
}
