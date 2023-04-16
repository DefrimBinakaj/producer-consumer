#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/resource.h>
#include <bits/stdc++.h>
#include <map>
#include <pthread.h>
#include <chrono>
#include <fstream>
#include <semaphore.h>

#include "tands.h"
#include "common.h"
#include "commands.h"

using namespace std;



// start time of entire application
chrono::system_clock::time_point start_time = std::chrono::system_clock::now();



// number of threads; from user input from cli
int numThreads;



// time of the last work call; used for trans per second calc
string lastWorkCall;


// create producerInfo instance
producerInfo producerID;

// https://cplusplus.com/forum/beginner/3396/
// create consumerInfo vector for all the consumer threads; random element access required
vector<consumerInfo> allConsumers;

// make a queue for this assignment:
// will make it very easy because of FIFO; can pop
// the first element while adding other elements to the end
queue<int> callBuffer;
queue<char> argBuffer;


// sem / mutex inits
sem_t semEmpty; // buffer empty
sem_t semFull; // buffer full
// https://www.ibm.com/docs/en/i/7.3?topic=ssw_ibm_i_73/apis/users_61.htm
pthread_mutex_t mutex1; // code manipulation critSec
pthread_mutex_t mutex2; // writing to the file



// main function
int main(int argc, char **argv) {

    // logic for determining file name and output rewriting
	if (argc < 2 || argc > 3) {
		printf("Please write either one or two arguments\n");
		exit(-1);
	}
    // number of threads is first arg
	numThreads = atoi(argv[1]);

	int fileID = 0;

	char fileName[50];

    // file number is second arg
	if (argc == 3) {
		fileID = atoi(argv[2]);
		sprintf(fileName, "prodcon.%d.log",fileID);
		freopen(fileName, "w", stdout);
	}
	else {
		freopen("prodcon.log", "w", stdout);
	}


	// semaphore inits
	sem_init(&semEmpty, 0, numThreads*2);
    sem_init(&semFull, 0, 0);


	// Launch the producers / consumers
    pthread_t prodThread; // producers
	pthread_t threads[numThreads]; // consumers


    // set producerID attrs to 0 to enable calcs w/o error
    producerID.workNum = 0;
    producerID.sleepNum = 0;
	// make producer thread
    if (pthread_create(&prodThread, NULL, producer, NULL) != 0) {
        perror("Thread create error");
    }

    // make consumer threads
    for (int i = 0; i < numThreads; i++) {
        // init consumerInfo structs
        consumerInfo consumerID;
        // set all values to zero so that we can add without errors
        consumerID.threadID = 0;
        consumerID.askNum = 0;
        consumerID.receiveNum = 0;
        consumerID.completeNum = 0;
        allConsumers.push_back(consumerID);
        //https://stackoverflow.com/questions/39610372/passing-a-single-integer-to-a-multithread-function-in-c
        // pass in i as the id of each thread -> later added 1 to avoid error
        int* threadArg = new int(i);
        if (pthread_create(&threads[i], NULL, &consumer, threadArg) != 0) {
            perror("Thread create error");
        }
    }

	// wait for producer
    if (pthread_join(prodThread, NULL) != 0) {
        perror("Thread create error");
    }
	// wait for consumers
    for (int i = 0; i < numThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Thread create error");
        }
    }

    // this is the time of the last line printed
    lastWorkCall = end_time(start_time).c_str();

    // sem / mutex destroys
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    // summary print
	printf("Summary: \n");
    // init the total amounts, in which specific attrs of the vector of consumer threads are summed
    int totalAskNum = 0;
    int totalReceiveNum = 0;
    int totalCompleteNum = 0;
    for (int i = 0; i < numThreads; i++) {
        totalAskNum += allConsumers.at(i).askNum;
        totalReceiveNum += allConsumers.at(i).receiveNum;
        totalCompleteNum += allConsumers.at(i).completeNum;
    }
    // print each count
    printf("\tWork \t\t%d\n", producerID.workNum);
    printf("\tAsk \t\t%d\n", totalAskNum);
    printf("\tReceive \t%d\n", totalReceiveNum);
    printf("\tComplete \t%d\n", totalCompleteNum);
    printf("\tSleep \t\t%d\n", producerID.sleepNum);
    for (int i = 0; i < numThreads; i++) {
        printf("\tThread %d\t%d\n", allConsumers.at(i).threadID, allConsumers.at(i).completeNum);
    }

    // using last possible time, convert to float and calculate trans per second, to 2 dec places    
    // https://www.programiz.com/cpp-programming/string-float-conversion
    float lastWorkCallFloat = stof(lastWorkCall);
    float dispLastWork = producerID.workNum / lastWorkCallFloat;
    printf("Transactions per second: %.2f", dispLastWork);

	return 0;
}





