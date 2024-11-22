#include "util.h"

void update_checksum(unsigned char* data, unsigned int size) {
    if (size == 0) return; // Handle empty data structure

    unsigned int sum = 0;

    // Calculate the sum of all bytes except the last one
    for (unsigned int i = 0; i < size - 1; i++) {
        sum += (unsigned char)data[i];
    }

    // Calculate the checksum value for the last byte
    data[size - 1] = (char)(-sum & 0xFF); // Ensure it wraps within one byte
}


int is_checksum_valid(unsigned char* data, unsigned int size) {
    if (size == 0) return 0; // Invalid for empty data structure

    unsigned int sum = 0;

    // Calculate the sum of all bytes
    for (unsigned int i = 0; i < size; i++) {
        sum += (unsigned char)data[i];
    }

    // Return 1 if the sum is zero, otherwise 0
    return (sum & 0xFF) == 0;
}
int cmp_str(const char *str1, const char *str2)
{
  while (*str1 && *str2)
  {
    if (*str1 != *str2)
    {
      return 0;
    }
    str1++;
    str2++;
  }
  return *str1 == '\0' && *str2 == '\0';
}