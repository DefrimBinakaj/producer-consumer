#ifndef COMMON_H
#define COMMON_H

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

#include "commands.h"
#include "tands.h"

using namespace std;


// common header file used for scoping issues:
// https://stackoverflow.com/questions/18233566/variable-in-header-file-not-declared-in-scope
// this header is used for producer.cpp and consumer.cpp

// start time init
extern chrono::system_clock::time_point start_time;

// numThreads init
extern int numThreads;


// info of producer thread (used for counts)
struct producerInfo {
    int workNum;
    int sleepNum;
};

// producerID init
extern producerInfo producerID;


// info of consumer threads (used for id and counts)
struct consumerInfo {
    int threadID;
    int askNum;
    int receiveNum;
    int completeNum;
};


// lastWorkCall init
extern string lastWorkCall;

// allConsumers init
extern vector<consumerInfo> allConsumers;

// buffer inits
extern queue<int> callBuffer;
extern queue<char> argBuffer;


// sem / mutex inits
extern sem_t semEmpty;
extern sem_t semFull;
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;

// producer function init
void *producer(void *threadIteration);

// consumer function init
void *consumer(void *threadIteration);



#endif