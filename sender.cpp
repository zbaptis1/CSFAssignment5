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

  if (!conn.send(slogin)) return conn.invalidSendOrRecieve(); //Error sending login
  if (!conn.recieve(slogin)) return conn.invalidSendOrRecieve(); // Error recieving login
  
  // TODO: loop reading commands from user, sending messages to server as appropriate
  while (1) {
    std::cout << "> ";  // Format:> [insert user input] \n
    std::string line; 

    std::getLine(std::cin, line); // gets user input
    line = trim(line); // get rid of whitespace
    
    if (line[0] == '/') { // for commands 
      std::string command = line.substr(1, line.length());

      if (line.substr(1, 5) == "join") { // join
        std::string roomName = line.substr(5, line.length());
        if (roomName == "") { // Missing room name
          std::cerr << "Needs a room name: ./join [room name]\n";
        } else { // send message
          if (!conn.send(Message(TAG_JOIN, roomName))) std::cerr << "Send Failed"<< endl;
          /** TODO: should we close connection or leave it open ? */
        }
      } 

      else if (line.substr(1, 6) == "leave") { // leave
        if (!conn.send(Message(TAG_LEAVE, "left room"))) std::cerr << "Send Failed" << endl;  // Error sending message
         /** TODO: should we close connection or leave it open ? */
      } 

      else if (line.substr(1, 5) == "quit") { // quit: send message to signify quit
        if (!conn.send(Message(TAG_QUIT, "quit room"))) std::cerr << "Send Failed" << endl; /** TODO: should we close connection or leave it open ? */
        else { break; } // successfully quit
      } 
      else { // invalid commands
        std::cerr << "-Invalid command-\n" << "Valid Commands: /join /leave /quit\n"; 
      }
    } 
    
    else { // regular messages
      if (!conn.send(Message(TAG_SENDALL, line)) { //Error sending message
        // std::cerr << conn.m_last_result() << endl;
        // conn.close();
        return conn.invalidSendOrRecieve(); /** TODO: same logic for code, change name of this function ??? */
      }
    }
  } //end of while

  conn.close(); /** TODO: see if this close is necessary */
  return 0;
}
