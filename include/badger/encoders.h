#ifndef _BADGER_ENCODERS_H_
#define _BADGER_ENCODERS_H_

#include <sys/types.h>

typedef void (*badger_encoder_func)(unsigned char *dest,const unsigned char *src,size_t length,void *data);

#endif
