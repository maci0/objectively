/*
 * Objectively: Ultra-lightweight object oriented framework for c99.
 * Copyright (C) 2014 Jay Dolan <jay@jaydolan.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef _Objectively_Once_h_
#define _Objectively_Once_h_

#include <Objectively/Types.h>

/**
 * @file
 *
 * @brief Helpers for at-most-once semantics.
 */

/**
 * @brief The Once type.
 */
typedef long long int Once;

/**
 * @brief Executes the given `block` at most one time.
 */
#define DispatchOnce(once, block) \
	if (__sync_val_compare_and_swap(&once, 0, -1) == 0) { \
		block; once = 1; \
	} else { \
		while (once != 1) ; \
	}

#endif
