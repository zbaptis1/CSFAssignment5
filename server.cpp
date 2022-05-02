#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

/** NOTES:

  - Keep limit on # of threads created
  - 
*/

namespace {
  void *worker(void *arg) {
    pthread_detach(pthread_self());
    ConnInfo *info_ = static_cast<ConnInfo *>(arg);
    std::unique_ptr<ConnInfo> info(info_);

    info->conn->connect(info->server->getMssock(), info->server->getMport()); //Connect server using server fields given
    Message msg;



    /** TODO: need to do error checking w/ this BOOL function */
    if (!info->conn->receive(msg)) { //1st receive is not a login
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
        info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      return nullptr;
    }

    /** TODO: have error checking for what a correct user login is, then put logic here like previous error */

    std::string userLogin = msg.data;
    User *thisUser = new User(userLogin); //Dynamically make user on top level of thread, then pass to sender/receiver functions

    if (!info->conn->send(Message(TAG_OK, "Welcome " + userLogin))) return nullptr; //Understand this logic for other send requests

    if (msg.tag == TAG_RLOGIN) { 
      info->server->chat_with_receiver(thisUser, info->conn, info->server); 
    } else if (msg.tag == TAG_SLOGIN) {
      info->server->chat_with_sender(thisUser, info->conn, info->server);
    };

    // TODO: use a static cast to convert arg from a void* to
    //       whatever pointer type describes the object(s) needed
    //       to communicate with a client (sender or receiver)

    // TODO: read login message (should be tagged either with
    //       TAG_SLOGIN or TAG_RLOGIN), send response

    // TODO: depending on whether the client logged in as a sender or
    //       receiver, communicate with the client (implementing
    //       separate helper functions for each of these possibilities
    //       is a good idea)

    delete thisUser;
    return nullptr;
  }
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port): m_port(port), m_ssock(-1) {
  // TODO: initialize mutex
  
  /** pthread_mutexattr_t mattr; TODO: see if this is needed for 2nd param, if so HOW */
  pthread_mutex_init(&m_lock, NULL); /** TODO: figure out mutex in threads */
} 

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not

  //FROM LECTURE 27: APP PROTOCOLS {SLIDE 23}
  std::string portStr = std::to_string(m_port);
  m_ssock = open_listenfd(portStr.c_str()); /** TODO: I believe server_fd = m_ssock (intuition AND todo comment above) */ 
  if (m_ssock < 0) { 
    std::cerr << "ERROR Couldn't listen to server" << std::endl;
    return false;
  } 
  
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client

  //FROM LECTURE 27: APP PROTOCOLS {SLIDE 25}

  Connection conn;

  while (1) {

    // MAYBE LAST 2 ARGS AREN'T NECESSARY: Accept(int s, struct sockaddr *addr, socklen_t *addrlen) 
    int client_fd = Accept(m_ssock, NULL, NULL);
    if (client_fd > 0) {
      /** TODO: make a connection object ???    */

      ConnInfo *info = new ConnInfo(new Connection(client_fd), this);

      pthread_t thr_id;
      if (pthread_create(&thr_id, nullptr, worker, static_cast<void *>(info)) < 0) { // static cast? why not just info
        std::cerr << "Could not create thread\n";
        delete info;
      }

      // close(client_fd); /** TODO: see if necessary b/c worker might already handle connection closure w/ deconstructor

    } else std::cerr << "ERROR opening a connection" << std::endl;
  } 

  return;
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  // guard g(m_lock) --> idky david has this


  // RoomMap::iterator works, could use that be would that be sus?
  
  if (m_rooms.find(room_name) != m_rooms.end()) return m_rooms[room_name]; //Use arrow-> reference to get value from iterator pointing to pair
  
  Room* newRoom = new Room(room_name); //Create new room if not found in map
  m_rooms[room_name] = newRoom; //Add to map of rooms

  return newRoom;
}

