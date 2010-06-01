#ifndef _BADGER_ENCODERS_H_
#define _BADGER_ENCODERS_H_

#include <badger/config.h>

#include <sys/types.h>

typedef void (*badger_encoder_func)(unsigned char *dest,const unsigned char *src,size_t length,void *data);
typedef void (*badger_decoder_func)(unsigned char *dest,const unsigned char *src,size_t length,void *data);

#ifdef ENCODERS_XOR

struct badger_xor
{
	size_t length;
	const unsigned char pad[];
};
typedef struct badger_xor badger_xor_t;

void badger_xor_encoder(unsigned char *dest,const unsigned char *src,size_t length,badger_xor_t *data);

#endif

#endif
