/**
* CSC 541 Assignment 2
* Mitchell Neville
* 
* This program explores in-memory indexing with availability lists of
* various strategies. For complete documentation information, please see
* the README.
*/

#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#include <main.h>

index_S prim[MAX_INDICES] = { { 0, 0 } };
avail_S avail[MAX_INDICES] = { { 0, 0 } };

std::string studentFile;
std::string strategy;

FILE *outStudentDb;
FILE *outIndex;
FILE *outAvail;
int availCount = 0;
int primCount = 0;

bool isValidStrategyArg(std::string strategy);
bool recordExists(int key);
void addRecord(int key, char *strKey, char *last, char *first, char *major);
int getAvailOffset(int recSize);
void printPrim();
void printAvail();
void findRecord(int key);
int binarySearch(int target, int lower, int upper);
void deleteRecord(int key);
void addAvail(avail_S hole);
void deleteAvailIndex(int index);
void deletePrimIndex(int index);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid number of arguments" << std::endl;
		exit(EXIT_ERROR);
	} 
	else
	{
		strategy = argv[1];
		studentFile = argv[2];

		// Is this a valid strategy argument?
		if (!isValidStrategyArg(strategy))
		{
			std::cout << "Invalid strategy argument: " << strategy << std::endl;
			exit(EXIT_ERROR);
		}

		// If the student.db does not exist, create it
		if ((outStudentDb = fopen(studentFile.c_str(), "r+b")) == NULL)
		{
			// Create all new student.db, avail list, and index files
			outStudentDb = fopen(studentFile.c_str(), "w+b");
			outIndex = fopen(OUT_FILE_INDEX, "w+b");
			outAvail = fopen(OUT_FILE_AVAIL, "w+b");			
		} 
		else
		{
			// Student.db exists. Avail list and index should also exist. 
			// Open avail list and index for reading and updating
			if ((outIndex = fopen(OUT_FILE_INDEX, "r+b")) == NULL)
			{
				std::cout << "Student database found, but index file is missing!" << std::endl;
				exit(EXIT_ERROR);
			}

			if ((outAvail = fopen(OUT_FILE_AVAIL, "r+b")) == NULL)
			{
				std::cout << "Student database found, but availability list file is missing!" << std::endl;
				exit(EXIT_ERROR);
			}


			fseek(outIndex, 0, SEEK_SET);
			index_S index;
			while (fread(&index, sizeof(index_S), 1, outIndex) == 1)
			{
				prim[primCount] = index;
				primCount++;
			}

			avail_S hole;
			while (fread(&hole, sizeof(avail_S), 1, outAvail) == 1)
			{
				avail[availCount] = hole;
				availCount++;
			}			

			/*std::cout << "Recreated index and avail:" << std::endl;
			printPrim();
			printAvail();*/

			fclose(outIndex);
			fclose(outAvail);
		}
				
		// Command loop
		while (1)
		{
			std::string cmdString;
			std::getline(std::cin, cmdString);
			
			char *cmd = (char*)malloc(1 + cmdString.length());
			strcpy(cmd, cmdString.c_str());
			char *tok = strtok(cmd, " |\n");

			if (tok != NULL)
			{
				// First input should be the command name
				if (strcmp(tok, CMD_END) == 0)
				{
					// Write primary key index file to disk
					outIndex = fopen(OUT_FILE_INDEX, "wb");
					fseek(outIndex, 0, SEEK_SET);
					fwrite(prim, sizeof(index_S), primCount, outIndex);
					fclose(outIndex);

					outAvail = fopen(OUT_FILE_AVAIL, "wb");
					fseek(outAvail, 0, SEEK_SET);
					fwrite(avail, sizeof(avail_S), availCount, outAvail);
					fclose(outAvail);

					fclose(outStudentDb);

					printPrim();
					printAvail();

					// Exit the program
					exit(EXIT_SUCCESS);
				}
				else if (strcmp(tok, CMD_ADD) == 0)
				{
					int key = 0;
					tok = strtok(NULL, " \n");
					// Parse integer primary key
					if (tok != NULL)
					{
						key = atoi(tok);
					}
					if (key != 0)
					{
						char *args[4];
						int argCount = 0;

						while (argCount < 4)
						{
							tok = strtok(NULL, " |");
							if (tok == NULL)
							{
								std::cout << "Invalid record format" << std::endl;
								break;
							}
							args[argCount] = tok;
							argCount++;
						}

						if (argCount == 4)
						{
							// Attempt to add the new record
							addRecord(key, args[0], args[1], args[2], args[3]);
						}
					}				
				}
				else if (strcmp(tok, CMD_DELETE) == 0)
				{
					int key = 0;
					tok = strtok(NULL, " \n");
					// Parse integer primary key
					if (tok != NULL)
					{
						key = atoi(tok);
					}
					if (key != 0)
					{
						deleteRecord(key);
					}					
				}
				else if (strcmp(tok, CMD_FIND) == 0)
				{
					int key;
					tok = strtok(NULL, " ");
					// Parse integer primary key
					if (tok != NULL)
					{
						key = atoi(tok);
					}

					findRecord(key);
				}
				else
				{
					std::cout << "Unrecognized command: "<< tok << std::endl;
				}
			}	

			free(cmd);
		}
	}
}

