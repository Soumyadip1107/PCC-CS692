#define MQ_KEY 1234
#define DATASIZE 128

typedef struct {
	long mtype;
	char mdata[DATASIZE];
}MQ;
