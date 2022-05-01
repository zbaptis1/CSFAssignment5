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
  if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
    std::cerr << "Error handling realtime clock" << std::endl;
    return nullptr;
  }

  ts.tv_sec += 1; //call sem_timedwait to wait up to 1 second for a message to be available
  int retTs = sem_timedwait(&full, &ts);

/** TODO: see if you need to check this error */
//   while ((retTs = sem_timedwait(&full, &ts)) == -1 && errno == EINTR)
//       continue;       /* Restart if interrupted by handler */
//   /* Check what happened */
//   if (s == -1)
//   {
//     if (errno == ETIMEDOUT)
//         printf("sem_timedwait() timed out\n");
//     else
//         perror("sem_timedwait");
// } else
//         printf("sem_timedwait() succeeded\n");

  Message *msg = nullptr;

  // Remove the next message from the queue, return it
  if (retTs == -1) { std::cerr << "Dequeue timed out" << std::endl;} 
  else { //Dequeue successfully timed
    msg = m_messages.front();
    m_messages.pop();
    ts.tv_sec += 1;
  }

  return msg;
}