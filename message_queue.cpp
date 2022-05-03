#include <cassert>
#include <ctime>
#include <iostream>

#include "message_queue.h"

MessageQueue::MessageQueue() {
  // TODO: initialize the mutex and the semaphore
  pthread_mutex_init(&m_lock, nullptr); 
  sem_init(&m_avail, 0, 0); //FROM PIAZZA @937
}

MessageQueue::~MessageQueue() {
  // TODO: destroy the mutex and the semaphore
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  // TODO: put the specified message on the queue
  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
  pthread_mutex_lock(&m_lock);
  m_messages.push_back(msg);
  pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail); //Need to user sem_post to notify queue
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
  int retTs = sem_timedwait(&m_avail, &ts);

  Message *msg = nullptr;

  // Remove the next message from the queue, return it
  if (retTs != 0 ) { std::cerr << "Dequeue timed out" << std::endl; } 
  else { //Dequeue successfully timed
    Guard newGuard(m_lock);

    pthread_mutex_lock(&m_lock);
    msg = m_messages.front();
    m_messages.pop_front();
    pthread_mutex_unlock(&m_lock);
  }

  return msg;
}