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
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];

  // TODO: connect to server
  Connection conn;

  // use sever socket to connect to server
  conn.connect(argv[1], argv[2]);

  // USE CONNECTION FUNCTIONALITY HERE 

  // TODO: send slogin message (assuming this goes to server)
  Message slogin(TAG_SLOGIN, "/slogin " + username); 
  rio_writen(fd, slogin, strlen(slogin)); // writes /slogin [username] to server
  rio_writen(fd, "\n", 1); // just a newline

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  while (1) {
    




  }
  return 0;
}
