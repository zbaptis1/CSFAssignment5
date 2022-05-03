#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <iostream>

Room::Room(const std::string &room_name): room_name(room_name) {
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  pthread_mutex_destroy(&lock); 
}

void Room::add_member(User *user) {
  Guard newGuard(lock);
  if (members.find(user) == members.end()) { members.insert(user); }
  else { std::cerr << "Set already has user specified" << std::endl; }
}

void Room::remove_member(User *user) {
  Guard newGuard(lock);
  if (members.find(user) != members.end()) { members.erase(user); } 
  else { std::cerr << "Set does not has specified user" << std::endl; }
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {

  Guard newGuard(lock);
  for (UserIterator s = members.begin(); s != members.end(); s++) {
    User* currUser = *s;

    if (currUser->username != sender_username) {
      Message *newMsg = new Message(TAG_DELIVERY, room_name + ":" + sender_username + ":" + message_text);
      currUser->mqueue.enqueue(newMsg);
    }
  }
}
