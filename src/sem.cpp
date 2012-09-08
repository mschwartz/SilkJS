/** @ignore */
#include "SilkJS.h"
#include <semaphore.h>

static sem_t mutex;

static JSVAL sem_Init (JSARGS args) {
    HandleScope scope;
    int ret;
    if ((ret = sem_init(&mutex, 1, 1)) < 0) {
        perror("sem_init");
        return False();
    }
    return True();
}

static JSVAL sem_Destroy (JSARGS args) {
    return Integer::New(sem_destroy(&mutex));
}

static JSVAL sem_Wait (JSARGS args) {
    int ret = sem_wait(&mutex);
    printf("wait %d\n", ret);
    return Integer::New(ret);
}

static JSVAL sem_Post (JSARGS args) {
    return Integer::New(sem_post(&mutex));
}

void init_sem_object () {
    Handle<ObjectTemplate>sem = ObjectTemplate::New();
    sem->Set(String::New("init"), FunctionTemplate::New(sem_Init));
    sem->Set(String::New("destroy"), FunctionTemplate::New(sem_Destroy));
    sem->Set(String::New("wait"), FunctionTemplate::New(sem_Wait));
    sem->Set(String::New("post"), FunctionTemplate::New(sem_Post));

    builtinObject->Set(String::New("sem"), sem);
}
