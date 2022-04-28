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
  struct ConnInfo *info = static_cast<ConnInfo *>(arg);

  std::unique_ptr<ConnInfo> info(info_);
  Message msg;

  /** TODO: need to do error checking w/ this BOOL function */
  if (!info->conn->receive(msg)) { //1st receive is not a login
    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "invalid message"));
    }
    return nullptr;
  }

  std::string userLogin = msg.data;
  if (!info->conn->send(Message(TAG_OK, "Welcome " + userLogin))) return nullptr;

  if (msg.tag == TAG_RLOGIN) chat_with_receiver(userLogin, info->conn, info->server); //If user is receiver
  else if (msg.tag == TAG_SLOGIN) chat_with_sender(userLogin, info->conn, info->server);

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)

  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port): m_port(port), m_ssock(-1) {
  // TODO: initialize mutex
  ptthread_mutex_init(&m_lock, m_ssock); /** TODO: figure out mutex in threads */
} 

Server::~Server() {
  // TODO: destroy mutex
  m_lock.destroy(); /** TODO: look at lecture notes for this */
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not

  //FROM LECTURE 27: APP PROTOCOLS {SLIDE 23}

  int server_fd = open_listenfd(m_port);
  if (server_fd < 0) { 
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

  int keep_going = 1;
  while (keep_going) {

    // MAYBE LAST 2 ARGS AREN'T NECESSARY: Accept(int s, struct sockaddr *addr, socklen_t *addrlen) 
    int client_fd = Accept(m_ssock, NULL, NULL);
    if (client_fd > 0) {
      /** TODO: make a connection object ???    */

      ConnInfo *info = new ConnInfo(new Connection(clientfd), this);

      pthread_t thr_id;
      if (pthread_create(&thr_id, nullptr, worker, static_cast<void *>(info)) != 0) {
        std::cerr << "Could not create thread\n";
        return;
      }

      // close(client_fd); /** TODO: see if necessary b/c worker might already handle connection closure w/ deconstructor

    } else std::cerr << "ERROR opening a connection" << std::endl;
  } 

  return;
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary

  return nullptr;
}


int chat_with_client(int client_fd) {

  Connection conn; /** TODO: should we pass conn. obj from server as param or create new one here */
  Message msg;

  // Read line from client
  conn.receive(msg); 

  if (rc < 0) return 1; // error reading data from client

  if (msg.tag == TAG_ERR) return 0;
  else {
    FILE *in = fmemopen(buf, (size_t) rc, "r");
    while (fscanf(in, "%d", &val) == 1) { sum += val; }
    fclose(in);
    snprintf(buf, sizeof(buf), "Sum is %d\n", sum);
    rio_writen(client_fd, buf, strlen(buf));
    return 1;
  }
  
}


void chat_with_sender(std::string username, Connection *conn, Server *server) {) {
  /** TODO: figure out implementation */



  // Just loop reading messages and sending an ok response for each one
  // while (true) {
  //   if (!info->conn->receive(msg)) {
  //     if (info->conn->get_last_result() == Connection::INVALID_MSG) {
  //       info->conn->send(Message(TAG_ERR, "invalid message"));
  //     }
  //     break;
  //   }

  //   if (!info->conn->send(Message(TAG_OK, "this is just a dummy response"))) {
  //     break;
  //   }
  // }


}

void chat_with_receiver(std::string username, Connection *conn, Server *server) {) {
    /** TODO: figure out implementation */

  User *user = new User(username);
  Room *room = server->find_or_create_room(room_name);

  room->add_member(user);

  Message* msg = nullptr;

  while (true) {
    // try to dequeue a Message from the user's MessageQueue

    //User * user = room->members.find(user);
    //user->deque();

    msg = user->dequeue();

    /** TODO: figure out how to check sucess of dequeue() */

    

    // if a Message was successfully dequeued, send a "delivery"
    // message to the receiver. If the send is unsuccessful,
    // break out of the loop (because it's likely that the receiver
    // has exited and the connection is no longer valid)


  }

  // make sure to remove the User from the room
  room->remove_member(user);

}
