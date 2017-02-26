#include "oi_tea.h"

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++ i) 
	{
		unsigned char pbyOut[256] = {0};
		int nOutLength = sizeof(pbyOut);

		static int tKey[ 4 ] = {4356,78534,46677,3545};
		static unsigned char* tpKey = (unsigned char*)&tKey[0];

		oi_symmetry_encrypt( (const unsigned char*) argv[i], strlen(argv[i]), tpKey, pbyOut, &nOutLength );

		printf("encrypt result: [%s]-->[", argv[i]);

		for (int i = 0; i < nOutLength; ++i)
		{
			printf("%02x", pbyOut[i]);
		}

		printf("]\n");
	}

	return 0;
}
