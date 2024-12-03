#include "httpparser.h"
#include "socket.h"

#define SOCKET_CONNECTED 1
#define SERVER_PORT 8080

#define METHOD_GET 0
#define METHOD_PUT 1
#define METHOD_DELETE 2

#define URI_LOG 0
#define URI_CONFIG 1
#define URI_DEVICE 2

#define HEADER_KEY_TCRITHI 0
#define HEADER_KEY_TCRITLO 1
#define HEADER_KEY_TWARNHI 2
#define HEADER_KEY_TWARNLO 3
#define HEADER_KEY_RESET 4

enum httpparser_state {
    PARSER_RECONNECTING,
    PARSER_WAITING,
    PARSER_METHOD,
    PARSER_URI,
    PARSER_VARIABLE_KEY,
    PARSER_VARIABLE_VALUE,
    PARSER_ERROR,
    PARSER_RESPOND
} httpparser_state;

enum httpparser_state httpparser_state = PARSER_WAITING;


unsigned char method;
unsigned char uri
unsigned char header_key;
int header_value;

unsigned int body_index = 0;
usigned char socket;

void httpparser_init(unsigned char socket) {
    httpparser_state = PARSER_READY;
    server_socket = socket;
}

void httpparser_update() {
    switch(httpparser_state) {
        case PARSER_RECONNECTING:
            socket_open(server_socket);
            socket_listen(server_socket,SERVER_PORT);
            httpparser_state = PARSER_WAITING;
        break;
        case PARSER_WAITING:
            if (!socket_is_listening(server_socket))
            {
                httpparser_state = PARSER_RECONNECTING;
                break;
            }
            else if (socket_received_line(server_socket)) 
            {
                httpparser_state = PARSER_METHOD;
                break;
            }
        break;
        case PARSER_METHOD:
            if (socket_recv_compare(server_socket,"GET")) {
                method = METHOD_GET;
                httpparser_state = PARSER_URI;
                break;
            }
            else if (socket_recv_compare(server_socket,"PUT")) {
                method = METHOD_PUT;
                httpparser_state = PARSER_URI;
                break;
            }
            else if (socket_recv_compare(server_socket,"DELETE")) {
                method = METHOD_DELETE;
                httpparser_state = PARSER_URI;
                break;
            }
            else {
                httpparser_state = PARSER_ERROR;
            }
        break;
        case PARSER_URI:
            if (socket_recv_compare(server_socket, "\\device") {
                /* /device/log */
                if (socket_recv_compare(server_socket,"\\log") {
                    if (method == METHOD_DELETE) {
                        uri = URI_LOG;
                        httpparser_state = PARSER_RESPOND;
                        break;
                    }
                        
                    /* invalid method for route*/
                    else {
                        httpparser_state = PARSER_ERROR;
                        break;
                    }
                } 
                
                /* /device/config? */
                else if (socket_recv_compare(server_socket,"\\config?") {
                    if (method == METHOD_PUT) {
                        uri = URI_CONFIG;
                        httpparser_state = PARSER_VARIABLE_KEY;
                        break;
                    }
                        
                    /* invalid method for route */
                    else {
                        httpparser_state = PARSER_ERROR;
                        break;
                    }
                }
                /* /device */
                else if (socket_recv_compare(server_socket," ") {
                    if (method == METHOD_GET) {
                        uri = URI_DEVICE;
                        httpparser_state = PARSER_RESPOND;
                        break;
                    }
                    /* invalid method for route */
                    else {
                        httpparser_state = PARSER_ERROR;
                        break;
                    }
                }
                    
                /* /device? */
                else if (socket_recv_compare(server_socket, "?") {
                    if (method == METHOD_PUT) {
                        uri = URI_DEVICE;
                        httpparser_state = PARSER_VARIABLE_KEY;
                        break;
                    }
                }
                /* invalid method for route */
                else {
                    httpparser_state = PARSER_ERROR;
                    break;
                }
            }
            /* invalid route */
            else
            {
                httpparser_state = PARSER_ERROR;
                break;
            }

        break;
        case PARSER_VARIABLE_KEY:
            if (uri == URI_CONFIG) {
                if (socket_recv_compare(server_socket,"tcrit_hi=")) {
                    header_key = HEADER_KEY_TCRITHI;
                    httpparser_state = PARSER_VARIABLE_VALUE;
                    break;
                }
                else if (socket_recv_compare(server_socket,"twarn_hi=")) {
                    header_key = HEADER_KEY_TWARNHI;
                    httpparser_state = PARSER_VARIABLE_VALUE;
                    break;
                }
                else if (socket_recv_compare(server_socket,"tcrit_lo=")) {
                    header_key = HEADER_KEY_TCRITLO;
                    httpparser_state = PARSER_VARIABLE_VALUE;
                    break;
                }
                else if (socket_recv_compare(server_socket,"twarn_lo=")) {
                    header_key = HEADER_KEY_TWARNLO;
                    httpparser_state = PARSER_VARIABLE_VALUE;
                    break;
                }
                else {
                    /* invalid key */
                    httpparser_state = PARSER_ERROR;
                    break;
                }
            }
            else if (uri == URI_DEVICE) {
                if (socket_recv_compare(server_socket,"reset=")) {
                    header_key = HEADER_KEY_RESET;
                    httpparser_state = PARSER_VARIABLE_VALUE;
                    break;
                }
                else {
                    /* invalid key */
                    httpparser_state = PARSER_ERROR;
                    break;
                }
            }
            else {
                /* we should never have entered this */
                /* we have a bad state transition */
                httpparser_state = PARSER_ERROR;
                break;
            }

        break;
        case PARSER_VARIABLE_VALUE:
            if (header_key == HEADER_KEY_RESET) {
                if (socket_recv_compare(server_socket,"false")) {
                    header_value = 0;
                    httpparser_state = PARSER_RESPOND;
                    break;
                }
                else if (socket_recv_compare(server_socket,"true")) {
                    header_value = 1;
                    httpparserver_state = PARSER_RESPOND;
                    break;
                }
                else {
                    /* invalid value */
                    httpparser_state = PARSER_ERROR;
                    break;
                }
            }
            else if (header_key < HEADER_KEY_RESET) {
                if (socket_recv_int(server_socket, &header_value)) {
                     httpparser_state = PARSER_RESPOND;
                    break;
                }
                else 
                {
                    /* invalid value */
                    httpparser_state = PARSER_ERROR;
                    break;
                }
            }
            else {
                /* invalid header key */
                /* should never have been in here */
                httpparser_state = PARSER_ERROR;
                break;
        break;
        case PARSER_ERROR:
            /* delete the line and try again */
            socket_flush_line(server_socket);
            httpparser_state = PARSER_WAITING;
        break;
        case PARSER_RESPOND:
            if (method == METHOD_GET) {
                
            }
            if (method == METHOD_PUT) {
                
            }
            if (method == METHOD_DELETE && uri == URI_LOG) {
                
            }
        break;
        default:
            httpparser_state = PARSER_ERROR;
    }
    
}

/************************************************
 * cmp_str
 * Description: Reads a specified amount of data from the EEPROM 
 *              starting at the specified address and stores it in 
 *              the provided buffer.
 * 
 * Arguments:
 *    - addr: The starting address in the EEPROM to read from.
 *    - buf: Pointer to the buffer where the read data will be stored.
 *    - size: The number of bytes to read from the EEPROM.
 * 
 * Returns: None.
 * Changes:
 *    - Populates the provided buffer with data read from the EEPROM.
 ************************************************/
int cmp_str(const char *str1, const char *str2)
{
    /* while strings are valid */
  while (*str1 && *str2)
  {
    /* if the values are ever not the same, then stop */
    if (*str1 != *str2)
    {
      return 0;
    }
    /* go to the next character */
    str1++;
    str2++;
  }
  return *str1 == '\0' && *str2 == '\0';
}
