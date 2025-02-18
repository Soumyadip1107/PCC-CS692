#define MQ_KEY 1234
#define MAX_STUDENTS 100
#define NAME_SIZE 32

typedef struct {
	char name[NAME_SIZE];
	int roll;
} Student;

typedef struct {
	long mtype;
	Student students[MAX_STUDENTS];
	int count;
} Message;
