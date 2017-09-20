#ifndef TABOU_TASK_H
#define TABOU_TASK_H

#include "Structs.h"
#include "Instance.h"

/**
 * Frees a task.
 *
 * @param task The task to free.
 */
void task_destroy(Task * task);

/**
 * Creates a new task ready to be used.
 *
 * @param instance The parent instance.
 * @return A new task.
 */
Task * task_create(struct _Instance * instance);

/**
 * Set the duration on a machine for a task.
 *
 * @param task The task to modify.
 * @param machineID The ID of the machine concerned.
 * @param duration The duration on the defined machine for this task.
 */
static inline void task_setMachineDuration(Task * task, unsigned int machineID, unsigned int duration)
{
	task->machineDurations[machineID] = duration;
}

/**
 * Get the duration on a machine for a task.
 *
 * @param task The task to modify.
 * @param machineID The ID of the machine concerned.
 * @return The duration on the defined machine for this task.
 */
static inline unsigned int task_getMachineDuration(Task * task, unsigned int machineID)
{
	if(machineID > task->instance->machineCount)
	{
		fprintf(stderr, "TASK: Error when getting machine duration, index out of range (machineID: %d).", machineID);
		exit(EXIT_FAILURE);
	}
	return task->machineDurations[machineID];
}

/**
 * Set the due date for a task.
 *
 * @param task The task to modify.
 * @param duration The due date for this task.
 */
static inline void task_setDueDate(Task * task, unsigned int date)
{
	task->dueDate = date;
}

/**
 * Get the due date for a task.
 *
 * @param task The task to modify.
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

#endif //TABOU_TASK_H
