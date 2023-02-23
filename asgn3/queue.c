#include "queue.h"
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <assert.h>

struct queue {
    int size;
    int in;
    int out;
    void **buffer;
    sem_t fullSlots;
    sem_t emptySlots;
    sem_t mutex;
};

queue_t *queue_new(int size) {
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (q) {
        q->size = size;
        q->in = 0;
        q->out = 0;
        q->buffer = (void **) calloc(size, sizeof(void *));
        if (!q->buffer) {
            free(q);
            q = NULL;
        }
        int rc;
        rc = sem_init(&(q->fullSlots), 0, size);
        assert(!rc);
        rc = sem_init(&(q->emptySlots), 0, 0);
        assert(!rc);
        rc = sem_init(&(q->mutex), 0, 1);
        assert(!rc);
    }

    return q;
}

void queue_delete(queue_t **q) {
    if (*q && (*q)->buffer) {
        int rc;
        rc = sem_destroy(&(*q)->fullSlots);
        assert(!rc);
        rc = sem_destroy(&(*q)->emptySlots);
        assert(!rc);
        rc = sem_destroy(&(*q)->mutex);
        assert(!rc);
        free((*q)->buffer);
        (*q)->buffer = NULL;
        free(*q);
        *q = NULL;
    }
    return;
}

bool queue_push(queue_t *q, void *elem) {
    if (q == NULL) {
        return (false);
    }
    sem_wait(&(q->fullSlots));
    sem_wait(&(q->mutex));
    q->buffer[q->in] = elem;
    q->in = (q->in + 1) % (q->size);
    sem_post(&(q->mutex));
    sem_post(&(q->emptySlots));
    return (true);
}

bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL) {
        return (false);
    }
    sem_wait(&(q->emptySlots));
    sem_wait(&(q->mutex));
    *elem = q->buffer[q->out];
    q->out = (q->out + 1) % (q->size);
    sem_post(&(q->mutex));
    sem_post(&(q->fullSlots));
    return (true);
}
