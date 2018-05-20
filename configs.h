#ifndef _CONFIGS_H
#define _CONFIGS_H

#include "configs/chipkit.h"
#include "configs/microchip.h"
#include "configs/majenko.h"

#if !defined(_BOARD_VALID_)
#error No config found for your board
#endif

#endif
