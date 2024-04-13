#pragma once

#ifndef __BASE32_H_
#define __BASE32_H_

#include <stddef.h>   // size_t

/**
 * Returns the length of the output buffer required to encode len bytes of
 * data into base32. This is a macro to allow users to define buffer size at
 * compilation time.
 */
#define BASE32_LEN(len)  (((len)/5)*8 + ((len) % 5 ? 8 : 0))

 /**
  * Returns the length of the output buffer required to decode a base32 string
  * of len characters. Please note that len must be a multiple of 8 as per
  * definition of a base32 string. This is a macro to allow users to define
  * buffer size at compilation time.
  */
#define UNBASE32_LEN(len)  (((len)/8)*5)

  /**
   * Encode the data pointed to by plain into base32 and store the
   * result at the address pointed to by coded. The "coded" argument
   * must point to a location that has enough available space
   * to store the whole coded string. The resulting string will only
   * contain characters from the [A-Z2-7=] set. The "len" arguments
   * define how many bytes will be read from the "plain" buffer.
   **/
int base32_encode(const unsigned char* plain, size_t len, unsigned char* coded);

/**
 * Decode the null terminated string pointed to by coded and write
 * the decoded data into the location pointed to by plain. The
 * "plain" argument must point to a location that has enough available
 * space to store the whole decoded string.
 * Returns the length of the decoded string. This may be less than
 * expected due to padding. If an invalid base32 character is found
 * in the coded string, decoding will stop at that point.
 **/
size_t base32_decode(const unsigned char* coded, unsigned char* plain);

#endif