void Server::chat_with_sender(User* user, Connection *conn, Server *server) {
  /** TODO: figure out implementation */

  Message msg;
  Room * room = nullptr;

  while (1) {
    if (!conn->receive(msg)) {
      Connection::Result last_result = conn->get_last_result();
      if (last_result == Connection::INVALID_MSG) {
        conn->send(Message(TAG_ERR, "invalid message"));
        // can try again
      } else if (last_result == Connection::EOF_OR_ERROR) {
        conn->send(Message(TAG_ERR, "EOF or Error"));
        break;
      }
    }

    //  join
    if (msg.tag == TAG_JOIN) {
      std::string roomName = msg.data;
      // check for valid room name
      if (!isValidRoomName(roomName)) {
        conn->send(Message(TAG_ERR, "invalid room name"));
      } 
      else {
        if (room != nullptr) { room->remove_member(user); } // sender is in a room already
        room = server->find_or_create_room(roomName);
        room->add_member(user);
        conn->send(Message(TAG_OK, "succesfully joined " + roomName + "!"));
      }
    }
    // leave
    else if (msg.tag == TAG_LEAVE) {
      if (room == nullptr) { conn->send(Message(TAG_ERR, "not in any room")); }
      else {
        room->remove_member(user);
        conn->send(Message(TAG_OK, user->username + " has left the room " + room->get_room_name()));
        room = nullptr;
      }
    }
    // quit
    else if (msg.tag == TAG_QUIT) {
      conn->send(Message(TAG_OK, "quit"));
      break;
    }
    // sendall
    else if (msg.tag == TAG_SENDALL) {
      if (room == nullptr) { conn->send(Message(TAG_ERR, "not in any room")); }
      else {
        room->broadcast_message(user->username, msg.data);
        conn->send(Message(TAG_OK, "message was sent to all users"));
      }
    }
    // invalid message tag
    else { conn->send(Message(TAG_ERR, "invalid message tag")); }

  }
    // close the room as the user is done using it
    if (room) { room->remove_member(user); }
}


void Server::chat_with_receiver(User *user, Connection *conn, Server *server) {
    /** TODO: figure out implementation */


  /** TODO: need to do error checking w/ this BOOL function */
  
  Message msg;

  if (!conn->receive(msg)) { //1st receive is not a login
    Connection::Result last_result = conn->get_last_result();
    if (last_result == Connection::INVALID_MSG) {
      conn->send(Message(TAG_ERR, "invalid message"));
      return;
    } else if (last_result == Connection::EOF_OR_ERROR) {
    /** TODO: send error */
      conn->send(Message(TAG_ERR, "EOF or Error"));
      return;
    }
  }

  if (msg.tag != TAG_JOIN) {
    conn->send(Message(TAG_ERR, "need to send a join message first"));
    return;
  }


  std::string room_name = msg.data;
  if (!isValidRoomName(room_name)) {  // add room error checking 
    conn->send(Message(TAG_ERR, "invalid room name"));
    return;
  }


  Room *room = server->find_or_create_room(room_name);
  room->add_member(user);

  conn->send(Message(TAG_OK, "succesfully joined room"));
 
  while (true) {
    // try to dequeue a Message from the user's MessageQueue
    Message *newMsg = user->mqueue.dequeue();

    /** TODO: figure out how to check sucess of dequeue() */
    // if a Message was successfully dequeued, send a "delivery"
    // message to the receiver. If the send is unsuccessful,
    // break out of the loop (because it's likely that the receiver
    // has exited and the connection is no longer valid)

    Message derefMsg = *newMsg;

    /** TODO: IFF sent request was received, then perform a delivery message */
    if (newMsg) {
      if (conn->send(derefMsg)) {
        delete newMsg;
        break;
      }
      delete newMsg;
    }
    else {
        conn->send(Message(TAG_ERR, "Empty error"));
        break;
    }
  }

  // make sure to remove the User from the room
  room->remove_member(user);

}

// if it is a valid room name
bool Server::isValidRoomName(std::string name) {
  if (name.size() < 0) return false; 

  for (size_t i = 0; i < name.size(); i++) {
    if (!isalnum(name[i])) return false;
  }

  return true;
}