// Returns whether or not a strategy string argument is valid 
bool isValidStrategyArg(std::string strategy)
{
	if (strategy.compare(STRATEGY_BEST_FIT) == 0)
	{
		return true;
	}
	else if (strategy.compare(STRATEGY_FIRST_FIT) == 0)
	{
		return true;
	}
	else if (strategy.compare(STRATEGY_WORST_FIT) == 0)
	{
		return true;
	}

	// Strategy argument did not match valid arg
	return false;
}

// Returns whether or not a record exists
bool recordExists(int key)
{
	int index = binarySearch(key, 0, primCount - 1);
	if (index != -1)
	{
		return true;
	}

	return false;
}

// Add a record to the student database
void addRecord(int key, char *strKey, char *last, char *first, char *major)
{
	if (recordExists(key))
	{
		std::cout << "Record with SID=" << key << " exists" << std::endl;
		return;
	}

	char *recordString = (char*)malloc(strlen(strKey) + strlen(last) + strlen(first) + strlen(major) + 1);
	strcpy(recordString, strKey);
	strcat(recordString, "|");
	strcat(recordString, last);
	strcat(recordString, "|");
	strcat(recordString, first);
	strcat(recordString, "|");
	strcat(recordString, major);

	int recSize = strlen(recordString);
	int buff[1] = { recSize };

	long offset = 0;
	// If there are no holes, append to student.db
	// std::cout << "looking for hole of size " << recSize + sizeof(int) << " for " << strKey << " and " << recordString << std::endl;
	int availIndex = getAvailOffset(recSize + sizeof(int));
	
	if (availIndex != -1)
	{
		
		avail_S hole = avail[availIndex];
		offset = hole.off;
		// std::cout << "Hole available: off: " << hole.off << " size: " << hole.size << std::endl;
		// Remove the old hole from the avail list
		deleteAvailIndex(availIndex);

		if (hole.size > (recSize + sizeof(int)))
		{
			// std::cout << "Leftover space: " << hole.size - recSize - sizeof(int) << std::endl;
			// Add a new hole to the avail list
			avail_S newHole = { hole.size - recSize - sizeof(int),
				offset + recSize + sizeof(int) };
			
			addAvail(newHole);
		}
		fseek(outStudentDb, offset, SEEK_SET);
	}
	else
	{
		fseek(outStudentDb, 0, SEEK_END);
		offset = ftell(outStudentDb);
	}
	
	fwrite(buff, sizeof(int), 1, outStudentDb);

	fwrite(recordString, sizeof(char), recSize, outStudentDb);  
	
	index_S index = { key, offset };
	index_S temp;
	int i = 0;
	if (primCount == 0)
	{
		prim[i] = index;
	}
	else
	{
		for (; i < primCount; i++)
		{
			if (prim[i].key > key)
			{
				break;
			}
		}

		index_S curr = index;
		for (int j = i; j < primCount + 1; j++)
		{
			temp = prim[i];
			prim[i++] = curr;
			curr = temp;
		}
		
	}
	
	primCount++;

	free(recordString);
}

// Returns an index from the availability list that will accomodate a record
// of a specific size. If there is no hole large enough for the record, it will
// return -1.
int getAvailOffset(int recSize)
{

	for (int i = 0; i < availCount; i++)
	{
		if (avail[i].size >= recSize)
		{
			return i;
		}
	}
	
	return -1;
}

// Prints the primary key index
void printPrim()
{
	printf("Index:\n");
	for (int i = 0; i < primCount; i++)
	{
		printf("key=%d: offset=%ld\n", prim[i].key, prim[i].off);
	}
}

// Prints the availability list
void printAvail()
{
	printf("Availability:\n");
	int holeSpace = 0;
	for (int i = 0; i < availCount; i++)
	{
		printf("size=%d: offset=%ld\n", avail[i].size, avail[i].off);
		holeSpace += avail[i].size;
	}
	printf("Number of holes: %d\n", availCount);
	printf("Hole space: %d\n", holeSpace);
}

