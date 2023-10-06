#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "package_api.h"

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