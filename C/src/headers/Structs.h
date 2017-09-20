#ifndef TABOU_STRUCTS_H
#define TABOU_STRUCTS_H

struct _Instance;
struct _Task;

//Structure defining an instance.
typedef struct _Instance
{
	unsigned int machineCount; //Count of machines.
	unsigned int taskCount; //Count of tasks.
	unsigned int ** distancesMatrix; //Matrix of the distances for the delivery.
	struct _Task ** tasks; //List of the tasks of the instance.
} Instance;

//Structure defining a task.
typedef struct _Task
{
	struct _Instance * instance; //The parent instance.
	unsigned int * machineDurations; //The duration on each machine.
	unsigned int dueDate; //The due date.
} Task;

#endif //TABOU_STRUCTS_H
