#include <stdio.h>
#include <string.h>

//Nビット回転
int bitShiftN(unsigned num, int n){
	return ((num) << n) | ((num) >> 32 - n);
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

void Sha1Step(unsigned int *W, unsigned int *H){
	unsigned int A, B, C, D, E;
	unsigned int tmp;

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

void SHA1f(char *p, unsigned int length, unsigned int *pH){

	unsigned char Block[64];
	unsigned int WBlock[80];

	//A~E初期値
	pH[0] = 0x67452301;
	pH[1] = 0xEFCDAB89;
	pH[2] = 0x98BADCFE;
	pH[3] = 0x10325476;
	pH[4] = 0xC3D2E1F0;

	memcpy((char*)Block, p, length);

	//1の付加
	Block[length] = 0x80;
	//0の付加
	for (int i = length + 1; i<60; i++)
	{
		Block[i] = 0;
	}
	//サイズ情報
	Block[60] = length * 8 >> 24;
	Block[61] = length * 8 >> 16;
	Block[62] = length * 8 >> 8;
	Block[63] = length * 8;

	//W0~W15
	for (int i = 0; i<16; i++){
		WBlock[i] = Block[i * 4] << 24;
		WBlock[i] |= Block[i * 4 + 1] << 16;
		WBlock[i] |= Block[i * 4 + 2] << 8;
		WBlock[i] |= Block[i * 4 + 3];
	}

	Sha1Step(WBlock, pH);

}

int main(){
	unsigned int Result[5];
	char str[100];

	printf("文字列\t：");
	gets_s(str,sizeof(str));

	SHA1f(str, strlen(str), Result);

	printf("SHA-1\t：");
	for (int i = 0; i<5; i++)
		printf("%x", Result[i]);

	printf("\n");
	return 0;
}