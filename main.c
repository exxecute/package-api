#include "package_api.h"

static void _print_massive(uint8_t *__massive, uint8_t __size, char *__name)
{
	printf("%s: ", __name);
	for(int i = 0; i < __size; i++)
	{
		printf("%02X ", __massive[i]);
	}
	printf("\n");
}

const uint8_t test_buffer[] = {0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x00, 0x11, 0x22, 0x33, \
                            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99,\
                            0xcc, 0xdd, 0xee, 0xcc, 0xdd, 0xee, 0xff};
const uint16_t test_size = 27;

int main(void)
{
    PACKAGE_API_t package_api = {0};
    _print_massive(test_buffer, 28, "test buffer");
    _print_massive(package_api.buffer, 64, "buffer before cpy");
    PACKAGE_API_init(&package_api, test_buffer, test_size);
    _print_massive(package_api.buffer, 64, "buffer after cpy");
    PACKAGE_API_find_package(&package_api);
    printf("start byte: %d, stop byte: %d\n", package_api.start_package_byte, package_api.stop_package_byte);
    _print_massive(package_api.buffer + package_api.start_package_byte, package_api.stop_package_byte - package_api.start_package_byte, "package");
    return 1;
}
