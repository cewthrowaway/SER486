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
            /* TODO: Send the error response 200 */
            httpparser_state = PARSER_WAITING;
        break;
        case PARSER_RESPOND:
            /* todo send the success response 200 for all */
            if (method == METHOD_GET) {
                /* todo: send the gets */
                break;
            }
            if (method == METHOD_PUT) {
                if (header_key == HEADER_KEY_RESET && header_value) {
                    wdt_force_restart();
                    httpparser_state = PARSER_WAITING;
                    break;
                }
                else if (header_key == HEADER_KEY_TCRITHI) {
                    if (header_value > config.hi_warn) { 
                        config.hi_crit = header_value;
                        httpparser_state = PARSER_WAITING;
                        break;
                    }
                    else {
                        httpparser_state = PARSER_ERROR;
                        break;
                    }
                }
                else if (header_key == HEADER_KEY_TCRITLO) {
                    if (header_value < config.lo_warn) { 
                        config.lo_crit = header_value;
                        httpparser_state = PARSER_WAITING;
                        break;
                    }
                    else {
                        httpparser_state = PARSER_ERROR;
                        break;
                    }
                }
                else if (header_key == HEADER_KEY_TWARNHI) {
                    if (header_value > config.lo_warn && header_value < config.hi_crit) { 
                        config.hi_warn = header_value;
                        httpparser_state = PARSER_WAITING;
                        break;
                    }
                    else {
                        httpparser_state = PARSER_ERROR;
                        break;
                    }
                }
                else if (header_key == HEADER_KEY_TWARNLO) {
                    if (header_value < config.hi_warn && header > config.lo_crit) { 
                        config.hi_crit = header_value;
                        httpparser_state = PARSER_WAITING;
                        break;
                    }
                }
                else {
                    /* how did i get here */
                    httpparser_state = PARSER_ERROR;
                    break;
                }
                
            }
            else if (method == METHOD_DELETE && uri == URI_LOG) {
                log_clear();
                /* send the success */
                httpparser_state = PARSER_WAITING;
            }
            else {
                /* how did i get here??? */
                httpparser_state = PARSER_ERROR;
            }
        break;
        default:
            httpparser_state = PARSER_ERROR;
    }
    
}



void send_error_response() {
    socket_connect(server_socket, addr, SERVER_PORT);
    socket_writestr(server_socket,"HTTP/1.1 400 Bad Request\r\n");
    socket_writestr(server_socket,"Connection: close\r\n");
    socket_writestr(server_socket,"\r\n");
    socket_disconnect(server_socket);
}
void send_success_response() {
/* todo: define addr */
    socket_connect(server_socket, addr, SERVER_PORT);
    socket_writestr(server_socket,"HTTP/1.1 200 OK\r\n");
    socket_writestr(server_socket,"Connection: close\r\n");
    socket_writestr(server_socket,"\r\n");
    socket_disconnect(server_socket);
}
void send_device_response() {
    socket_connect(server_socket, addr, SERVER_PORT);
    socket_writestr(server_socket,"HTTP/1.1 200 OK\r\n");
    socket_writestr(server_socket,"Content-Type: application/vnd.api + json\r\n");
    socket_writestr(server_socket,"Connection: close\r\n");
    socket_writestr(server_socket,"\r\n");
    /* ******* start Body ******** */
    socket_writestr(server_socket,"{\r\n");
    socket_writequotedstring(server_socket,"vpd");
    socket_writestr(server_socket,":{");
    socket_writequotedstring(server_socket,"model");
    socket_writestr(server_socket,":");
    socket_writequotedstring(server_socket,config.model);

    
    socket_disconnect(server_socket);
}
