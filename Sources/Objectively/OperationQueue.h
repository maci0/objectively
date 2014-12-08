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

#ifndef _Objectively_OperationQueue_h
#define _Objectively_OperationQueue_h

#include <Objectively/Condition.h>
#include <Objectively/Lock.h>
#include <Objectively/Object.h>
#include <Objectively/Operation.h>
#include <Objectively/Thread.h>

typedef struct OperationQueue OperationQueue;
typedef struct OperationQueueInterface OperationQueueInterface;

/**
 * @brief The OperationQueue type.
 */
struct OperationQueue {

	/**
	 * @brief The parent.
	 */
	Object object;

	/**
	 * @brief The typed interface.
	 */
	OperationQueueInterface *interface;

	/**
	 * @private
	 */
	struct {

		/**
		 * @brief A condition signaled on `addOperation` and `removeOperation`.
		 */
		Condition *condition;

		/**
		 * @brief The Operations.
		 */
		Array *operations;

		/**
		 * @brief The backing Thread.
		 */
		Thread *thread;

	} locals;

	/**
	 * @brief When `YES`, the queue will not `start` any new Operations.
	 */
	BOOL isSuspended;
};

/**
 * @brief The OperationQueue type.
 */
struct OperationQueueInterface {

	/**
	 * @brief The parent.
	 */
	ObjectInterface objectInterface;

	/**
	 * @return The current OperationQueue, or NULL if none can be determined.
	 *
	 * @remark This method should only be called from a synchronous Operation
	 * that was dispatched via an OperationQueue. This method uses thread-local
	 * storage.
	 */
	OperationQueue *(*currentQueue)(void);

	/**
	 * @brief Adds an Operation to this queue.
	 *
	 * @param operation The Operation to add.
	 */
	void (*addOperation)(OperationQueue *self, Operation *operation);

	/**
	 * @brief Cancels all pending Operations residing within this Queue.
	 */
	void (*cancelAllOperations)(OperationQueue *self);

	/**
	 * @brief Initializes this OperationQueue.
	 *
	 * @return The initialized OperationQueue, or `NULL` on error.
	 */
	OperationQueue *(*init)(OperationQueue *self);

	/**
	 * @return An instantaneous copy of this OperationQueue's Operations.
	 */
	Array *(*operations)(const OperationQueue *self);

	/**
	 * @brief Removes the Operation from this queue.
	 *
	 * @param operation The Operation to remove.
	 */
	void (*removeOperation)(OperationQueue *self, Operation *operation);

	/**
	 * @brief Waits until all Operations submitted to this queue have finished.
	 */
	void (*waitUntilAllOperationsAreFinished)(OperationQueue *self);

};

/**
 * @brief The OperationQueue Class.
 */
extern Class __OperationQueue;

#endif
