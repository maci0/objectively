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

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include <Objectively/Dictionary.h>
#include <Objectively/Hash.h>
#include <Objectively/MutableArray.h>
#include <Objectively/MutableDictionary.h>
#include <Objectively/MutableString.h>

#define _Class _Dictionary

#pragma mark - ObjectInterface

/**
 * @see ObjectInterface::copy(const Object *)
 */
static Object *copy(const Object *self) {

	const Dictionary *this = (Dictionary *) self;

	Dictionary *that = $(alloc(Dictionary), initWithDictionary, this);

	return (Object *) that;
}

/**
 * @see ObjectInterface::dealloc(Object *)
 */
static void dealloc(Object *self) {

	Dictionary *this = (Dictionary *) self;

	for (size_t i = 0; i < this->capacity; i++) {
		release(this->elements[i]);
	}

	free(this->elements);

	super(Object, self, dealloc);
}

/**
 * @brief A DictionaryEnumerator for description.
 */
static BOOL description_enumerator(const Dictionary *dict, id obj, id key, id data) {

	MutableString *desc = (MutableString *) data;

	String *objDesc = $((Object *) obj, description);
	String *keyDesc = $((Object *) key, description);

	$(desc, appendFormat, "%s: %s, ", keyDesc->chars, objDesc->chars);

	release(objDesc);
	release(keyDesc);

	return NO;
}

/**
 * @see ObjectInterface::description(const Object *)
 */
static String *description(const Object *self) {

	const Dictionary *this = (Dictionary *) self;

	MutableString *desc = $(alloc(MutableString), init);

	$(desc, appendFormat, "{");

	$(this, enumerateObjectsAndKeys, description_enumerator, desc);

	$(desc, appendFormat, "}");

	return (String *) desc;
}

/**
 * @see ObjectInterface::hash(const Object *)
 */
static int hash(const Object *self) {

	const Dictionary *this = (Dictionary *) self;

	int hash = HashForInteger(HASH_SEED, this->count);

	for (size_t i = 0; i < this->capacity; i++) {
		if (this->elements[i]) {
			hash = HashForObject(hash, this->elements[i]);
		}
	}

	return hash;
}

/**
 * @see ObjectInterface::isEqual(const Object *, const Object *)
 */
static BOOL isEqual(const Object *self, const Object *other) {

	if (super(Object, self, isEqual, other)) {
		return YES;
	}

	if (other && $(other, isKindOfClass, &_Dictionary)) {

		const Dictionary *this = (Dictionary *) self;
		const Dictionary *that = (Dictionary *) other;

		if (this->count == that->count) {

			Array *keys = $(this, allKeys);

			for (size_t i = 0; i < keys->count; i++) {
				const id key = $(keys, objectAtIndex, i);

				const Object *thisObject = $(this, objectForKey, key);
				const Object *thatObject = $(that, objectForKey, key);

				if ($(thisObject, isEqual, thatObject) == NO) {
					release(keys);
					return NO;
				}
			}

			release(keys);
			return YES;
		}
	}

	return NO;
}

#pragma mark - DictionaryInterface

/**
 * @brief DictionaryEnumerator for allKeys.
 */
static BOOL allKeys_enumerator(const Dictionary *dict, id obj, id key, id data) {
	$((MutableArray *) data, addObject, key); return NO;
}

/**
 * @see DictionaryInterface::allKeys(const Dictionary *)
 */
static Array *allKeys(const Dictionary *self) {

	MutableArray *keys = $(alloc(MutableArray), initWithCapacity, self->count);

	$(self, enumerateObjectsAndKeys, allKeys_enumerator, keys);

	return (Array *) keys;
}

/**
 * @brief DictionaryEnumerator for allObjects.
 */
static BOOL allObjects_enumerator(const Dictionary *dict, id obj, id key, id data) {
	$((MutableArray *) data, addObject, obj); return NO;
}

/**
 * @see DictionaryInterface::allObjects(const Dictionary *)
 */
static Array *allObjects(const Dictionary *self) {

	MutableArray *objects = $(alloc(MutableArray), initWithCapacity, self->count);

	$(self, enumerateObjectsAndKeys, allObjects_enumerator, objects);

	return (Array *) objects;
}

/**
 * @see DictionaryInterface::dictionaryWithDictionary(const Dictionary *)
 */
static Dictionary *dictionaryWithDictionary(const Dictionary *dictionary) {

	return $(alloc(Dictionary), initWithDictionary, dictionary);
}

/**
 * @see DictionaryInterface::dictionaryWithObjectsAndKeys(id, ...)
 */
static Dictionary *dictionaryWithObjectsAndKeys(id obj, ...) {

	Dictionary *dict = (Dictionary *) super(Object, alloc(Dictionary), init);
	if (dict) {

		va_list args;
		va_start(args, obj);

		while (obj) {
			id key = va_arg(args, id);

			$$(MutableDictionary, setObjectForKey, (MutableDictionary *) dict, obj, key);

			obj = va_arg(args, id);
		}

		va_end(args);
	}

	return dict;
}

