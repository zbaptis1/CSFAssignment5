#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]); /** TODO: use try-catch for error check maybe */
  std::string username = argv[3];

  /** TODO: USE CONNECTION FUNCTIONALITY HERE */

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port); // use sever socket to connect to server
  if (!conn.is_open()) {
    std::cerr << "Couldn't connect to server\n";
    return 1;
  }
  
  // TODO: send slogin message
  Message slogin(TAG_SLOGIN, username); 
  if (!conn.send(slogin)) { //Error sending login
  /** TODO: is this correct */
      std::cerr << conn.m_last_result() << endl;
      conn.close();
      return 1;
  }

  if (!conn.recieve(slogin)) { // Error recieving login
  /** TODO: is this correct */
      std::cerr << conn.m_last_result() << endl;
      conn.close();
      return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while (1) {
    /** TODO: how to read commands
        - do we use scanf to read user input
        - message <- scanf
        - use conn.send(message) for server

        - end command to end while loop
    */

    // Format:> [insert user input] \n
    std::cout << "> ";
    std::string line; // user input

    std::getLine(std::cin, line);

    // why trim??
    
    if (line[0] == '/') {
      std::string command = line.substr(1, line.length());
      // join
      if (line.substr(1, 5) == "join") {
        std::string roomName;
        if (line.substr(5, line.length()) == "") {
          std::cerr << "Needs a room name: ./join [room name]\n";
        }
      } else if (line.substr(1, 6) == "leave") { // leave
        // send message to signify leave
      } else if (line.substr(1, 5) == "quit") { // quit
        // send message to signify quit
        break;
      } else {
        std::cerr << "-Invalid command-\n" << "Valid Commands: /join /leave /quit\n"; 
      }
    }

    // what is send_request?
  }
  conn.close();
  return 0;
}
