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

#include <check.h>
#include <stdlib.h>

#include <Objectively.h>

BOOL enumerator(const Array *array, id obj, id data) {
	(*(int *) data)++; return NO;
}

ORDER comparator(const id obj1, const id obj2) {
	return $((Number *) obj1, compareTo, (Number *) obj2);
}

START_TEST(mutableArray)
	{
		MutableArray *array = $$(MutableArray, array);

		ck_assert(array != NULL);
		ck_assert_ptr_eq(&_MutableArray, classof(array));

		ck_assert_int_eq(0, ((Array *) array)->count);

		Object *one = $(alloc(Object), init);
		Object *two = $(alloc(Object), init);
		Object *three = $(alloc(Object), init);

		$(array, addObject, one);
		$(array, addObject, two);
		$(array, addObject, three);

		ck_assert_int_eq(3, ((Array *) array)->count);

		ck_assert($((Array *) array, containsObject, one));
		ck_assert($((Array *) array, containsObject, two));
		ck_assert($((Array *) array, containsObject, three));

		ck_assert_int_eq(0, $((Array *) array, indexOfObject, one));
		ck_assert_int_eq(1, $((Array *) array, indexOfObject, two));
		ck_assert_int_eq(2, $((Array *) array, indexOfObject, three));

		ck_assert_int_eq(2, one->referenceCount);
		ck_assert_int_eq(2, two->referenceCount);
		ck_assert_int_eq(2, three->referenceCount);

		$(array, removeObject, one);

		ck_assert(!$((Array *) array, containsObject, one));
		ck_assert_int_eq(1, one->referenceCount);
		ck_assert_int_eq(2, ((Array *) array)->count);

		$(array, removeAllObjects);

		ck_assert_int_eq(0, ((Array *) array)->count);

		ck_assert(!$((Array *) array, containsObject, two));
		ck_assert_int_eq(1, two->referenceCount);

		ck_assert(!$((Array *) array, containsObject, three));
		ck_assert_int_eq(1, three->referenceCount);

		release(one);
		release(two);
		release(three);

		$(array, removeAllObjects);

		ck_assert_int_eq(((Array *) array)->count, 0);

		srand(time(NULL));

		for (size_t i = 0; i < 100; i++) {

			Number *number = $$(Number, numberWithValue, rand());

			$(array, addObject, number);

			release(number);
		}

		$(array, sort, comparator);

		int previous = -1;

		for (size_t i = 0; i < ((Array *) array)->count; i++) {

			Number *number = $((Array *) array, objectAtIndex, i);

			ck_assert_int_ge($(number, intValue), previous);

			previous = $(number, intValue);
		}

		release(array);

	}END_TEST

int main(int argc, char **argv) {

	TCase *tcase = tcase_create("mutableArray");
	tcase_add_test(tcase, mutableArray);

	Suite *suite = suite_create("mutableArray");
	suite_add_tcase(suite, tcase);

	SRunner *runner = srunner_create(suite);

	srunner_run_all(runner, CK_VERBOSE);
	int failed = srunner_ntests_failed(runner);

	srunner_free(runner);

	return failed;
}
