#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection(): m_fd(-1), m_last_result(SUCCESS) {}

Connection::Connection(int fd): m_fd(fd), m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) { // used sockets.zip
  // TODO: call open_clientfd to connect to the server
  m_fd = open_clientfd(hostname, port);
  if (!is_open()) {
    std::cerr << "Couldn't connect to server";
    return();
  }
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd); /** TODO: see if this is unneces. duplicate of constructor*/
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
    close(m_fd); /** TODO: find out the "special" case and how it changes this */
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message

  rio_writen(m_fd, msg, strlen(msg)); // writes msg to serve
  rio_writen(m_fd, "\n", 1); // just a newline
  

  /** TODO: 
    - HOW TO CHECK THIS (Zyan)
    - Redo this function or check to see what to add other
      than these 2 lines above
  */

  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg

  /** TODO: No need for init, use class field (REPLACE EVERY INSTANCE OF rio with m_fdbuf)*/
  // rio_t rio;
  // rio_readinitb(&rio, m_fd); 

  // read response from server
  char buf[1000];
  ssize_t n = rio_readlineb(&m_fdbuf, buf, sizeof(buf));

  if (n < 1) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  string bufStr(buf);
  bufstr = trim.bufstr;

  /* 2 Invalid Case to handle: 
      1. if hold you don't have colon 
      2. (MAYBE) The tag must only require lower case characters
  */
  if (!hasColon(bufstr) || !islowercased(bufstr)) {
    m_last_result = INVALID_MSG;
    return false;
  }

  msg("", bufStr);
  vector<string> vec = msg.split_payload();  

  /** TODO: figure out what tag can be from response received from server 
            (My guess is either "ok" or "sendall") */


  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  return true;
}
