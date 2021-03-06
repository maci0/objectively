/*
 * Objectively: Ultra-lightweight object oriented framework for GNU C.
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

#ifndef _Objectively_Array_h_
#define _Objectively_Array_h_

#include <Objectively/Object.h>

/**
 * @file
 *
 * @brief Immutable arrays.
 */

/**
 * @defgroup Collections Collections
 *
 * @brief Abstract data types for aggregating Objects.
 */

typedef struct Array Array;
typedef struct ArrayInterface ArrayInterface;

/**
 * @brief A function pointer for Array enumeration (iteration).
 *
 * @param array The Array.
 * @param obj The Object for the current iteration.
 * @param data User data.
 *
 * @return See the documentation for the enumeration methods.
 */
typedef BOOL (*ArrayEnumerator)(const Array *array, id obj, id data);

/**
 * @brief Immutable arrays.
 *
 * @extends Object
 *
 * @ingroup Collections
 */
struct Array {

	/**
	 * @brief The parent.
	 *
	 * @private
	 */
	Object object;

	/**
	 * @brief The typed interface.
	 *
	 * @private
	 */
	ArrayInterface *interface;

	/**
	 * @brief The count of elements.
	 */
	size_t count;

	/**
	 * @brief The elements.
	 *
	 * @private
	 */
	id *elements;
};

typedef struct MutableArray MutableArray;

/**
 * @brief The Array interface.
 */
struct ArrayInterface {

	/**
	 * @brief The parent interface.
	 */
	ObjectInterface objectInterface;

	/**
	 * @brief Returns a new Array containing the contents of `array`.
	 *
	 * @param array An Array.
	 *
	 * @return The new Array, or `NULL` on error.
	 *
	 * @relates Array
	 */
	Array *(*arrayWithArray)(const Array *array);

	/**
	 * @brief Returns a new Array containing the given Objects.
	 *
	 * @param obj The first in a `NULL`-terminated sequence of Objects.
	 *
	 * @return The new Array, or `NULL` on error.
	 *
	 * @relates Array
	 */
	Array *(*arrayWithObjects)(id obj, ...);

	/**
	 * @return `YES` if this Array contains the given Object, `NO` otherwise.
	 *
	 * @relates Array
	 */
	BOOL (*containsObject)(const Array *self, const id obj);

	/**
	 * @brief Enumerate the elements of this Array with the given function.
	 *
	 * @param enumerator The enumerator function.
	 * @param data User data.
	 *
	 * @remark The enumerator should return `YES` to break the iteration.
	 *
	 * @relates Array
	 */
	void (*enumerateObjects)(const Array *self, ArrayEnumerator enumerator, id data);

	/**
	 * @brief Creates a new Array with elements that pass the filter function.
	 *
	 * @param enumerator The enumerator function.
	 * @param data User data.
	 *
	 * @return The new, filtered Array.
	 *
	 * @relates Array
	 */
	Array *(*filterObjects)(const Array *self, ArrayEnumerator enumerator, id data);

	/**
	 * @return The index of the given Object, or `-1` if not found.
	 *
	 * @relates Array
	 */
	int (*indexOfObject)(const Array *self, const id obj);

	/**
	 * @brief Initializes this Array to contain the Objects in `array`.
	 *
	 * @param array An Array.
	 *
	 * @return The initialized Array, or `NULL` on error.
	 *
	 * @relates Array
	 */
	Array *(*initWithArray)(Array *self, const Array *array);

	/**
	 * @brief Initializes this Array with the specified objects.
	 *
	 * @return The initialized Array, or `NULL` on error.
	 *
	 * @relates Array
	 */
	Array *(*initWithObjects)(Array *self, ...);

	/**
	 * @return A MutableArray with the contents of this Array.
	 *
	 * @relates Array
	 */
	MutableArray *(*mutableCopy)(const Array *self);

	/**
	 * @param index The index of the desired Object.
	 *
	 * @return The Object at the specified index.
	 *
	 * @relates Array
	 */
	id (*objectAtIndex)(const Array *self, const int index);
};

/**
 * @brief The Array Class.
 */
extern Class _Array;

#endif
