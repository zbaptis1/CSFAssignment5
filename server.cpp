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

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

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
/*
Server::Server(int port): m_port(port), m_ssock(-1) {
  // TODO: initialize mutex


  ptthread_mutex_init(&m_lock, m_ssock); // OUR OWN TODO:: figure out mutex in threads
} */

Server::~Server() {
  // TODO: destroy mutex
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not

  //FROM LECTURE 27: APP PROTOCOLS {SLIDE 23}
 /* int server_fd = open_listenfd(m_port);
  if (server_fd < 0) { return false;} */
  
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client


  //FROM LECTURE 27: APP PROTOCOLS {SLIDE 25}

/*  int keep_going = 1;
  while (keep_going) {


    // MAYBE LAST 2 ARGS AREN'T NECESSARY: Accept(int s, struct sockaddr *addr, socklen_t *addrlen) 
    int client_fd = Accept(server_fd, NULL, NULL);
    if (client_fd > 0) {
      keep_going = chat_with_client(client_fd);
      close(client_fd);
    }
  } */

  return;
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary

  return nullptr;
}
