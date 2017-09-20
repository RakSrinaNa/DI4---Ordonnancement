#ifndef TABOU_PARSER_H
#define TABOU_PARSER_H

#include <stdio.h>
#include "Instance.h"

/**
 * Read an instance from a text file.
 *
 * @param filepath The path to the file to read.
 * @return The instance read or null if an error occurred.
 */
Instance * parser_readInstanceFromFile(char * filepath);

/**
 * Fill an instance with the content of a file.
 *
 * @param file The file that will be read.
 * @param instance The instance to fill.
 * @return The filled instance or null if an error happened. If null is returned, the instance will also be freed.
 */
Instance * parser_fillInstance(FILE * file, Instance * instance);

/**
 * Read a non empty line from a file.
 *
 * @param file The file to read from.
 * @return A string on the heap being the next non empty line in the file. If it couldn't read any, NULL is returned.
 */
char * parser_readLine(FILE * file);

/**
 * Transform a line of numbers into an array of integers.
 *
 * @param line The line to parse.
 * @param valuesNumber The maximum number values to read.
 * @return An array of integer on the heap. If there was less values than valuesNumber, the remaining ones are 0.
 */
int * parser_lineToIntArray(char * line, int valuesNumber);

/**
 * Read a line from the file.
 *
 * @param linePtr A pointer to the string that will be read.
 * @param lineSize A pointer to a size_t defining the size of linePtr.
 * @param file The file to read from.
 * @return The length read.
 */
int getLine(char ** linePtr, size_t * lineSize, FILE * file);

#endif //TABOU_PARSER_H
