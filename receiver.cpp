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

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
    
  Message rlogin(TAG_RLOGIN, username);
  Message join(TAG_JOIN, room_name);


  if (!conn.send(rlogin)) { //Error sending login
    std::cerr << rlogin.data << endl;
    conn.close();
    return 1;
  } 

  if (!conn.recieve(rlogin)) { // Error recieving login
      std::cerr << rlogin.data << endl;
      conn.close();
      return 1;
  }

  if (!conn.send(join)) { //Error sending join request
    std::cerr << join.data << endl;
    conn.close();
    return 1;
  }

  if (!conn.recieve(join)) { // Error recieving join request
    std::cerr << join.data << endl;
    conn.close();
    return 1;
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  Message msg;

  int err = 0;
  while(1) {
    /** TODO: how to recieve messages
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
      
      vector<string> loadData = msg.split_payload();

      if (loadData.size() != 3) { 
        err = 1;
        break;
      }

      string room = loadData[0];
      string sender = loadData[1];
      string text = loadData[2];
      std::cout << sender << ":" << text << endl;
    }
  }

  if (err == 1) {
    conn.invalidSendOrRecieve();
    return 1;
  }
  return 0;
}
