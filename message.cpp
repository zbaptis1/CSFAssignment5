#include <algorithm>
#include <vector>
#include <string>

#include "message.h"

std::vector<std::string> Message::split_payload() const {
    std::vector<std::string> result;
    
   /* Check to see if valid message */
    // An encoded message must not be more than MAX_LENGTH bytes.
    if ((tag.size() + data.size()) > 253) { // 253 to account for colon and "\n"
      result[0] = "ERROR The given message was longer than 255 bytes"; 
      return result;
    }
    
    // A message must be a single line of text with no newline characters contained within it.
    int newLineCount = std::count(tag.begin(), tag.end(), "\n");
    if (newLineCount < 1 || (data.find(":") == std::string::npos)) { 
      result[0] = "ERROR no newline character or no color separartor"; 
      return result;
    }
    
    // The tag must be one of the operations specified in the “tag table”.
    std::vector<std::string> tagTable = {TAG_ERR, TAG_OK, TAG_SLOGIN, TAG_RLOGIN, TAG_JOIN, 
                                        TAG_LEAVE, TAG_SENDALL, TAG_SENDUSER, TAG_QUIT, 
                                        TAG_DELIVERY, TAG_EMPTY};
    if (std::find(tagTable.begin(), tagTable.end(), tag) == tagTable.end()) { 
      result[0] = "ERROR The given tag was not apart of the tag table"; 
      return result;
    } // tag is not in tag table

    // The payload is an arbitrary sequence of characters. If a tag has a structured payload, the payload must be formatted exactly as specified.
    if (tag == TAG_DELIVERY) { // room:sender:message_text;
      if (std::count(data.begin(), data.end(), ':') != 2) {
        result[0] = "ERROR The given message payload was not structured correctly"; 
        return result;
      }
    }

    std::string copy = data; //Copy data into copy
    std::string splitData = "";
    std::string colon = ":"; //Use delimiter for copy.find()

    size_t pos = 0;
    unsigned index = 0;

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
