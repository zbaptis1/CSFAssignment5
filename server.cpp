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
    int client_fd = Accept(server_fd, NULL, NULL);
    if (client_fd > 0) {
      /** TODO: make a connection object ???    */
      
      conn(client_fd);

      keep_going = chat_with_client(client_fd);
      close(client_fd);
    }
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

  if (strcmp(buf, "quit\n") == 0 || strcmp(buf, "quit\r\n") == 0) return 0;
  else {
    FILE *in = fmemopen(buf, (size_t) rc, "r");
    while (fscanf(in, "%d", &val) == 1) { sum += val; }
    fclose(in);
    snprintf(buf, sizeof(buf), "Sum is %d\n", sum);
    rio_writen(client_fd, buf, strlen(buf));
    return 1;
  }
  
}
