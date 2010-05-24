#include <badger/encoders.h>

#ifdef XOR_ENCODER

void badger_xor_encoder(unsigned char *dest,const unsigned char *src,size_t length,badger_xor_t *data)
{
	unsigned int i;
	size_t pad_length = data->length;

	for (i=0;i<length;i++)
	{
		dest[i] = (src[i] ^ data->pad[i % pad_length]);
	}
}

#endif
