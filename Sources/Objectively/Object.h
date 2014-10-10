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

#ifndef _Objectively_Object_h_
#define _Objectively_Object_h_

#include <Objectively/Class.h>
#include <Objectively/Types.h>

/**
 * @file
 *
 * @brief Object is the root instance type. Every subclass of Object must
 * declare a structure that begins with `Object object;`.
 */

typedef struct ObjectInterface ObjectInterface;
typedef struct Object Object;

/**
 * @brief The Object type.
 */
struct Object {

	/**
	 * @brief Every instance of Object begins with a pointer to its Class.
	 */
	const Class *clazz;

	/**
	 * @brief The typed interface.
	 */
	const ObjectInterface *interface;

	/**
	 * @brief The reference count of this Object.
	 */
	unsigned referenceCount;
};

typedef struct String String;

/**
 * @brief The Object interface.
 */
struct ObjectInterface {

	/**
	 * @brief Creates a shallow copy of this Object.
	 *
	 * @param self The instance.
	 *
	 * @return The copy.
	 */
	Object *(*copy)(const Object *self);

	/**
	 * @brief Frees all resources held by this Object.
	 */
	void (*dealloc)(Object *self);

	/**
	 * @return A brief description of this Object.
	 */
	String *(*description)(const Object *self);

	/**
	 * @return An integer hash for use in hash tables, etc.
	 */
	int (*hash)(const Object *self);

	/**
	 * @brief The instance initializer (required).
	 *
	 * @details This method typically begins with a call to the superclass
	 * initializer, passing the arguments list up the initializer chain. Method
	 * overrides as well as method and member assignment and initialization
	 * then follow.
	 *
	 * @param obj The newly allocated instance.
	 * @param interface The interface handle.
	 * @param args The initializer arguments list.
	 *
	 * @return The initialized instance, or the unmodified pointer on error.
	 */
	Object *(*init)(id obj, id interface, va_list *args);

	/**
	 * @brief Tests equality of the other Object.
	 *
	 * @return YES if other is deemed equal, NO otherwise.
	 */
	BOOL (*isEqual)(const Object *self, const Object *other);

	/**
	 * @brief Tests for class hierarchy membership.
	 *
	 * @return YES if this instance belongs to class' hierarchy, NO otherwise.
	 */
	BOOL (*isKindOfClass)(const Object *self, const Class *clazz);
};

/**
 * @brief The Object Class.
 */
extern Class __Object;

#endif
