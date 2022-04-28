#include <cassert>
#include <ctime>
#include "message_queue.h"

MessageQueue::MessageQueue() {
  // TODO: initialize the mutex and the semaphore
  ptthread_mutex_init(&m_lock, -1); /** TODO: figure out what 2nd arg should be */
  sem_init(m_avail, 0, 0); //FROM PIAZZA @937
}

MessageQueue::~MessageQueue() {
  // TODO: destroy the mutex and the semaphore
  Sem_destroy(m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  // TODO: put the specified message on the queue

  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post

  m_messages.add(msg);
  sem_post(); /** TODO: FROM DIAGRAM, need to user sem_post to notify queue (FIGURE OUT ARGS) */
}

Message *MessageQueue::dequeue() {
  struct timespec ts;

  // get the current time using clock_gettime:
    // we don't check the return value because the only reason
    // this call would fail is if we specify a clock that doesn't
    // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  // TODO: call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available

  // TODO: remove the next message from the queue, return it
  Message *msg = nullptr;
  sem_timedwait();
  msg =  m_messages.front(); /** TODO: Figure out how to remove next element in deuque */
  delete msg;
  msg = nullptr;

  return msg;
}
