
void* santaTask(void* args){
    shared_data* data = (shared_data*) args;
    sem_wait(data->santaSem);
    if(data->reindeer >= 9){
        prepareSleigh();
        for(int i=0;i<data->reindeer;++i){
        sem_post(data->reindeerSem);
        }
        data->reindeer -= 9;
    }
    else if(data->elves == 3){
        helpElves();
    }
}

void* reindeerTask(void* args){
    randomSleep(15000,35000);
    shared_data* data = (shared_data*) args;
    if(data->reindeer >= 9){
        sem_post(data->santaSem);
    }
    sem_wait(data->reindeerSem);
    getHitched();
}









