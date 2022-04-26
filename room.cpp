#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name): room_name(room_name) {
  // TODO: initialize the mutex
  lock(room_name); /** TODO: use the mutex lock field "lock" */
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_destroy(lock); /** TODO: see if theres a special case */
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  if (!memeber.has(user)) members.add(user); 
  else std::cerr << "Set already has user specified" << std::endl;
   
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  if (memeber.has(user)) members.remove(user); /** TODO: see if theres a special case */
  else std::cerr << "Set does not has specified user" << std::endl;

}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room

  /** TODO: Notes


  */

}
