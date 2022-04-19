#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
    
  Message rlogin(TAG_RLOGIN, "/rlogin " + username);
  Message join(TAG_JOIN, "/join " + room_name);

  conn.send(rlogin);
  conn.send(join);


  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  while(1) {
    /** TODO: how to recieve messages
      use messageQueue?
      change the tag of these messages to TAG_DELIVERY?

      where do these messages output?

      end command to end while loop
      */

  }



  return 0;
}
