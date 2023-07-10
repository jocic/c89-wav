#include <stdio.h>
#include <assert.h>

#include "tests/binary-io.h"

/**
 * - Add unit tests - covering everything
 * - Alter the code removing use of any data types larger than a single byte
 * - Replace "get/set" functions with macro functions
 * - Buffered sample read functions - not sample by sample but 64 128, etc
 * - Document everything
 * - Update readme file including examples
 * - Use error codes
 * - Release version 1.0.0
 */

int main(int argc, char *argv[]) {
    
    test_bin_io();
    test_wav_format();
    
    return 0;
}
