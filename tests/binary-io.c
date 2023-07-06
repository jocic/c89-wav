#include <stdio.h>
#include <assert.h>

#include "bin.h"
#include "binary-io.h"

void test_bin_open_close() {
    
    printf("TEST: Binary I/O -> File Open/Close\n");
    
    BIN_FILE file = bin_open("test-files/60hz-5s-16bit-signed-44100.wav", O_RDONLY);
    assert(file.open && "File not open.");
    
    bin_close(&file);
    assert(!file.open && "File not closed.");
}

void test_bin_io() {
    
    test_bin_open_close();
}
