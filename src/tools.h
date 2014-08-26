#ifdef __cplusplus
extern "C"{
#endif

#ifndef TOOLS_INCLUDE_H
#define TOOLS_INCLUDE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "log/log.h"

int16_t unhex(uint8_t c);
uint32_t hex2bin(uint8_t *hex, uint8_t *bin);

#endif /* TOOLS_INCLUDE_H */

#ifdef __cplusplus
}
#endif

