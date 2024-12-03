#ifndef SERVER_H_INCLUDED
#def SERVER_H_INCLUDED

enum http_method {
  GET, 
  POST,
  DELETE
} http_method

struct http_header {
  char key[7],
  char value[4]
}
struct http_request {
  enum http_method method,
  struct http_header headers[2],
  char body[48]
};

struct http_response {
  unsigned int status,
  struct http_header headers[2],
  char body[48]
};
  
void server_init(int socket, char* address, int port);
void server_start();
void server_update();

void server_send_response(struct http_response);

#endif
