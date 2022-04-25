#include <sstream>
#include <iostream>
#include <cctype>
#include <cassert>
#include <string>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"


Connection::Connection(): m_fd(-1), m_last_result(SUCCESS) {}

Connection::Connection(int fd): m_fd(fd), m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) { // used sockets.zip
  // TODO: call open_clientfd to connect to the server
  std::string portStr = std::to_string(port);

  m_fd = open_clientfd(hostname.c_str(), portStr.c_str());
  if (!is_open()) {
    std::cerr << "Couldn't connect to server";
    return;
  }
  
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd); 
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  this->close(); 
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return (m_fd >= 0);
}

void Connection::close() {
  // TODO: close the connection if it is open
  if (is_open()) {
    ::close(m_fd); 
    m_fd = -1;
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message


  std::string tagAndPayStr = msg.tag + ":" + msg.data + "\n";
  
  ssize_t n = rio_writen(m_fd, tagAndPayStr.c_str(), strlen(tagAndPayStr.c_str()));

  if (n != (ssize_t) strlen(tagAndPayStr.c_str())) { 
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  
  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  // read response from server
  std::string tag;
  std::string data;
  char buf[msg.MAX_LEN + 1];
  ssize_t n = rio_readlineb(&m_fdbuf, buf, msg.MAX_LEN + 1); // issue here

  if (n < 1) {
    msg.tag = TAG_ERR; // message wasn't received 
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  const std::string newStr(buf);
  std::string bufStr = trim(newStr);

  size_t colonPos = bufStr.find(':');
  if(colonPos == std::string::npos) {
    msg.tag = TAG_ERR;
    m_last_result = INVALID_MSG;
    return false;
  }
  
  tag = bufStr.substr(0, colonPos);
  data = bufStr.substr(colonPos + 1);

  if(!isLowerCased(tag)) {
    msg.tag = TAG_ERR;
    m_last_result = INVALID_MSG;
    return false;
  }

  
  msg.tag = tag;
  msg.setData(data); 

  return true;
}

// prints to stderr and closes the connection
  // automatically returns 1
int Connection::invalidSendOrreceive() {
  std::cerr << m_last_result << std::endl;
  this->close();
  return 1;
}