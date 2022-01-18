
#include  <stdio.h>
#include  <stdlib.h>
#include  <libgen.h>
#include  <unistd.h>
#include  <sys/wait.h>
#include  <sys/types.h>
#include  <math.h>
#include <stdint.h>
#include <cctype>
#include <string>
 
#include  "fifo.h"
#include  "delays.h"
#include  "process.h"

#define ACCESS 0
#define FULLNESS 1
#define EMPTINESS 2

typedef struct FIFO
{
    uint32_t ii;   ///< point of insertion
    uint32_t ri;   ///< point of retrieval
    uint32_t cnt;  ///< number of items stored
    uint32_t ids[FIFOSZ];  ///< storage memory
}FIFO;

typedef struct ServiceRequest
{   
    uint32_t clientid;
    char *frase;
}ServiceRequest;

typedef struct ServiceResponse
{
    uint32_t len = 0;
    uint32_t alpha = 0;
    uint32_t numbers = 0;
    uint32_t serverid = 0;
}ServiceResponse;

typedef struct SLOT
{
    ServiceRequest req;
    ServiceResponse res;
    bool hasResponse = false;
    
}SLOT;

typedef struct ARG
{
    int id;
    int iter;
}ARG;


static SLOT slots[FIFOSZ];
static FIFO frees;
static FIFO pending;

void freesInit(void){
    
    frees.ii = frees.ri = 0;
    frees.cnt = FIFOSZ;
    uint32_t i;
    for ( i = 0; i < FIFOSZ; i++)
    {
        frees.ids[i] = i;
    }
    
}

void pendingInit(void){
    
    pending.cnt = pending.ii = pending.ri = 0;
    
}

void insert(FIFO *fifo, uint32_t id){
    

    while (fifo->cnt == FIFOSZ)
    {
        
    }

    fifo->ids[fifo->ii] = id;
    fifo->ii = (fifo->ii + 1) % FIFOSZ;
    fifo->cnt++;
    
}

uint32_t retrive(FIFO *fifo){
   

    while (fifo->cnt == 0)
    {
        
    }
    
    uint32_t id = fifo->ids[fifo->ri];
    fifo->ids[fifo->ri] = 99999999;
    fifo->ri = (fifo->ri + 1) % FIFOSZ;
    fifo->cnt--;
   
    return id;
}

void signalResponseIsAvailable(uint32_t id){
    slots[id].hasResponse = true;
    
}

void waitForResponse(uint32_t id){
    while (!slots[id].hasResponse)
    {
        
    }
    slots[id].hasResponse = false;
}

void callService(ServiceRequest &req, ServiceResponse &res){
    uint32_t id = retrive(&frees);
    
    slots[id].req = req;
    slots[id].res = res;
    insert(&pending, id);
    printf("Client %u call service\n", req.clientid);
    waitForResponse(id);
    res = slots[id].res;
    printf("Size: %u,  Alpha: %u,  Numbers: %u,  ServerId: %u\n",res.len,res.alpha,res.numbers,res.serverid);
   
    insert(&frees, id);
}

void processService(uint32_t sid){
    uint32_t id = retrive(&pending);
    
    slots[id].res.serverid = sid;

    char *i;
    for ( i = slots[id].req.frase; *i != '\0'; i++)
    {
        slots[id].res.len++;
        if(isdigit(*i)) slots[id].res.numbers++;
        if(isalpha(*i)) slots[id].res.alpha++; 
    }
    signalResponseIsAvailable(id);
    
}

void *client(void *arg){
    ARG* x = (ARG*)arg;
    printf("Client %d created\n",x->id);
    int iter = 10;
    int i;
    for (i = 0; i < iter; i++)
    {
        ServiceResponse res; //= new ServiceResponse();
        ServiceRequest req; //= new ServiceRequest();
        req.clientid = x->id;
        req.frase = (char *)"Teste String 123";
        callService(req, res);
    }
    return NULL;
}

void *server(void *arg){
    int x = *((int*)arg);
    printf("Server %d created\n", x);
    while (1)
    {
        processService(x);   
    }
}

int main(void){
    int nservers = 2;
    int nclients = 2;

    freesInit();
    pendingInit();

    
    int sarg[nservers];
    ARG carg[nclients];

    int i;
    for ( i = 0; i < nservers; i++)
    {   
        sarg[i] = i;
        
    }

    int j;
    for ( j = 0; j < nclients; j++)
    {
        carg[j].id = j;
        
    }

    int k;
    for ( k = 0; k < nclients; k++)
    {
        
        printf("Client %d terminated\n",k);
    }

    int n;
    for ( n = 0; n < nservers; n++)
    {
        
        printf("Server %d terminated\n", n);
    }

    return EXIT_SUCCESS;
}