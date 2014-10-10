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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include <Objectively/Class.h>
#include <Objectively/Object.h>

static Class *__classes;

/**
 * @brief Called `atexit` to teardown Objectively.
 */
static void teardown(void) {
	Class *c;

	c = __classes;
	while (c) {
		if (c->destroy) {
			c->destroy(c);
		}
		c = c->locals.next;
	}

	c = __classes;
	while (c) {
		if (c->interface) {
			free(c->interface);
		}
		c = c->locals.next;
	}

	pthread_exit(NULL);
}

/**
 * @brief Called when initializing `Object` to setup Objectively.
 */
static void setup(void) {

	atexit(teardown);
}

/**
 * @brief Initializes the class by setting up its magic and archetype.
 */
static void initialize(Class *clazz) {

	assert(clazz);

	if (__sync_val_compare_and_swap(&clazz->locals.magic, 0, -1) == 0) {

		assert(clazz->name);
		assert(clazz->instanceSize);
		assert(clazz->interfaceSize);

		clazz->interface = calloc(1, clazz->interfaceSize);
		assert(clazz->interface);

		Class *super = clazz->superclass;

		if (clazz == (Class *) &__Object) {
			assert(super == NULL);
			setup();
		} else {
			assert(super != NULL);

			assert(super->instanceSize <= clazz->instanceSize);
			assert(super->interfaceSize <= clazz->interfaceSize);

			initialize(super);

			memcpy(clazz->interface, super->interface, super->interfaceSize);
		}

		clazz->initialize(clazz);

		clazz->locals.next = __sync_lock_test_and_set(&__classes, clazz);
		clazz->locals.magic = CLASS_MAGIC;

	} else {
		while (__sync_fetch_and_or(&clazz->locals.magic, 0) != CLASS_MAGIC) {
			;
		}
	}
}

id __new(Class *clazz, ...) {

	initialize(clazz);

	id obj = calloc(1, clazz->instanceSize);
	if (obj) {

		((Object *) obj)->clazz = clazz;
		((Object *) obj)->referenceCount = 1;

		id interface = clazz->interface;

		va_list args;
		va_start(args, clazz);

		obj = ((ObjectInterface *) interface)->init(obj, interface, &args);

		va_end(args);
	}

	return obj;
}

id __cast(Class *clazz, const id obj) {

	initialize(clazz);

	if (obj) {

		Object *object = (Object *) obj;
		if (object->clazz) {

			const Class *c = object->clazz;
			while (c) {

				assert(c->locals.magic == CLASS_MAGIC);

				// as a special case, we optimize for __Object

				if (c == clazz || clazz == (Class *) &__Object) {
					break;
				}

				c = c->superclass;
			}
			assert(c);
		}
	}

	return (id) obj;
}

void release(id obj) {

	Object *object = cast(Object, obj);

	assert(object);

	if (__sync_add_and_fetch(&object->referenceCount, -1) == 0) {
		$(object, dealloc);
	}
}

void retain(id obj) {

	Object *object = cast(Object, obj);

	assert(object);

	__sync_add_and_fetch(&object->referenceCount, 1);
}