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

#ifndef _Objectively_DateFormatter_h_
#define _Objectively_DateFormatter_h_

#include <Objectively/Date.h>
#include <Objectively/String.h>

/**
 * @file
 *
 * @brief Date formatting and parsing.
 */

/**
 * @brief ISO8601 date format.
 */
#define DATEFORMAT_ISO8601 "%Y-%m-%dT%H:%M:%S%z"

typedef struct DateFormatter DateFormatter;
typedef struct DateFormatterInterface DateFormatterInterface;

/**
 * @brief Date formatting and parsing.
 *
 * @extends Object
 *
 * @ingroup Date
 */
struct DateFormatter {

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
	DateFormatterInterface *interface;

	/**
	 * @brief The UTF-8 encoded format string.
	 */
	const char *fmt;
};

/**
 * @brief The DateFormatter interface.
 */
struct DateFormatterInterface {

	/**
	 * @brief The parent interface.
	 */
	ObjectInterface objectInterface;

	/**
	 * Parses a Date from the specified UTF-8 encoded C string.
	 *
	 * @param string The String to parse.
	 *
	 * @return A Date instance, or `NULL` on error.
	 *
	 * @relates DateFormatter
	 */
	Date *(*dateFromCharacters)(const DateFormatter *self, const char *chars);

	/**
	 * Parses a Date from the specified String.
	 *
	 * @param string The String to parse.
	 *
	 * @return A Date instance, or `NULL` on error.
	 *
	 * @relates DateFormatter
	 */
	Date *(*dateFromString)(const DateFormatter *self, const String *string);

	/**
	 * Initializes a DateFormatter with the specified format string.
	 *
	 * @param fmt The format string.
	 *
	 * @return The initialized DateFormatter, or `NULL` on error.
	 *
	 * @see strftime
	 *
	 * @relates DateFormatter
	 */
	DateFormatter *(*initWithFormat)(DateFormatter *self, const char *fmt);

	/**
	 * Yields a String representation of the specified Date instance.
	 *
	 * @param date The Date to format.
	 *
	 * @return The String representation of the Date, or `NULL` on error.
	 *
	 * @relates DateFormatter
	 */
	String *(*stringFromDate)(const DateFormatter *self, const Date *date);
};

/**
 * @brief The DateFormatter Class.
 */
extern Class _DateFormatter;

#endif
