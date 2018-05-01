#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

sem_t agentSem = 1,
	tobacco = 0,
	paper = 0,
	match = 0;

bool isTobacco = false,
	isPaper = false,
	isMatch = false;

sem_t tobaccoSem = 0,
	 paperSem = 0,
	 matchSem = 0,
	 mutex = 1;

int getRand(int range) {
    return rand() % range;
}

void waitRandMili(int max){
	//fast mode
	usleep(getRand(max) * 1000);

	//slow mode
	// usleep(getRand(max) * 100000);
}

void P(sem_t * m) {
	sem_wait(m);
}

void V(sem_t * m) {
	sem_post(m);
}

void tryStartThread(pthread_t * t, void * f){
    if (pthread_create(t, NULL, f, NULL) != 0) {
        fprintf (stderr, "Cannot create thread\n");
        exit (1);
    }
}

bool isPaperHelper(){
	if (isPaper){
		isPaper = false;
		return true;
	}
	return false;
}

bool isMatchHelper(){
	if (isMatch){
		isMatch = false;
		return true;
	}
	return false;
}

bool isTobaccoHelper(){
	if (isTobacco){
		isTobacco = false;
		return true;
	}
	return false;
}

// AGENTS******************************
void * agentTP(void * arg) {
	int counter = 0;
	while (counter <= 5) {
		waitRandMili(200);
		P(&agentSem);
		printf("Agent placed: TP\n");
		counter++;
		V(&tobacco);
		V(&paper);
	}
	printf("agentTP exiting\n");
	return NULL;
}

void * agentTM(void * arg) {
	int counter = 0;
	while (counter <= 5) {
		waitRandMili(200);
		P(&agentSem);
		printf("Agent placed: TM\n");
		counter++;
		V(&tobacco);
		V(&match);
	}
	printf("agentTM exiting\n");
	return NULL;
}

void * agentPM(void * arg) {
	int counter = 0;
	while (counter <= 5) {
		waitRandMili(200);
		P(&agentSem);
		printf("Agent placed: PM\n");
		counter++;
		V(&paper);
		V(&match);
	}
	printf("agentPM exiting\n");
	return NULL;
}

//PUSHERS*********************

void * pusherT(void * arg) {
	int counter = 0;
	while (counter <= 12) {
		P(&tobacco);
		P(&mutex);
		if (isPaperHelper()) {
			V(&matchSem);
		}
		else if (isMatchHelper()) {
			V(&paperSem);
		}
		else {
			isTobacco = true;
		}
		V(&mutex);
		counter++;
	}
	printf("pusherT exiting\n");
	return NULL;
}

void * pusherP(void * arg) {
	int counter = 0;
	while (counter <= 12) {
		P(&paper);
		P(&mutex);
		if (isTobaccoHelper()) {
			V(&matchSem);
		}
		else if (isMatchHelper()) {
			V(&tobaccoSem);
		}
		else {
			isPaper = true;
		}
		V(&mutex);
		counter++;
	}
	printf("pusherP exiting\n");
	return NULL;
}

void * pusherM(void * arg) {
	int counter = 0;
	while (counter <= 12) {
		P(&match);
		P(&mutex);
		if (isTobaccoHelper()) {
			V(&paperSem);
		}
		else if (isPaperHelper()) {
			V(&tobaccoSem);
		}
		else {
			isMatch = true;
		}
		V(&mutex);
		counter++;
	}
	printf("pusherM exiting\n");
	return NULL;
}

//SMOKERS***********************************

void * smokerT(void * arg) {
	int counter = 0;
	while (counter <= 2) {
		P(&tobaccoSem);
		//rolling
		printf("T Smoker: Rolling\n");
		waitRandMili(50);
		V(&agentSem);
		//smoking
		printf("T Smoker: Smoking\n");
		waitRandMili(50);
		counter++;
	}
	printf("smokerT exiting, smoked:%d\n", counter);
	return NULL;
}

void * smokerP(void * arg) {
	int counter = 0;
	while (counter <= 2) {
		P(&paperSem);
		//rolling
		printf("P Smoker: Rolling\n");
		waitRandMili(50);
		V(&agentSem);
		//smoking
		printf("P Smoker: Smoking\n");
		waitRandMili(50);
		counter++;
	}
	printf("smokerP exiting, smoked:%d\n", counter);
	return NULL;
}

void * smokerM(void * arg) {
	int counter = 0;
	while (counter <= 2) {
		P(&matchSem);
		//rolling
		printf("M Smoker: Rolling\n");
		waitRandMili(50);
		V(&agentSem);
		//smoking
		printf("M Smoker: Smoking\n");
		waitRandMili(50);
		counter++;
	}
	printf("smokerM exiting, smoked:%d\n", counter);
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t
		agentTP_t,
		agentTM_t,
		agentPM_t,

		pusherT_t,
		pusherP_t,
		pusherM_t,

		smokerT_t,
		smokerP_t,
		smokerM_t;

    tryStartThread(&agentTP_t, agentTP);
    tryStartThread(&agentTM_t, agentTM);
    tryStartThread(&agentPM_t, agentPM);

    tryStartThread(&pusherT_t, pusherM);
    tryStartThread(&pusherP_t, pusherP);
    tryStartThread(&pusherM_t, pusherT);

    tryStartThread(&smokerT_t, smokerT);
    tryStartThread(&smokerP_t, smokerP);
    tryStartThread(&smokerM_t, smokerM);

    //twice as many smokers
    tryStartThread(&smokerT_t, smokerT);
    tryStartThread(&smokerP_t, smokerP);
    tryStartThread(&smokerM_t, smokerM);

    pthread_join(agentTP_t, NULL);
    pthread_join(agentTM_t, NULL);
    pthread_join(agentPM_t, NULL);

    pthread_join(pusherT_t, NULL);
    pthread_join(pusherP_t, NULL);
    pthread_join(pusherM_t, NULL);

    pthread_join(pusherT_t, NULL);
    pthread_join(pusherP_t, NULL);
    pthread_join(pusherM_t, NULL);

    

}