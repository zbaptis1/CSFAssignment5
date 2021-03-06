#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>
#include "client_util.h"

// standard message tags (note that you don't need to worry about
// "senduser" or "empty" messages)
#define TAG_ERR       "err"       // protocol error
#define TAG_OK        "ok"        // success response
#define TAG_SLOGIN    "slogin"    // register as specific user for sending
#define TAG_RLOGIN    "rlogin"    // register as specific user for receiving
#define TAG_JOIN      "join"      // join a chat room
#define TAG_LEAVE     "leave"     // leave a chat room
#define TAG_SENDALL   "sendall"   // send message to all users in chat room
#define TAG_SENDUSER  "senduser"  // send message to specific user in chat room
#define TAG_QUIT      "quit"      // quit
#define TAG_DELIVERY  "delivery"  // message delivered by server to receiving client
#define TAG_EMPTY     "empty"     // sent by server to receiving client to indicate no msgs available

struct Message {
  // An encoded message may have at most this many characters,
  // including the trailing newline ('\n'). Note that this does
  // *not* include a NUL terminator (if one is needed to
  // temporarily store the encoded message.)
  static const unsigned MAX_LEN = 255;

  std::string tag;
  std::string data;
  unsigned size = 0;

  Message() { }

  Message(const std::string &tag, const std::string &data):  tag(tag), data(data) { }

  std::vector<std::string> split_payload() const;

  std::string getData() { return data; }
  std::string getTag() { return tag; } 
  size_t getSize() { return size; }
  void setData(std::string newData ) { data = newData; }
    
};

#endif // MESSAGE_H
