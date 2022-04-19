#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection(): m_fd(-1), m_last_result(SUCCESS) {
}

Connection::Connection(int fd): m_fd(fd), m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_t in; 
  rio_readinitb();
}

void Connection::connect(const std::string &hostname, int port) { // used sockets.zip
  // TODO: call open_clientfd to connect to the server
  m_fd = open_clientfd(hostname, port);
  if (!is_open()) {
    std::cerr << "Couldn't connect to server";
    return();
  }

  // TODO: call rio_readinitb to initialize the rio_t object
  rio_t rio;
  rio_readinitb(&rio, fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  if (is_open()) { close(m_fd); }

}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return (m_fd >= 0);

}

void Connection::close() { // idk
  // TODO: close the connection if it is open
  if (is_open()) {
    close(m_fd);
  }

}

bool Connection::send(const Message &msg) {
  // TODO: send a message

  rio_writen(fd, msg, strlen(msg)); // writes msg to serve
  rio_writen(fd, "\n", 1); // just a newline


  /** TODO: HOW TO CHECK THIS */
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  /** TODO: need clarification on that ^^ */


  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
