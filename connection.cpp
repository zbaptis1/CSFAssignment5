#include <sstream>
#include <iostream>
#include <cctype>
#include <cassert>
#include <string>
#include "csapp.h"
#include "client_util.h"
#include "message.h"
#include "connection.h"

Connection::Connection(): m_fd(-1), m_last_result(SUCCESS) {}

Connection::Connection(int fd): m_fd(fd), m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) { // used sockets.zip
  // TODO: call open_clientfd to connect to the server
  std::string portStr = "" + port;

  /** TODO: need to convert hostname and port to char* */
  m_fd = open_clientfd(hostname.c_str(), portStr.c_str());
  if (!is_open()) {
    std::cerr << "Couldn't connect to server";
    return;
  }
  
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd); /** TODO: see if this is unneces. duplicate of constructor*/
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  this->close(); /** TODO: should this be close(m_fd) ??? */
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
  if (!is_open()) { 
    std::cerr << "Connection was not open" << std::endl;
    return false; 
  }

  std::vector<std::string> splitPayload = msg.split_payload(); /** TODO: might be split already */
  
  if (splitPayload[0].find("ERROR") != std::string::npos) { // there was an error
      m_last_result = INVALID_MSG; /** TODO: Might be EOFORERROR instead */
      return false; 
  }
  
  std::string tagAndPayStr = msg.tag + ":" + msg.data + "\n"; //Convert string to const char* for rio_written 2nd param
  const char * tagAndPayCharPtr = tagAndPayStr.c_str(); /** TODO: see if this will have null-terminator 
                                                                  (i.e '\0', important for strlen() function below */
  ssize_t n = rio_writen(m_fd, tagAndPayCharPtr, strlen(tagAndPayCharPtr)); // writes msg to server
  /** rio_writen(m_fd, "\n", 1); TODO: MIGHT/MIGHT NOT BE NEEDED */
  
  if (n < 1 || strlen(tagAndPayCharPtr) != n) { /** TODO: don't know if this needed, but essentially checks if any bytes are even being sent! */
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  if (!is_open()) { 
    std::cerr << "Connection was not open" << std::endl;
    return false; 
  }
  // read response from server
  std::string tag;
  std::string data;
  char buf[1000];
  ssize_t n = rio_readlineb(&m_fdbuf, buf, sizeof(buf));

  if (n < 1) {
    msg.tag = TAG_ERR; // message wasn't recieved 
    m_last_result = EOF_OR_ERROR;
    return false;
  }


  const trim(bufStr);
  std::string bufStr(buf);

  /* 2 Invalid Case to handle: 
      1. if hold you don't have colon 
      2. (MAYBE) The tag must only require lower case characters
  */
  if (!hasColon(bufStr)) {
    msg.tag = TAG_ERR;
    m_last_result = INVALID_MSG;
    return false;
  }

  size_t colonPos = bufStr.find(":");
  tag = bufStr.substr(0, colonPos);

  if(!isLowerCased(tag)) {
    msg.tag = TAG_ERR;
    m_last_result = INVALID_MSG;
    return false;
  }
  
  data = bufStr.substr(colonPos + 1, bufStr.size()); /** TODO: strlen() or STRING.size ??? (check if neither count null terminator) */

  msg.tag = tag;
  msg.data = data;
  //room:sender:text


  /** TODO: figure out what tag can be from response received from server 
            (My guess is either "ok" or "sendall") */


  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  return true;
}

// prints to stderr and closes the connection
  // automatically returns 1
int Connection::invalidSendOrRecieve() {
  std::cerr << m_last_result << std::endl;
  this->close();
  return 1;
}