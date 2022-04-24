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

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port); // use sever socket to connect to server
  if (!conn.is_open()) {
    std::cerr << "Couldn't connect to server\n";
    return 1;
  }
  
  // TODO: send slogin message
  Message slogin(TAG_SLOGIN, username); 

  if (!conn.send(slogin)) {
    std::cerr << slogin.getData() << std::endl;
    conn.close();
    return 1;
  }

  conn.receive(slogin);
  if (slogin.getTag() != "ok") { /** TODO: Error checking */
      std::cerr << slogin.getData() << std::endl;
      conn.close();
      return 1;
  }


  // TODO: loop reading commands from user, sending messages to server as appropriate
  while (1) {
    std::cout << "> ";  // Format:> [insert user input] \n
    std::string line; 

    std::getline(std::cin, line); // gets user input
    line = trim(line); // get rid of whitespace
    
    if (line.length() != 0) { // non-empty lines
      if (line[0] == '/') { // for commands
        std::string command = line.substr(1, line.length());

        if (line.substr(1, 5) == "join") { // join
          std::string roomName = line.substr(5, line.length() - 1);
          if (roomName == "") { // Missing room name
            std::cerr << "Needs a room name: ./join [room name]\n";
          } else { // send message
            Message msg (TAG_JOIN, roomName);
            if (!conn.send(msg)) { std::cerr << "Send Failed"<< std::endl; }
            conn.receive(msg);
            if (msg.getTag() != "ok") { 
              std::cerr << msg.getData() << std::endl;
            }
          }
        } 

        else if (line.substr(1, 6) == "leave") { // leave
            Message msg (TAG_LEAVE, "left room");
            if (!conn.send(msg)) std::cerr << "Send Failed"<< std::endl;
            conn.receive(msg);
            if (msg.getTag() != "ok") { 
              std::cerr << msg.getData() << std::endl;
            }
        } 

        else if (line.substr(1, 5) == "quit") { // quit: send message to signify quit
           Message msg (TAG_QUIT, "quit room");
            if (!conn.send(msg)) std::cerr << "Send Failed"<< std::endl;
            conn.receive(msg);
            if (msg.getTag() != "ok") {
              std::cerr << msg.getData() << std::endl;
            }
            break;
        } 
        else { // invalid commands
          std::cerr << "-Invalid command-\n" << "Valid Commands: /join /leave /quit\n"; 
        }
      } 

      else { // regular messages
        Message msg (TAG_SENDALL, line);
        if (!conn.send(msg)) std::cerr << "Send Failed"<< std::endl;
        conn.receive(msg);
        if (msg.getTag() != "ok") { /** TODO: Error checking */
              std::cerr << msg.getData() << std::endl;
        }
      }
    }
  } //end of while

  conn.close();
  return 0;
}
