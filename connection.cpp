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

  rio_readinitb()
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server

  riot_t obj = open_clientfd(hostname, port); //OUR OWN TODO: figure out args and return value(if any)

  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(); //OUR OWN TODO: figure out args and return value

}

Connection::~Connection() {
  // TODO: close the socket if it is open

  
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
}

void Connection::close() {
  // TODO: close the connection if it is open
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
