#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mainMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker = PTHREAD_MUTEX_INITIALIZER;

//actors conditionals
pthread_cond_t agent_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t tobaccoSmoker_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t matchSmoker_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t paperSmoker_c = PTHREAD_COND_INITIALIZER;

//resource conditionals
pthread_cond_t tobacco = PTHREAD_COND_INITIALIZER;
pthread_cond_t paper = PTHREAD_COND_INITIALIZER;
pthread_cond_t match = PTHREAD_COND_INITIALIZER;

int hasTobacco = 0;
int hasPaper = 0;
int hasMatch = 0;

int agentHasJob = 1;
int tobaccoSmokerHasJob = 0;
int matchSmokerHasJob = 0;
int paperSmokerHasJob = 0;

//**************utils**********
int getRand(int range) {
    return rand() % range;
}

void waitOn(pthread_cond_t *c, pthread_mutex_t * m) {
    pthread_cond_wait(c, m);
}

void P(int * j, pthread_cond_t * threadCond, pthread_mutex_t * m) {
    pthread_mutex_lock(m);
    while (*j == 0) {
        waitOn(threadCond, m);
    }
}

void V(pthread_mutex_t * m) {
    pthread_mutex_unlock(m);
}

void wake(pthread_cond_t *c) {
    pthread_cond_signal(c);
}

void tryStartThread(pthread_t * t, void * f){
    if (pthread_create(t, NULL, f, NULL) != 0) {
        fprintf (stderr, "Cannot create thread\n");
        exit (1);
    }
}

//**************main agent*******************

void * agent(void * arg) {

    while (1) {
        sleep(1);

        P(&agentHasJob, &agent_c, &mainMutex);

        // while (agentHasJob == 0) {
        //     waitOn(&agent_c, &mainMutex);
        // }

        printf("-------------------------------\n");

        printf("agent stuff ******************************\n");

        int randNum = getRand(3);

        if (randNum == 0) {

            agentHasJob = 0;
            hasMatch = 1;
            hasPaper = 1;
            printf("Put paper and match\n");
            wake(&paper);
            wake(&match);

        } else if (randNum == 1) {
            agentHasJob = 0;
            hasMatch = 1;
            hasTobacco = 1;
            printf("Put tobacco and match\n");
            wake(&paper);
            wake(&match);
        } else if (randNum == 2 ) {
            agentHasJob = 0;
            hasTobacco = 1;
            hasPaper = 1;
            printf("Put paper and tobacco\n");
            wake(&paper);
            wake(&tobacco);
        }

        printf("******************************\n");
        V(&mainMutex);
    }
    return 0;
}

// **********************PUSHERS********************

void * paperPusher(void * arg) {
    while (1) {

        P(&hasPaper, &paper, &mainMutex);

        usleep(getRand(200) * 1000);
        while (hasPaper == 0) {
            //wait until we are called an allowed to proceed
            waitOn(&paper, &mainMutex);
        }

        if (hasMatch == 1) {
            hasMatch = 0;
            agentHasJob = 0;
            tobaccoSmokerHasJob = 1;
            printf("Call the tobacco smoker\n");
            wake(&tobaccoSmoker_c);
        }

        if (hasTobacco == 1) {
            hasTobacco = 0;
            agentHasJob = 0;
            matchSmokerHasJob = 1;
            printf("Call the match smoker\n");
            wake(&matchSmoker_c);
        }
        V(&mainMutex);
    }

    return 0 ;
}

void * matchPusher(void * arg) {

    while (1) {
        P(&hasMatch, &match, &mainMutex);

        usleep(getRand(200) * 1000);

        // while (hasMatch == 0)
        //     waitOn(&match, &mainMutex);

        if (hasPaper == 1) {
            hasPaper = 0;
            agentHasJob = 0;
            tobaccoSmokerHasJob = 1;
            printf("Call the tobacco smoker\n");
            wake(&tobaccoSmoker_c);
        }
        if (hasTobacco == 1) {
            hasTobacco = 0;
            agentHasJob = 0;
            paperSmokerHasJob = 1;
            printf("Call the paper smoker\n");
            wake(&paperSmoker_c);
        }
        V(&mainMutex);
    }

    return 0 ;
}

