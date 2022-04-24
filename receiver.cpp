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
  int server_port = std::stoi(argv[2]); /** TODO: use try-catch for error check maybe */
  std::string username = argv[3];
  std::string room_name = argv[4];

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "Couldn't connect to server\n";
    return 1;
  }

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
    
  Message rlogin(TAG_RLOGIN, username);
  Message join(TAG_JOIN, room_name);

  if (!conn.send(rlogin)) { //Error sending login
    std::cerr << rlogin.getData() << std::endl;
    conn.close();
    return 1;
  } 
  
  conn.receive(rlogin);
  if (rlogin.tag != "ok") { /** TODO: Error checking */
      std::cerr << rlogin.getData() << std::endl;
      conn.close();
      return 1;
  }

  if (!conn.send(join)) { //Error sending join request
    std::cerr << join.getData() << std::endl;
    conn.close();
    return 2;
  }
  conn.receive(join);
  if (join.tag != "ok") { // Error recieving join request
    std::cerr << join.getData() << std::endl;
    conn.close();
    return 2;
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  Message msg;

  while(1) {

    conn.receive(msg);
    if (msg.getTag() != "ok") { 
      std::cerr << msg.getData() << std::endl;
    }

    if (msg.getTag() != TAG_DELIVERY) { 
      std::cerr << msg.getData() << std::endl;
    } 
    
    std::vector<std::string> loadData = msg.split_payload();

    if (loadData.size() == 3) { 
      std::string room = loadData[0];
      std::string sender = loadData[1];
      std::string text = loadData[2];
      std::cout << sender << ": " << text << std::endl;
    }
  }
  conn.close();
  return 0;
}
