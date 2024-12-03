#include "httpparser.h"
#include "socket.h"

#define SOCKET_CONNECTED 1

enum httpparser_state {
    PARSER_READY,
    PARSER_URI,
    PARSER_VERSION,
    PARSER_HEADERS_START,
    PARSER_HEADER_KEY,
    PARSER_HEADER_VALUE,
    PARSER_HEADERS_END,
    PARSER_BODY,
    PARSER_DONE,
    PARSER_ERROR
} httpparser_state;

enum httpparser_state httpparser_state = PARSER_READY;


unsigned char *method;
unsigned char *uri;
unsigned char *version;
char body[24];
unsigned int body_index = 0;

void httpparser_init() {
    httpparser_state = PARSER_READY;
    body_index = 0;
}

void httpparser_update() {
    /* if the socket is not connected, return */
    if (!socket_is_active(SOCKET_CONNECTED)) {
        return;
    }

    switch (httpparser_state) {
        case PARSER_READY:
            if (socket_recv_compare(SOCKET_CONNECTED, "GET ")) {
                method = "GET";
                httpparser_state = PARSER_URI;
            }
            else if (socket_recv_compare(SOCKET_CONNECTED, "PUT ")) {
                method = "PUT";
                httpparser_state = PARSER_URI;
            }
            else if (socket_recv_compare(SOCKET_CONNECTED, "DELETE ")) {
                method = "DELETE";
                httpparser_state = PARSER_URI;
            }
            else {
                /* invalid method mark to clear*/
                httpparser_state = PARSER_ERROR;
            }
            break;
        case PARSER_URI:
            if (socket_recv_compare(SOCKET_CONNECTED, "\\device\\log") && cmp_str(method, "DELETE")) {
                uri = "\\device\\log";
                httpparser_state = PARSER_VERSION;
            }
            else if (socket_recv_compare(SOCKET_CONNECTED, "\\device\\config\\") && cmp_str(method, "PUT")) {
                uri = "\\device\\config\\";
                httpparser_state = PARSER_VERSION;
            }
            else if (socket_recv_compare(SOCKET_CONNECTED, "\\device") && (cmp_str(method, "GET") || cmp_str(method, "PUT"))) {
                uri = "\\device";
                httpparser_state = PARSER_VERSION;
            }
            else {
                /* invalid uri mark to clear*/
                httpparser_state = PARSER_ERROR;
            }
            break;
        case PARSER_VERSION:
            break;

        case PARSER_ERROR:
            /* clean up and reinitialize */
            socket_flush_lines(SOCKET_CONNECTED);
            httpparser_init();
            break;
        default:
            httpparser_state = PARSER_READY;
            break;
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
