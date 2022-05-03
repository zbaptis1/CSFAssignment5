#include <cassert>
#include <ctime>
#include <iostream>

#include "message_queue.h"

MessageQueue::MessageQueue() {
  pthread_mutex_init(&m_lock, nullptr); 
  sem_init(&m_avail, 0, 0); //FROM PIAZZA @937
}

MessageQueue::~MessageQueue() {
  pthread_mutex_destroy(&m_lock);
  sem_destroy(&m_avail);
}

void MessageQueue::enqueue(Message *msg) {
  pthread_mutex_lock(&m_lock);
  m_messages.push_back(msg);
  pthread_mutex_unlock(&m_lock);
  sem_post(&m_avail); 
}

Message *MessageQueue::dequeue() {

  struct timespec ts;

  if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
    std::cerr << "Error handling realtime clock" << std::endl;
    return nullptr;
  }

  ts.tv_sec += 1; //call sem_timedwait to wait up to 1 second for a message to be available
  int retTs = sem_timedwait(&m_avail, &ts);


  // Remove the next message from the queue, return it
  if (retTs != 0 ) { 
    std::cerr << "Dequeue timed out" << std::endl; 
    return nullptr;
  } 
  else { //Dequeue successfully timed
    pthread_mutex_lock(&m_lock);
    Message *msg = m_messages.front();
    m_messages.pop_front();
    pthread_mutex_unlock(&m_lock);
    return msg;
  }
}