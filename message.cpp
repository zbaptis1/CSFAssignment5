#include <algorithm>
#include "message.h"
#include "client_util.h"


std::vector<std::string> Message::split_payload() const {
    std::vector<std::string> result(3);
    
   /* Check to see if valid message */
    // An encoded message must not be more than MAX_LENGTH bytes.
    if ((tag.size() + data.size()) > 255) { // 253 to account for colon and "\n"
      result[0] = "ERROR The given message was longer than 255 bytes"; 
      return result;
    } 
    
    // // A message must be a single line of text with no newline characters contained within it.
    // int newLineCount = std::count(tag.cbegin(), tag.cend(), '\n');
    // if (newLineCount < 1) { 
    //   result[0] = "ERROR no newline character"; 
    //   return result;
    // } 
    
    std::string tempData = trim(data);

    if(tempData.find('\n') != std::string::npos) {
      result[0] = "ERROR newline found withing data";
      return result;
    }

    if (data.find(':') == std::string::npos) {
        result[0] = "ERROR no colon separator"; 
        return result;
    }
    
    // The tag must be one of the operations specified in the “tag table”.
    if (tag != TAG_ERR && tag != TAG_OK && tag != TAG_SLOGIN && tag != TAG_RLOGIN
        && tag != TAG_JOIN && tag != TAG_LEAVE && tag != TAG_SENDALL && tag != TAG_SENDUSER
        && tag != TAG_QUIT && tag != TAG_DELIVERY && tag != TAG_EMPTY) {
      result[0] = "ERROR The given tag was not apart of the tag table"; 
      return result;
    }
    

    // The payload is an arbitrary sequence of characters. If a tag has a structured payload, the payload must be formatted exactly as specified.
    if (tag == TAG_DELIVERY) { // room:sender:message_text;
      if (std::count(data.cbegin(), data.cend(), ':') != 2) {
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

    result[index] = copy;
    
    return result; 
  }
