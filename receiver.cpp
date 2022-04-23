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
    std::cerr << rlogin.data << std::endl;
    conn.close();
    return 1;
  } 

  if (!conn.receive(rlogin)) { // Error recieving login
      std::cerr << rlogin.data << std::endl;
      conn.close();
      return 1;
  }

  if (!conn.send(join)) { //Error sending join request
    std::cerr << join.data << std::endl;
    conn.close();
    return 1;
  }

  if (!conn.receive(join)) { // Error recieving join request
    std::cerr << join.data << std::endl;
    conn.close();
    return 1;
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  Message msg;

  int err = 0;
  while(1) {
    /** TODO: how to receive messages
        - use messageQueue?
        - change the tag of these messages to TAG_DELIVERY?
        - where do these messages output?



        - end command to end while loop
    */


    if (!conn.receive(msg)) {
      err = 1;
      break;
    } 
    else {

      if (msg.tag != TAG_DELIVERY) {
        err = 1;
        break;
      } 
      
      std::vector<std::string> loadData = msg.split_payload();

      if (loadData.size() != 3) { 
        err = 1;
        break;
      }

      std::string room = loadData[0];
      std::string sender = loadData[1];
      std::string text = loadData[2];
      std::cout << sender << ":" << text << std::endl;
    }
  }

  if (err == 1) {
    return conn.invalidSendOrreceive();
  }
  return 0;
}
