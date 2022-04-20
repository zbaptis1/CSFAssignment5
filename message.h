#ifndef MESSAGE_H
#define MESSAGE_H

#include <algorithm>
#include <vector>
#include <string>

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
  std::vector<std::string> tagTable = {TAG_ERR, TAG_OK, TAG_SLOGIN,
                                      TAG_RLOGIN, TAG_JOIN, TAG_LEAVE, TAG_SENDALL, 
                                      TAG_SENDUSER, TAG_QUIT, TAG_DELIVERY, TAG_EMPTY};

  Message() { }

  Message(const std::string &tag, const std::string &data): tag(tag), data(data) { }

  // returns the payload (data) as a vector of strings,
  // split using ':' as the separator
  std::vector<std::string> split_payload() const {
    std::vector<std::string> result;
    
   
   /* Check to see if valid message */
    // An encoded message must not be more than MAX_LENGTH bytes.
    if (tag.size() > 255) {
      result[0] = tagTable[0];
      result[1] = "ERROR The given message was longer than 255 bytes"; 
      return result;
    }
    
    // A message must be a single line of text with no newline characters contained within it.
    int newLineCount = std::count(tag.begin(), tag.end(), "\n");
    if (newLineCount < 1 || !hasColon(data)) { 
      result[0] = tagTable[0];
      result[1] = "ERROR no newline character or no color separartor"; 
      return result;
    }
    
    // The tag must be one of the operations specified in the “tag table”.
    if (std::find(tagTable.begin(), tagTable.end(), tag) == tagTable.end()) { 
      result[0] = tagTable[0];
      result[1] = "ERROR The given tag was not apart of the tag table"; 
      return result;
    } // tag is not in tag table

    // The payload is an arbitrary sequence of characters. If a tag has a structured payload, the payload must be formatted exactly as specified.
    if (tag == TAG_DELIVERY) { // room:sender:message_text;
      if (std::count(tag.begin(), tag.end(), ':') != 2) {
        result[0] = tagTable[0];
        result[1] = "ERROR The given message payload was not structured correctly"; 
        return result;
      }
    }

    std::string copy = data; //Copy data into copy
    std::string splitData = "";
    std::string colon = ":"; //Use delimiter for copy.find()

    size_t pos = 0;
    unsigned index = 0;

    // result[0] = tag
    // result[1 -> end] = payload

    while ((pos = copy.find(colon)) != std::string::npos) { // find colon
      splitData = copy.substr(0, pos); // first iteration gets the tag
      result[index] = splitData;
      index++;
      copy.erase(0, pos + 1);
    }  

    // no more colons, we already got the tag in result[0]
    // puts the payload into result, and not the "\n"
    if ((pos = copy.find("\r\n")) != std::string::npos) {
      result[index] = copy.substr(0, copy.size() - 4);
    } else if ((pos = copy.find("\n")) != std::string::npos) {
      result[index] = copy.substr(0, copy.size() - 2);
    }
   
    return result; 
  }
};

#endif // MESSAGE_H
