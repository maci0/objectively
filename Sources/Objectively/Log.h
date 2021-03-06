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

#ifndef _Objectively_Log_h_
#define _Objectively_Log_h_

#include <stdio.h>

#include <Objectively/Object.h>

/**
 * @file
 *
 * @brief A Log4J-inspired log appender.
 */

/**
 * @brief Every Log has a threshold for generating messages.
 *
 * Messages beneath the set threshold level are not output. The default level
 * for newly initialized Logs is `INFO`.
 */
typedef enum {
	TRACE, DEBUG, INFO, WARN, ERROR, FATAL
} LogLevel;

/**
 * @brief The default Log format.
 */
#define LOG_FORMAT_DEFAULT "%c %%n [%%l]: %%m"

typedef struct Log Log;
typedef struct LogInterface LogInterface;

/**
 * @brief A Log4J-inspired log appender.
 *
 * @extends Object
 */
struct Log {

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
	LogInterface *interface;

	/**
	 * @brief The format string, defaults to `LOG_FORMAT_DEFAULT`.
	 *
	 * This string is post-processed after date substitution is performed by
	 * `strftime`. The following tokens are supported:
	 *
	 * * %%n - The Log name.
	 * * %%l - The message level.
	 * * %%m - The message.
	 */
	const char *format;

	/**
	 * @brief The file descriptor (defaults to `stdout`).
	 *
	 * @remark Non-`tty` files are closed when the Log is deallocated.
	 */
	FILE *file;

	/**
	 * @brief The LogLevel of this Log.
	 */
	LogLevel level;

	/**
	 * @brief The name of this Log.
	 */
	char *name;
};

/**
 * @brief The Log interface.
 */
struct LogInterface {

	/**
	 * @brief The parent interface.
	 */
	ObjectInterface objectInterface;

	/**
	 * @return The shared Log instance.
	 *
	 * @relates Log
	 */
	Log *(*sharedInstance)(void);

	/**
	 * @brief Log a debug message.
	 *
	 * @relates Log
	 */
	void (*debug)(const Log *self, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

	/**
	 * @brief Log an error message.
	 *
	 * @relates Log
	 */
	void (*error)(const Log *self, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

	/**
	 * @brief Log a fatal message.
	 *
	 * @relates Log
	 */
	void (*fatal)(const Log *self, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

	/**
	 * @brief Flushes and pending output to this Log's file.
	 *
	 * @relates Log
	 */
	void (*flush)(const Log *self);

	/**
	 * @brief Log an info message.
	 *
	 * @relates Log
	 */
	void (*info)(const Log *self, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

	/**
	 * @brief Initializes this Log.
	 *
	 * @return The initialized Log, or `NULL` on error.
	 *
	 * @relates Log
	 */
	Log *(*init)(Log *self);

	/**
	 * Initializes this Log with the specified name.
	 *
	 * @param name The Log name.
	 *
	 * @return The initialized Log, or `NULL` on error.
	 *
	 * @relates Log
	 */
	Log *(*initWithName)(Log *self, const char *name);

	/**
	 * @brief Write a message to the Log.
	 *
	 * @param level The severity of the message, which must be greater than or
	 * equal to the configured level of this Log.
	 *
	 * @relates Log
	 */
	void (*log)(const Log *self, LogLevel level, const char *fmt, va_list args);

	/**
	 * @brief Log a trace message.
	 *
	 * @relates Log
	 */
	void (*trace)(const Log *self, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));

	/**
	 * @brief Log a warn message.
	 *
	 * @relates Log
	 */
	void (*warn)(const Log *self, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
};

/**
 * @brief The Log Class.
 */
extern Class _Log;

#endif
