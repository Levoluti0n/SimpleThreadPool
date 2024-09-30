#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define T_COUNT 6

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef void*(*job)(void*);
typedef struct node_t node_t;

typedef struct node_t {
    node_t *next;
    job curr_job;
} node_t;

typedef struct queue_t {
    pthread_mutex_t lock;
    node_t* head;
} queue_t;

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t signal;
    int shut_down;
    int queue_size;
    queue_t queue;
    pthread_t tids[T_COUNT];
} tpool;

tpool pool;

void push_back(queue_t* q, job new_job) {
    pthread_mutex_lock(&(q->lock));
    node_t* tmp = q->head;
    if(!(q->head)) {
        q->head = (node_t*)malloc(sizeof(node_t));
        tmp = q->head;
    } else {
        while(tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = (node_t*)malloc(sizeof(node_t));
        tmp = tmp->next;
    }
    tmp->curr_job = new_job;
    tmp->next = NULL;

    pthread_mutex_unlock(&(q->lock));
}
job seek(queue_t *q) {

    pthread_mutex_lock(&(q->lock));
    job res = q->head->curr_job;
    node_t* tmp = q->head->next;
    free(q->head);
    q->head = tmp;

    pthread_mutex_unlock(&(q->lock));
    return res;
}

void* fptr(void *arg) {
    printf("Function Called !\n");
    return NULL;
}

void submit_task(tpool *pool, job new_job) {
    push_back(&pool->queue, fptr);

    pthread_mutex_lock(&pool->lock);
    pool->queue_size++;

    int x = pthread_cond_signal(&pool->signal);
    pthread_mutex_unlock(&pool->lock);
}

void* routine(void* arg) {
    for(;;) {
        pthread_mutex_lock(&pool.lock);
        while(pool.queue_size == 0 && pool.shut_down == 0) {
            pthread_cond_wait(&pool.signal, &pool.lock);
        }
        if(pool.shut_down) {
            pthread_exit(NULL);
        }
        job task = seek(&pool.queue);
        pool.queue_size--;
        task(NULL);

        pthread_mutex_unlock(&pool.lock);
    }
}
void init_thread(tpool* pool) {
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->signal, NULL);
    pool->shut_down = 0;
    pool->queue_size = 0;

    for(int i = 0; i < T_COUNT; ++i) {
        pthread_create(&pool->tids[i], NULL, routine, NULL);
        pthread_detach(pool->tids[i]);
    }
}

void pool_destroy(tpool* pool){
    pool->shut_down = 1;
    pthread_cond_broadcast(&pool->signal);

    pthread_cond_destroy(&pool->signal);
    pthread_mutex_destroy(&pool->lock);
}

int main() {
    queue_t q;
    q.head = NULL;
    q.lock = lock;

    
    init_thread(&pool);
    

    for (int i = 0; i < 512; ++i){
        submit_task(&pool, fptr);
        // printf("%d JOB ADDED\n", i);
    }

    sleep(4);
    printf("\n REMAIN %p", q.head);
    pool_destroy(&pool);   
    return 0;
}