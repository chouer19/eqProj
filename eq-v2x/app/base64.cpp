//======================================================================aaa
/* ! \file base64.cpp                                           
 *                                                                         
 * \copydoc Copyright                                                      
 * \author Xue Chong(xc)                                                   
 * \date Oct 13, 2019                                                     
 *                                                                         
 * base64 encode
 * Usage:                                                                  
 * ./base64 "dafdafdsafd"
 *///-------------------------------------------------------------------aaa 
#include <iostream>
#include <string.h>
using namespace std;
#define BASE64_PAD '='
#define BASE64_ENCODE_OUT_SIZE(s) ((int)((((s) + 2) / 3) * 4 + 1))
#define BASE64_DECODE_OUT_SIZE(s) ((int)(((s) / 4) * 3))

/* BASE 64 encode table */
static const char base64en[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};

int base64_encode(const char *in, int inlen, char *out)
{
	int s;
	int i;
	int j;
	char c;
	char l;

	s = 0;
	l = 0;
	for (i = j = 0; i < inlen; i++) {
		c = in[i];

		switch (s) {
		case 0:
			s = 1;
			out[j++] = base64en[(c >> 2) & 0x3F];
			break;
		case 1:
			s = 2;
			out[j++] = base64en[((l & 0x3) << 4) | ((c >> 4) & 0xF)];
			break;
		case 2:
			s = 0;
			out[j++] = base64en[((l & 0xF) << 2) | ((c >> 6) & 0x3)];
			out[j++] = base64en[c & 0x3F];
			break;
		}
		l = c;
	}

	switch (s) {
	case 1:
		out[j++] = base64en[(l & 0x3) << 4];
		out[j++] = BASE64_PAD;
		out[j++] = BASE64_PAD;
		break;
	case 2:
		out[j++] = base64en[(l & 0xF) << 2];
		out[j++] = BASE64_PAD;
		break;
	}

	out[j] = 0;

	return j;
}

int main(int argc, char **argv){
    //char *a = "qxnpxf001:9639f54";
    char *a = argv[1];
    char *encode_out;
    encode_out =(char *) malloc(BASE64_ENCODE_OUT_SIZE(strlen(a)));   
    base64_encode(a, strlen(a), encode_out);
    cout << encode_out << endl;
    return 1;
}
