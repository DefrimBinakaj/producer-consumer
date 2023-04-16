#include "common.h"


// producer thread function
void *producer(void *threadIteration) {

    // init call type and num
    char prodCall;
    int prodArg;
    // scan input in format %c%d for each line
    while ((scanf("%c%d \n", &prodCall, &prodArg)) != EOF) {
        
        // if the call type is T, push work onto buffer
		if (prodCall == 'T') {
            sem_wait(&semEmpty);
            pthread_mutex_lock(&mutex1);

            // add to queue
			callBuffer.push(prodCall);
			argBuffer.push(prodArg);

            pthread_mutex_unlock(&mutex1);
            sem_post(&semFull);
            producerID.workNum ++;

            pthread_mutex_lock(&mutex2);
            printf("%s ID= %d Q=%2ld Work\t%12d\n",end_time(start_time).c_str(), 0, callBuffer.size(), prodArg);
            pthread_mutex_unlock(&mutex2);
		}

        // if call type is S, sleep
        else if (prodCall == 'S') {
            pthread_mutex_lock(&mutex2);
            printf("%s ID= %d \t Sleep \t \t \t   %d\n",end_time(start_time).c_str(), 0, prodArg);
            pthread_mutex_unlock(&mutex2);
			Sleep(prodArg);
            producerID.sleepNum ++;
        }
        // neither command was used; ignore
		else {
			printf("Please use either the Trans or Sleep command\n");
		}
    }


    // fill in rest of buffer after all call lines were read
    printf("%s ID= %d \t End \n", end_time(start_time).c_str(), 0);

    for (int i = 0; i < numThreads; i++) {
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutex1);

        // use values of Z and -1 since there are no Z functions
        // and -1 would not normally work with Trans or Sleep
		callBuffer.push('Z');
		argBuffer.push(-1);

        pthread_mutex_unlock(&mutex1);
        sem_post(&semFull);
    }
	
    return 0;
}