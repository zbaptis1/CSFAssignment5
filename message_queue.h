#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <deque>
#include <pthread.h>
#include <semaphore.h>
struct Message;


typedef struct {
  void **data;
  unsigned max_items, head, tail;
  sem_t slots, items;
  pthread_mutex_t lock;
} BoundedQueue;

Bounded queue operations:
BoundedQueue *bqueue_create(unsigned max_items);
void bqueue_destroy(BoundedQueue *bq);
void bqueue_enqueue(BoundedQueue *bq, void *item);
void *bqueue_dequeue(BoundedQueue *bq);


// This data type represents a queue of Messages waiting to
// be delivered to a receiver
class MessageQueue {
public:
  MessageQueue();
  ~MessageQueue();

  void enqueue(Message *msg); // will not block
  Message *dequeue();         // blocks for at most a finite amount of time

private:
  // value semantics prohibited
  MessageQueue(const MessageQueue &);
  MessageQueue &operator=(const MessageQueue &);

  // these data members are sufficient to implement the
  // enqueue and dequeue operations: the idea is that the semaphore
  // keeps a count of how many messages are currently in the queue

  pthread_mutex_t m_lock; // must be held while accessing queue
  sem_t m_avail;
  std::deque<Message *> m_messages;
};

#endif // MESSAGE_QUEUE_H