// Find a record with a specific key. If the record is found, its record
// string is printed. Otherwise a message indicating no record was found is printed.
void findRecord(int key)
{
	// Binary search primary key index for the key
	int i = binarySearch(key, 0, primCount - 1);
	if (i != -1)
	{
		index_S index = prim[i];
		// Go to the offset of the record and read it
		fseek(outStudentDb, index.off, SEEK_SET);
		int recSize = 0;
		fread(&recSize, sizeof(int), 1, outStudentDb);
		char *recBuff = (char*)malloc(recSize + 1);
		fread(recBuff, 1, recSize, outStudentDb);
		recBuff[recSize] = '\0';
		std::cout << recBuff << std::endl;

		free(recBuff);
	}
	else
	{
		std::cout << "No record with SID=" << key << " exists" << std::endl;
	}

	
}

// Performs binary search on the primary key index for a target key value
int binarySearch(int target, int lower, int upper)
{
	if (lower > upper)
	{
		return -1;
	}

	int n = upper - lower;
	int mid = lower + (n / 2);

	if (lower == upper && target != prim[mid].key)
	{
		return -1;
	}
	if (target == prim[mid].key)
	{
		return mid;
	}
	else if (target < prim[mid].key)
	{
		return binarySearch(target, lower, mid - 1);
	}
	else
	{
		return binarySearch(target, mid + 1, upper);
	}
}

// Delete a record with a specific key. If no record is found, a message is 
// printed.
void deleteRecord(int key)
{
	int i = binarySearch(key, 0, primCount - 1);
	if (i == -1)
	{
		std::cout << "No record with SID=" << key << " exists" << std::endl;
		return;
	}

	index_S index = prim[i];
	fseek(outStudentDb, index.off, SEEK_SET);

	int recSize;
	fread(&recSize, sizeof(int), 1, outStudentDb);
	int holeSize = sizeof(int) + recSize;

	avail_S hole = { holeSize, index.off };

	addAvail(hole);

	deletePrimIndex(i);
}

// Comparison function for two availability list entries. Compares holes
// by ascending size.
int compareAscending(const void *a, const void *b)
{
	avail_S *first = (avail_S*)a;
	avail_S *second = (avail_S*)b;

	if (first->size < second->size)
	{
		return -1;
	}
	if (first->size == second->size)
	{
		return 0;
	}
	
	return 1;
	
}

// Comparison function for two availability list entries. Compares holes
// by descending size. If two holes are the same size, the holes' offsets
// are compared in ascending order.
int compareDescending(const void *a, const void *b)
{
	avail_S *first = (avail_S*)a;
	avail_S *second = (avail_S*)b;

	if (first->size < second->size)
	{
		return 1;
	}
	if (first->size == second->size)
	{
		// If the sizes are the same, then sort by ascending offset
		if (first->off < second->off)
		{
			return -1;
		}
		else if (first->off == second->off)
		{
			return 0;
		}

		return 1;

	}
	
	return -1;
	
}

// Add a hole to the availability list. The position of the hole in the list is
// determined by the specified strategy
void addAvail(avail_S hole)
{
	if (strategy.compare(STRATEGY_FIRST_FIT) == 0)
	{
		// Hole is added to end of availability list
		avail[availCount] = hole;
		availCount++;
	}
	else if (strategy.compare(STRATEGY_BEST_FIT) == 0)
	{
		avail[availCount] = hole;
		availCount++;

		qsort(avail, availCount, sizeof(avail_S), compareAscending);
	}
	else if (strategy.compare(STRATEGY_WORST_FIT) == 0)
	{
		avail[availCount] = hole;
		availCount++;

		qsort(avail, availCount, sizeof(avail_S), compareDescending);
	} 
	else
	{
		std::cout << "No strategy defined" << std::endl;
	}
}

// Delete an entry in the availability list by its index
void deleteAvailIndex(int index)
{
	if (index >= availCount)
	{
		return;
	}
	if (index == availCount - 1)
	{
		avail[index].size = 0;
		avail[index].off = 0;
	}
	else
	{
		for (int i = index; i < availCount - 1; i++)
		{
			avail[i] = avail[i + 1];
		}
		avail[availCount].size = 0;
		avail[availCount].off = 0;
	}

	availCount--;
}

// Delete an entry in the primary key index by its index position
void deletePrimIndex(int index)
{
	if (index >= primCount)
	{
		return;
	}
	if (index == primCount - 1)
	{
		prim[index].key = 0;
		prim[index].off = 0;
	}
	else
	{
		for (int i = index; i < primCount - 1; i++)
		{
			prim[i] = prim[i + 1];
		}
		prim[primCount].key = 0;
		prim[primCount].off = 0;
	}
	primCount--;
}
