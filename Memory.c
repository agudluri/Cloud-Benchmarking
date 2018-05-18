#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

long sizes[4] = {8,8*1024,8*1024*1024,80*1024*1024};
const char *sizenames[]={"8 Bytes","8 Kilobytes", "8 Megabytes", "80 Megabytes"};
long numOps[4]={174483045,170392,165,16};
char *source;

//Method declarations
void Thread(int number);
void *Data(void* lol);

//Operation struct
double factor = ((temp->numOperations*temp->size)-temp->size) / RAND_MAX;
typedef struct operation {
        int type;
        long size;
        long numOperations;
        int id;
} Run;

/*
   The main function. Here, it basically calls the output function with the number
   of threads as the parameter.
 */
int main(int argc, char *argv[]) {

        switch (atoi(argv[1])) {
        case 1:
                Thread(1);
                break;
        case 2:
                Thread(2);
                break;
        case 4:
                Thread(4);
                break;
        case 8:
                Thread(8);
                break;
        default:
                printf("%s\n","The number of threads should be 1,2,4 or 8.");
                break;
        }
        return 0;
}

/*
   This method is where we run the threads and calculate the time elapsed.
 */
char * destination;
void Thread(int number){
        printf("\nNo. of threads: %d\n-----------------\n",number);

        //time_t start, end;
        struct timespec start, finish;
        double elapsed;

        Run *run = (Run*)malloc(number*sizeof(Run));
        pthread_t *threads = (pthread_t *)malloc(number * sizeof(pthread_t));
        destination = (char *) malloc(1395864372);

        //Allocating and filling up the source data that would be copied into the destination array later
        source = (char *) malloc(1395864372); //1.3GB
        for (long lol=0; lol<1395864372; lol++)
        {
                source[lol] = (char)('a'+ (lol%26));
        }
        //Loop for block sizes
        for (int i = 0; i<4; i++)
        {
                printf("\n\nBlock Size: %s\n------------------------\n------------------------\n",sizenames[i]);

                //--------------------------------------------------------------

                //Loop for operation types
                for(int j=0; j<3; j++) {

                        //Print the name of the operation accordingly
                        switch (j) {
                        case 0:
                                printf("Operation: Read+Write\n");
                                break;
                        case 1:
                                printf("Operation: Sequential Write\n");
                                break;
                        case 2:
                                printf("Operation: Random Write\n");
                                break;
                        }

                        //Timer start ----------------------------------------------------------
                        //start = time(NULL);
                        clock_gettime(CLOCK_MONOTONIC, &start);
                        //printf("Start time %ld\n", start);
                        for (int t=0; t<number; t++)
                        {
                                run[t].numOperations = numOps[i]/((long)number);
                                run[t].size = sizes[i];
                                run[t].type=j;
                                run[t].id = t;
                                //  printf("Run id: %d\n", run[t].id);
                                pthread_create(&threads[t],NULL,Data,&run[t]);

                        }
                        for (int t=0; t<number; t++)
                        {
                                pthread_join(threads[t], NULL );
                        }
                        //sleep(2);
                        //end = time(NULL);
                        clock_gettime(CLOCK_MONOTONIC, &finish);
                        //printf("End time %ld\n", end);
                        //Timer end---------------------------------------------------------------
                        //double difference = difftime(end, start);
                        elapsed = (finish.tv_sec - start.tv_sec);
                        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
                        double difference = elapsed;
                        printf("difference: %lf\n",difference);
                        double throughput = (double)(numOps[i]*sizes[i])/(1048576*difference);
                        double latency = (double)(difference*1000000/numOps[i]);
                        if(i==0) {
                                printf("Latency: ");
                                printf("%lf ns\n",latency);
                        } else{
                                printf("Throughput: ");
                                printf("%lf MBps\n",throughput);
                        }
                        printf("----------------------------------\n");
                }
        }
        printf("%s\n","----------------------------------------------------------------------------" );

        //Freeing pointers
        pthread_exit(NULL);
        free(source);
        free(destination);
        free(threads);
        free(run);
        destination=NULL;
        source=NULL;
        threads=NULL;
        run=NULL;

        return;
}

void *Data(void* lol)
{
        long random=0;
        Run* temp = (Run*)lol;


        //If operation type is R+W
        if(temp->type==0) {
                int start = temp->id * temp->numOperations;
                for (int i = start; i<temp->numOperations+start; i++)
                {
                        memcpy(&destination[i*temp->size],&source[i*temp->size],temp->size);
                }
        }


        //if the operation type is sequential write
        else if (temp->type==1) {
                for (int i=0; i<temp->numOperations; i++)
                {
                        memset(&destination[i*temp->size],1,temp->size);
                }
        }

        //if the operation type is random write
        else if(temp->size==2) {
                double factor = ((temp->numOperations*temp->size)-temp->size) / RAND_MAX;
                for (int i=0; i<temp->numOperations; i++)
                {
                        random = rand()*factor;;
                        memset(&destination[random], 1, temp->size);
                }
        }

        return NULL;
}
