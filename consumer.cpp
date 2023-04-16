#include "common.h"


// consumer thread function
void *consumer(void *threadIteration) {

    // threadID = threadIteration + 1
    allConsumers.at(*(int *)threadIteration).threadID = *(int *)threadIteration + 1;

	while (true) {
        // print ask line
        pthread_mutex_lock(&mutex2);
        printf("%s ID= %d \t Ask\n",end_time(start_time).c_str(), allConsumers.at(*(int *)threadIteration).threadID);
        pthread_mutex_unlock(&mutex2);
        // add one to askNum
        allConsumers.at(*(int *)threadIteration).askNum ++;
        

        sem_wait(&semFull);
        pthread_mutex_lock(&mutex1);

        // register this specific call and arg
		char firstCall = callBuffer.front();
		int firstArg = argBuffer.front();
        // pop from both queues
		callBuffer.pop();
		argBuffer.pop();

        pthread_mutex_unlock(&mutex1);
        sem_post(&semEmpty);

        // stop the loop if the next work wasn't an acceptable call
		if (firstArg == -1) {
			break;
		}
        // check if call was T and arg isn't -1
		else if (firstCall == 'T' && firstArg != -1) {
            
            // print statements and run Trans
            pthread_mutex_lock(&mutex2);
			printf("%s ID= %d Q=%2ld Receive\t%8d\n",end_time(start_time).c_str(), allConsumers.at(*(int *)threadIteration).threadID, callBuffer.size(), firstArg);
            pthread_mutex_unlock(&mutex2);

            // add one to receiveNum
            allConsumers.at(*(int *)threadIteration).receiveNum ++;
			Trans(firstArg);
            
            pthread_mutex_lock(&mutex2);
            printf("%s ID= %d \t Complete\t%8d\n",end_time(start_time).c_str(), allConsumers.at(*(int *)threadIteration).threadID, firstArg);
            pthread_mutex_unlock(&mutex2);

            // add one to completeNum
            allConsumers.at(*(int *)threadIteration).completeNum ++;
		}
        else {
            // nothing
        }
	}

    // free up memory
    free(threadIteration);
    return 0;
}

