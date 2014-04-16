#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

//Nビット回転
int bitShiftN(unsigned num, int n){
	return ((num) << n) | ((num) >> (32 - n));
}

//関数f
unsigned int f(int t, unsigned int B, unsigned int C, unsigned int D)
{
	if (t<20)
		return (B & C) | ((~B) & D);
	else if (t<40)
		return B ^ C ^ D;
	else if (t<60)
		return (B & C) | (B & D) | (C & D);
	else
		return B ^ C ^ D;
}

//定数K
unsigned int K(int t)
{
	if (t<20)
		return 0x5A827999;
	else if (t<40)
		return 0x6ED9EBA1;
	else if (t<60)
		return 0x8F1BBCDC;
	else
		return 0xCA62C1D6;
}

void SHA1f(char *str, unsigned int length, unsigned int *H){

	unsigned char msg[64];
	unsigned int W[80];
	unsigned int A, B, C, D, E;
	unsigned int tmp;

	//A~E初期値
	H[0] = 0x67452301;
	H[1] = 0xEFCDAB89;
	H[2] = 0x98BADCFE;
	H[3] = 0x10325476;
	H[4] = 0xC3D2E1F0;

	memcpy((char*)msg, str, length);

	//1の付加
	msg[length] = 0x80;
	//0の付加
	for (int i = length + 1; i<60; i++)
	{
		msg[i] = 0;
	}
	//サイズ情報
	msg[60] = length * 8 >> 24;
	msg[61] = length * 8 >> 16;
	msg[62] = length * 8 >> 8;
	msg[63] = length * 8;

	//W0~W15
	for (int i = 0; i<16; i++){
		W[i] = msg[i * 4] << 24;
		W[i] |= msg[i * 4 + 1] << 16;
		W[i] |= msg[i * 4 + 2] << 8;
		W[i] |= msg[i * 4 + 3];
	}

	//W16~W79
	for (int i = 16; i<80; i++){
		W[i] = bitShiftN(W[i - 16] ^ W[i - 14] ^ W[i - 8] ^ W[i - 3], 1);
	}

	A = H[0];
	B = H[1];
	C = H[2];
	D = H[3];
	E = H[4];

	for (int i = 0; i<80; i++)
	{
		tmp = bitShiftN(A, 5) + f(i, B, C, D) + E + W[i] + K(i);
		E = D;
		D = C;
		C = bitShiftN(B, 30);
		B = A;
		A = tmp;
	}

	H[0] += A;
	H[1] += B;
	H[2] += C;
	H[3] += D;
	H[4] += E;

}

int main(){
	unsigned int sha1num[5];
	char str[100];

	printf("文字列\t：");
	gets(str);

	SHA1f(str, strlen(str), sha1num);
	
	printf("SHA-1\t：");
	for (int i = 0; i<5; i++)
		printf("%x", sha1num[i]);

	printf("\n");
	return 0;
}