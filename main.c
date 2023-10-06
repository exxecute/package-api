#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct PACKAGE_API_t
{
    uint8_t buffer[64];
    uint16_t buffer_size;
    uint8_t start_package_byte;
    uint8_t stop_package_byte;
}PACKAGE_API_t;

const uint8_t START_PACKAGE_COMBINATION[] = {0x00, 0x11, 0x22, 0x33};
const uint8_t STOP_PACKAGE_COMBINATION[] = {0xcc, 0xdd, 0xee, 0xff};

const uint8_t START_COMBINATION_SIZE = sizeof(START_PACKAGE_COMBINATION);
const uint8_t STOP_COMBINATION_SIZE = sizeof(STOP_PACKAGE_COMBINATION);

static uint8_t _validate_head(const uint8_t *__buffer, const uint8_t *__head, const uint8_t __head_size)
{
    uint8_t _answer = true;
    for(int byte_index = 0; byte_index < __head_size; byte_index++)
    {
        if(__buffer[byte_index] != __head[byte_index])
        {
            _answer = false;
            break;
        }
    }
    return _answer;
}

void PACKAGE_API_init(PACKAGE_API_t *this, uint8_t* __buffer, uint16_t __buffer_size)
{
    memcpy(this->buffer, __buffer, __buffer_size);
    this->buffer_size = __buffer_size;
}

void PACKAGE_API_find_package(PACKAGE_API_t *this)
{
    uint8_t _is_package = false;
    for(int byte_index = 0; byte_index < this->buffer_size; byte_index++)
    {
        if(_is_package)
        {
            if(_validate_head(this->buffer + byte_index, STOP_PACKAGE_COMBINATION, STOP_COMBINATION_SIZE))
            {
                this->stop_package_byte = byte_index + STOP_COMBINATION_SIZE;
                break;
            }
        }
        else
        {
            if(_validate_head(this->buffer + byte_index, START_PACKAGE_COMBINATION, START_COMBINATION_SIZE))
            {
                this->start_package_byte = byte_index;
                _is_package = true;
            }
        }

    }
}

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
