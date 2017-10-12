#ifndef TABOU_TASK_H
#define TABOU_TASK_H

#include <stdlib.h>

struct _Task;

#include "Instance.h"

//Structure defining a task.
typedef struct _Task
{
	struct _Instance * instance; //The parent instance.
	unsigned int * machineDurations; //The duration on each machine.
	unsigned int dueDate; //The due date.
} Task;

/**
 * Frees a task.
 *
 * @param task The task to free.
 */
void task_destroy(Task * task);

/**
 * Creates a new task ready to be used.
 *
 * @param instance The parent instance. Not null.
 * @return A new task.
 */
Task * task_create(struct _Instance * instance);

/**
 * Set the duration on a machine for a task.
 * If the machine isn't found, nothing is changed.
 *
 * @param task The task to modify. Not null.
 * @param machineID The ID of the machine concerned.
 * @param duration The duration on the defined machine for this task.
 */
void task_setMachineDuration(Task * task, unsigned int machineID, unsigned int duration);

/**
 * Get the duration on a machine for a task.
 * If the machine isn't found, a fatal error is thrown.
 *
 * @param task The task to get the duration for. Not null.
 * @param machineID The ID of the machine concerned.
 * @return The duration on the defined machine for this task.
 */
unsigned int task_getMachineDuration(Task * task, unsigned int machineID);

/**
 * Set the due date for a task.
 *
 * @param task The task to modify. Not null.
 * @param duration The due date for this task.
 */
static inline void task_setDueDate(Task * task, unsigned int date)
{
	task->dueDate = date;
}

/**
 * Get the due date for a task.
 *
 * @param task The task to modify. Not null.
 * @return The due date for this task.
 */
static inline unsigned int task_getDueDate(Task * task)
{
	return task->dueDate;
}

/**
 * Prints a task in the console.
 *
 * @param task The task to print.
 */
void task_print(Task * task);

#endif