void * tobaccoPusher(void * arg) {
    while (1) {
        P(&hasTobacco, &tobacco, &mainMutex);

        usleep(getRand(200) * 1000);

        // while (hasTobacco == 0)
        //     waitOn(&tobacco, &mainMutex);

        if (hasMatch == 1) {
            hasMatch = 0;
            agentHasJob = 0;
            paperSmokerHasJob = 1;
            printf("Call the paper smoker\n");
            wake(&paperSmoker_c);
        }
        if (hasPaper == 1) {
            hasTobacco = 0;
            agentHasJob = 0;
            matchSmokerHasJob = 1;
            printf("Call the match smoker\n");
            wake(&matchSmoker_c);
        }
        V(&mainMutex);
    }
    return 0 ;
}

// **************SMOKERS********************

void * tobaccoSmoker(void * arg) {

    while (1) {

        P(&tobaccoSmokerHasJob, &tobaccoSmoker_c, &smoker);

        hasPaper = 0;
        hasMatch = 0;
        tobaccoSmokerHasJob = 0;
        agentHasJob = 1;
        printf("Tobacco Smoker: making & smoking cigarette...\n");
        usleep(getRand(50) * 1000);
        
        V(&smoker);
        printf("Tobacco Smoker: Smoked...\n");
    }

    return 0;
}

void * paperSmoker(void * arg) {

    while (1) {

        P(&paperSmokerHasJob, &paperSmoker_c, &smoker);
        // while (paperSmokerHasJob == 0) {
        //     waitOn(&paperSmoker_c, &smoker);
        // }

        hasTobacco = 0;
        hasMatch = 0;
        paperSmokerHasJob = 0;
        agentHasJob = 1;
        printf("Paper Smoker: making & smoking cigarette...\n");
        usleep(getRand(50) * 1000);
        
        V(&smoker);
        printf("Paper Smoker: Smoked...\n");
    }

    return 0;
}

void * matchSmoker(void * arg) {

    while (1) {

        P(&matchSmokerHasJob, &matchSmoker_c, &smoker);
        // while (matchSmokerHasJob == 0) {
        //     waitOn(&matchSmoker_c, &smoker);
        // }

        hasPaper = 0;
        hasTobacco = 0;
        matchSmokerHasJob = 0;
        agentHasJob = 1;
        printf("Match Smoker: making & smoking cigarette...\n");
        usleep(getRand(50) * 1000);
        
        V(&smoker);
        printf("Match Smoker: Smoked...\n");
    }

    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t
    agentThread,

    tobaccoSmoker_t,
    paperSmoker_t,
    matchSmoker_t,

    tobaccoPusher_t,
    paperPusher_t,
    matchPusher_t;

    //random seed
    time_t t;
    srand((unsigned) time(&t));

    tryStartThread(&agentThread, agent);
    tryStartThread(&tobaccoPusher_t, tobaccoPusher);
    tryStartThread(&paperPusher_t, paperPusher);
    tryStartThread(&matchPusher_t, matchPusher);
    tryStartThread(&tobaccoSmoker_t, tobaccoSmoker);
    tryStartThread(&paperSmoker_t, paperSmoker);
    tryStartThread(&matchSmoker_t, matchSmoker);

    pthread_join(agentThread, NULL);
    pthread_join(tobaccoPusher_t, NULL);
    pthread_join(paperPusher_t, NULL);
    pthread_join(matchPusher_t, NULL);
    pthread_join(tobaccoSmoker_t, NULL);
    pthread_join(paperSmoker_t, NULL);
    pthread_join(matchSmoker_t, NULL);

}