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
  rio_t rio;
  rio_readinitb(&rio, m_fd);
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
  rio_readinitb(&rio, m_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  this->close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return (m_fd >= 0);

}

void Connection::close() { // idk
  // TODO: close the connection if it is open
  if (is_open()) {
    /** TODO: how to close connection */
  }

}

bool Connection::send(const Message &msg) {
  // TODO: send a message




  rio_writen(m_fd, msg, strlen(msg)); // writes msg to serve
  rio_writen(m_fd, "\n", 1); // just a newline
  

  /** TODO: HOW TO CHECK THIS */
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately



}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  /** TODO: need clarification on that ^^ */


  rio_t rio;
  rio_readinitb(&rio, m_fd);

  // read response from server
  char buf[1000];
  ssize_t n = rio_readlineb(&rio, buf, sizeof(buf));

  string bufStr(buf);
  msg("", bufStr);

  vector<string> vec = msg.split_payload();  

  /** TODO: figure out what tag can be from response received from server 
            (My guess is either "ok" or "sendall") */

  if (msg.tag == "ok" || "sendall") {
    this->send(msg);
  }

  if (msg.tag == "delivery") {
    /** TODO: use room and sender from data in msg object and utilize in sending */

  }
  
  /** TODO: see if any "error" type tags that warrant a else statement */


  m_last_result = SET APPRORAITE;

  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
