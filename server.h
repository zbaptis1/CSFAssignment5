#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <string>
#include <pthread.h>

//ADDITIONAL FILE IMPORTS
#include "connection.h"

class Room;
class Server;

struct ConnInfo {

  Connection * conn;
  Server* server;
  
  //FROM PARTIAL IMPLEMENTATION
  ConnInfo(Connection *conn, Server *server) : conn(conn), server(server) { }
  ~ConnInfo() { delete conn; } // destroy connection when ConnInfo object is destroyed
};


//ADD ANY ADDITIONAL STRUCTURE(S) WE NEED HERE

class Server {
public:
  Server(int port);
  ~Server();

  bool listen();

  void handle_client_requests();

  Room *find_or_create_room(const std::string &room_name);

  int chat_with_client(int client_fd);

  void chat_with_sender(User *user, Connection *conn, Server *server);

  void chat_with_receiver(User *user, Connection *conn, Server *server);


private:
  // prohibit value semantics
  Server(const Server &);
  Server &operator=(const Server &);

  typedef std::map<std::string, Room *> RoomMap;

  // These member variables are sufficient for implementing
  // the server operations
  int m_port;
  int m_ssock;
  RoomMap m_rooms;
  pthread_mutex_t m_lock;
};

#endif // SERVER_H
