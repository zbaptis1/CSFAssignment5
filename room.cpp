#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <iostream>

/** NOTES:

  - Look at piazza 938 for broadcasting
  - look for other piazza posts
  - Understand role of semaphore and mutexes in room environment
    (LOOK AT PROMPT DIAGRAM)
*/

Room::Room(const std::string &room_name): room_name(room_name) {
  // TODO: initialize the mutex
  /** pthread_mutexattr_t mattr; TODO: see if this is needed for 2nd param, if so HOW */
  pthread_mutex_init(&lock, nullptr); /** TODO: figure out what 2nd arg should be */
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_destroy(&lock); /** TODO: see if theres a special case AND if there's any other args to pass*/
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  Guard newGuard(lock);
  if (members.find(user) == members.end()) { members.insert(user); }
  else { std::cerr << "Set already has user specified" << std::endl; }
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  Guard newGuard(lock);
  if (members.find(user) != members.end()) { members.erase(user); } /** TODO: see if theres a special case */
  else { std::cerr << "Set does not has specified user" << std::endl; }
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room

  Guard newGuard(lock);
  for (UserIterator s = members.begin(); s != members.end(); s++) {
    User* currUser = *s;

    if (currUser->username != sender_username) {
      Message *newMsg = new Message(TAG_DELIVERY, room_name + ":" + sender_username + ":" + message_text);
      currUser->mqueue.enqueue(newMsg);
    }
  }
}
