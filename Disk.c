#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

long sizes[4] = {8,8*1024,8*1024*1024,80*1024*1024};
const char *sizenames[]={"8 Bytes","8 Kilobytes", "8 Megabytes", "80 Megabytes"};
long numOps[4] = {134217728,131072,128,12};

//Method declarations
void Thread(int number);
void *Data(void* lol);

//Operation struct
typedef struct operation {
        int type;
        long size;
        long numOperations;
} Run;


//The main function.Here, it basically calls the output function with the number
//of threads as the parameter.
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
void Thread(int number){
        printf("\nNo. of threads: %d\n-----------------\n",number);

        struct timespec start, finish;
        Run *run = (Run*)malloc(number*sizeof(Run));
        pthread_t *threads = (pthread_t *)malloc(number * sizeof(pthread_t));

        //Creating the input file first ----------------------------------------
        FILE* fl;

        //Filling up source data to be written to the file
        const char *source = "z";
        fl = fopen("read_input.txt","a");
        for (int z=0; z<1073741824; z++)
        {
                fwrite(source,1,1,fl);
        }
        fclose(fl);
        // ^ Our input file is ready to be read.--------------------------------

        //Loop for block sizes
        for (int i = 0; i<4; i++)
        {
                printf("\n\nBlock Size: %s\n------------------------\n------------------------\n",sizenames[i]);
                run->numOperations = numOps[i]/((long)number);
                run->size = sizes[i];

                //--------------------------------------------------------------

                //Loop for operation types
                for(int j=0; j<3; j++) {
                        run->type=j;

                        //Print the name of the operation accordingly
                        switch (j) {
                        case 0:
                                printf("Operation: Read+Write\n");
                                break;
                        case 1:
                                printf("Operation: Sequential Read\n");
                                break;
                        case 2:
                                printf("Operation: Random Read\n");
                                break;
                        }

                        //Timer start ----------------------------------------------------------
                        //  start = clock();
                        clock_gettime(CLOCK_MONOTONIC, &start);

                        for (int t=0; t<number; t++)
                        {
                                pthread_create(&threads[t],NULL,Data,run);

                        }
                        for (int t=0; t<number; t++)
                        {
                                pthread_join(threads[t], NULL );
                        }
                        //  end = clock();
                        clock_gettime(CLOCK_MONOTONIC, &finish);

                        //Timer end---------------------------------------------------------------
                        double elapsed = (finish.tv_sec - start.tv_sec);
                        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
                        double difference = elapsed;
                        double throughput = (double)(numOps[i]*sizes[i]*number)/(1048576*difference);
                        double latency = (double)(difference*1000000/numOps[i]);
                        if(i==0) {
                                printf("Latency: ");
                                printf("%lf ns\n",latency);
                        } else{
                                printf("Throughput: ");
                                printf("%lfMBps\n",throughput);
                        }
                        printf("----------------------------------\n");
                }
        }
        printf("%s\n","----------------------------------------------------------------------------" );

        //Freeing pointers and removing created files
        remove("read_input.txt");
        pthread_exit(NULL);
        free(threads);
        free(run);
        threads=NULL;
        run=NULL;
        return;
}

/*
   This method performs the requisite read/write operations
 */
void *Data(void* lol)
{
        Run* temp = (Run*)lol;
        FILE *fl;

        long random=0;

        //creating some data to either be written to an output file or to
        //act like a buffer
        char *something = (char *) malloc(temp->size*sizeof(char));
        for (int i=0; i<temp->size; i++)
        {
                something[i] = (char)('a'+ (i%26));
        }

        //If operation type is R+W
        if(temp->type==0) {
                fl = fopen("write_output.txt","w+");

                for (int i=0; i<temp->numOperations; i++)
                {
                        fwrite(something,1,temp->size,fl);
                        fflush(fl);
                }
        }

        //if the operation type is sequential read
        else if (temp->type==1) {
                fl = fopen("read_input.txt","r+");

                for (int i=0; i<temp->numOperations; i++)
                {
                        fread(something,1,temp->size,fl);
                }
        }

        //if the operation type is random read
        else if(temp->size==2) {
                double factor = ((temp->numOperations*temp->size)-temp->size) / RAND_MAX;
                fl = fopen("read_input.txt","r+");
                for (int i=0; i<temp->numOperations; i++)
                {
                        random = rand()*factor;
                        fseek(fl,random,SEEK_SET);
                        fread(something,1,temp->size,fl);
                }
        }
        remove("write_output.txt");
        return NULL;
}
