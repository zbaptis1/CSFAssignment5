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

  m_fd = -1;
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  std::vector<std::string> splitPayload = msg.split_payload(); /** TODO: might be split already */
  
  if (msg.tag.equals(TAG_ERR)) { //Error checking 
    if (splitPayload[0].find("ERROR") != std::string::npos) { /** TODO: see if there are distinct errors for result  REFLECT IN SPLIT PAYLOAD FOR INDICY 0*/
      m_last_result = INVALID_MSG; /** TODO: Might be EOFORERROR instead */
      return false; 
    }
  }
  
  string tagAndPayStr = msg.tag + ":" + msg.data + "\n"; //Convert string to const char* for rio_written 2nd param
  const char * tagAndPayCharPtr = tagAndPayStr.c_str(); /** TODO: see if this will have null-terminator 
                                                                  (i.e '\0', important for strlen() function below */
  ssize_t n = rio_writen(m_fd, tagAndPayCharPtr, strlen(tagAndPayStr)); // writes msg to server
  /** rio_writen(m_fd, "\n", 1); TODO: MIGHT/MIGHT NOT BE NEEDED */
  
  if (n < 1 || strlen(tagAndPayStr) != n) { /** TODO: don't know if this needed, but essentially checks if any bytes are even being sent! */
    m_last_result = EOF_OR_ERROR;
    return false;
  }

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
  bufstr = trim(bufstr);

  /* 2 Invalid Case to handle: 
      1. if hold you don't have colon 
      2. (MAYBE) The tag must only require lower case characters
  */
  if (!hasColon(bufstr)) {
    m_last_result = INVALID_MSG;
    return false;
  }

  size_t colonPos = bufstr.find(":")
  string tag = bufstr.substr(0, colonPos);

  if(!islowercased(tag) {
     m_last_result = INVALID_MSG;
    return false;
  }

  string data = bufstr.substr(colonPos + 1, strlen(bufstr));
  msg(tag, data);


  msg.data = data;
  //room:sender:text


  /** TODO: figure out what tag can be from response received from server 
            (My guess is either "ok" or "sendall") */


  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  return true;
}
