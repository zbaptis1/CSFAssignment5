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

  // TODO: send slogin message
  Message slogin(TAG_SLOGIN, "/slogin " + username); 
  if (!conn.send(slogin)) { //Error sending login
    /** TODO: figure out how to handle send error AND if needed 
              Maybe it's printing to std::cerr??? */
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


  }
  return 0;
}
