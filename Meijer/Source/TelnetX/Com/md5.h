#ifndef __MD5
#define __MD5

typedef unsigned int uint32;
typedef struct {
    uint32 h[4];
} MD5_Core_State;


class MD5
{
protected:
	void MD5Init(void);
	void MD5_Core_Init(MD5_Core_State *a);
	void MD5_Block(MD5_Core_State *a,uint32 *block);
	struct 
	MD5Context {
		MD5_Core_State core;
		_TUCHAR block[64];
		int blkused;
		uint32 lenhi, lenlo;
		};

	struct MD5Context md5c;
	struct MD5Context *s;

public:

		 MD5(void);
	void Update(_TUCHAR const *p,unsigned len);
	void Final(_TUCHAR output[16]);

};

#endif