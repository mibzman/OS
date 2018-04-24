pthread_mutex_t main_m = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t tobacco =    PTHREAD_COND_INITIALIZER;
pthread_cond_t paper =  PTHREAD_COND_INITIALIZER;
pthread_cond_t match =  PTHREAD_COND_INITIALIZER;

int agent_has_job = 1;

int have_tobacco = 0;
int have_paper = 0;
int have_match = 0;


void * agent(void * arg){
    while(1) {
        
        sleep(1);
        
        pthread_mutex_lock(&main_m);
        
        //The agent stays waiting if agent_has_job is equal 0
        while(agent_has_job == 0)
            pthread_cond_wait(&agent_c, &main_m);
        
        
        printf("-------------------------------\n");
        
        int randNum = getRand(3);
        
        // signil what is needed
        if (randNum == 0) {
           
            agent_has_job = 0;
            have_match = 1;
            have_paper = 1;
            puts("Put paper and match");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&match);

        } else if (randNum == 1) {

            agent_has_job = 0;
            have_match = 1;
            have_tobacco = 1;
            puts("Put tobacco and match");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&match);
        } else if (randNum == 2) {

            agent_has_job = 0;
            have_tobacco = 1;
            have_paper = 1;
            puts("Put paper and tobacco");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&tobacco);
        }
        
        pthread_mutex_unlock(&main_m);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t agent, smoker_tobacco, smoker_paper, smoker_match, pusher_tobacco, pusher_paper, pusher_match;
    
    //random seed
    time_t t;
    srand((unsigned) time(&t));
    
    if (pthread_create(&agent,NULL,agent,NULL) != 0) {
        fprintf (stderr, "Cannot create agent thread\n");
        exit (1);
    }
    
    if (pthread_create(&pusher_tobacco,NULL,pusher_tobacco,NULL) != 0) {
        fprintf (stderr, "Cannot create pusher_tobacco thread\n");
        exit (1);
    }
    
    if (pthread_create(&pusher_paper,NULL,pusher_paper,NULL) != 0) {
        fprintf (stderr, "Cannot create pusher_paper thread\n");
        exit (1);
    }
    
    if (pthread_create(&pusher_match,NULL,pusher_match,NULL) != 0) {
        fprintf (stderr, "Cannot create pusher_match thread\n");
        exit (1);
    }
    
    if (pthread_create(&smoker_tobacco,NULL,smoker_tobacco,NULL) != 0) {
        fprintf (stderr, "Cannot create smoker_tobacco thread\n");
        exit (1);
    }
    
    if (pthread_create(&smoker_paper,NULL,smoker_paper,NULL) != 0) {
        fprintf (stderr, "Cannot create smoker_paper thread\n");
        exit (1);
    }
    
    if (pthread_create(&smoker_match,NULL,smoker_match,NULL) != 0) {
        fprintf (stderr, "Cannot create smoker_match thread\n");
        exit (1);
    }
    
    pthread_join(agent, NULL);
    pthread_join(pusher_tobacco, NULL);
    pthread_join(pusher_paper, NULL);
    pthread_join(pusher_match, NULL);
    pthread_join(smoker_tobacco, NULL);
    pthread_join(smoker_paper, NULL);
    pthread_join(smoker_match, NULL);

}