/**
 * @see DictionaryInterface::enumerateObjectsAndKeys(const Dictionary *, DictionaryEnumerator, id)
 */
static void enumerateObjectsAndKeys(const Dictionary *self, DictionaryEnumerator enumerator,
		id data) {

	assert(enumerator);

	for (size_t i = 0; i < self->capacity; i++) {

		Array *array = self->elements[i];
		if (array) {

			for (size_t j = 0; j < array->count; j += 2) {

				id key = $(array, objectAtIndex, j);
				id obj = $(array, objectAtIndex, j + 1);

				if (enumerator(self, obj, key, data)) {
					return;
				}
			}
		}
	}
}

/**
 * @see DictionaryInterface::filterObjectsAndKeys(const Dictionary *, DictionaryEnumerator, id)
 */
static Dictionary *filterObjectsAndKeys(const Dictionary *self, DictionaryEnumerator enumerator,
		id data) {

	assert(enumerator);

	MutableDictionary *dictionary = $(alloc(MutableDictionary), init);

	for (size_t i = 0; i < self->capacity; i++) {

		Array *array = self->elements[i];
		if (array) {

			for (size_t j = 0; j < array->count; j += 2) {

				id key = $(array, objectAtIndex, j);
				id obj = $(array, objectAtIndex, j + 1);

				if (enumerator(self, obj, key, data)) {
					$(dictionary, setObjectForKey, obj, key);
				}
			}
		}
	}

	return (Dictionary *) dictionary;
}

/**
 * @see DictionaryInterface::initWithDictionary(Dictionary *, const Dictionary *)
 */
static Dictionary *initWithDictionary(Dictionary *self, const Dictionary *dictionary) {

	self = (Dictionary *) super(Object, self, init);
	if (self) {
		if (dictionary) {

			self->capacity = dictionary->capacity;

			self->elements = calloc(self->capacity, sizeof(id));
			assert(self->elements);

			for (size_t i = 0; i < dictionary->capacity; i++) {

				Array *array = dictionary->elements[i];
				if (array) {
					self->elements[i] = $((Object *) array, copy);
				}
			}

			self->count = dictionary->count;
		}
	}

	return self;
}

/**
 * @see DictionaryInterface::initWithObjectsAndKeys(Dictionary *, ...)
 */
static Dictionary *initWithObjectsAndKeys(Dictionary *self, ...) {

	self = (Dictionary *) super(Object, self, init);
	if (self) {

		va_list args;
		va_start(args, self);

		while (YES) {

			id obj = va_arg(args, id);
			if (obj) {

				id key = va_arg(args, id);
				$$(MutableDictionary, setObjectForKey, (MutableDictionary *) self, obj, key);
			} else {
				break;
			}
		}

		va_end(args);
	}

	return self;
}

/**
 * @see DictionaryInterface::mutableCopy(const Dictionary *)
 */
static MutableDictionary *mutableCopy(const Dictionary *self) {

	MutableDictionary *copy = $(alloc(MutableDictionary), initWithCapacity, self->count);
	if (copy) {
		$(copy, addEntriesFromDictionary, self);
	}

	return copy;
}

/**
 * @see DictionaryInterface::objectForKey(const Dictionary *, const id)
 */
static id objectForKey(const Dictionary *self, const id key) {

	const size_t bin = HashForObject(HASH_SEED, key) % self->capacity;

	Array *array = self->elements[bin];
	if (array) {

		int index = $(array, indexOfObject, key);
		if (index > -1) {
			return $(array, objectAtIndex, index + 1);
		}
	}

	return NULL;
}

#pragma mark - Class lifecycle

/**
 * @see Class::initialize(Class *)
 */
static void initialize(Class *clazz) {

	ObjectInterface *object = (ObjectInterface *) clazz->interface;

	object->copy = copy;
	object->dealloc = dealloc;
	object->description = description;
	object->hash = hash;
	object->isEqual = isEqual;

	DictionaryInterface *dictionary = (DictionaryInterface *) clazz->interface;

	dictionary->allKeys = allKeys;
	dictionary->allObjects = allObjects;
	dictionary->dictionaryWithDictionary = dictionaryWithDictionary;
	dictionary->dictionaryWithObjectsAndKeys = dictionaryWithObjectsAndKeys;
	dictionary->enumerateObjectsAndKeys = enumerateObjectsAndKeys;
	dictionary->filterObjectsAndKeys = filterObjectsAndKeys;
	dictionary->initWithDictionary = initWithDictionary;
	dictionary->initWithObjectsAndKeys = initWithObjectsAndKeys;
	dictionary->mutableCopy = mutableCopy;
	dictionary->objectForKey = objectForKey;
}

Class _Dictionary = {
	.name = "Dictionary",
	.superclass = &_Object,
	.instanceSize = sizeof(Dictionary),
	.interfaceOffset = offsetof(Dictionary, interface),
	.interfaceSize = sizeof(DictionaryInterface),
	.initialize = initialize,
};

#undef _Class

