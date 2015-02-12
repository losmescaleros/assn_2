#define EXIT_ERROR -1
#define EXIT_SUCCESS 0

#define MAX_INDICES 10000

// Holes in availability list are saved in order added 
#define STRATEGY_FIRST_FIT "--first-fit"
// Order holes in availability list in ascending size order 
#define STRATEGY_BEST_FIT "--best-fit"
// Order holes in descending size order in availability list
#define STRATEGY_WORST_FIT "--worst-fit"

#define OUT_FILE_INDEX "index.bin"
#define OUT_FILE_AVAIL "avail.bin"

#define CMD_ADD "add"
#define CMD_DELETE "del"
#define CMD_FIND "find"
#define CMD_END "end"

typedef struct
{
	// Record's key
	int key;
	// Record's offset in memory
	long off;
} index_S;

typedef struct
{
	int size;
	long off;
} avail_S;

typedef struct
{
	int id;
	std::string last;
	std::string first;
	std::string major;
} studentRecord;
