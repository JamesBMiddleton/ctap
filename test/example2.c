#include "../utils/types.h"
#include "../src/ctap_api.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    ctp_api.load_map();
    printf("example_test!");
	return 0;
}
