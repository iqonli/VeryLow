#include <bits/stdc++.h>
using namespace std;
string byteArrayToHexString(const vector<unsigned char>& byteArray)
{
	stringstream _ss;
	for(unsigned char _byte : byteArray)
	{
		_ss<<hex<<setw(2)<<setfill('0')<<(int)_byte;
	}
	return _ss.str();
}
vector<unsigned char> _PD_hash_sha256_text(const string& _data)
{
	// 定义常量和变量
	const unsigned int K[64]={
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	
	unsigned int H[8]={
		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};
	
	// 预处理
	vector<unsigned char> bytes(_data.begin(),_data.end());
	bytes.push_back(0x80);
	while((bytes.size()+8)%64!=0)
	{
		bytes.push_back(0x00);
	}
	uint64_t bitLength=_data.size()*8;
	for(int i=7;i>=0;i--)
	{
		bytes.push_back(bitLength>>(i*8));
	}
	// 处理每个块
	for(size_t i=0;i<bytes.size();i+=64)
	{
		unsigned int W[64];
		for(int j=0;j<16;j++)
		{
			W[j] = (bytes[i + 4 * j] << 24) | (bytes[i + 4 * j + 1] << 16) | (bytes[i + 4 * j + 2] << 8) | bytes[i + 4 * j + 3];
		}
		for (int j=16;j<64;j++)
		{
			unsigned int s0 = _rotr(W[j - 15], 7) ^ _rotr(W[j - 15], 18) ^ (W[j - 15] >> 3);
			unsigned int s1 = _rotr(W[j - 2], 17) ^ _rotr(W[j - 2], 19) ^ (W[j - 2] >> 10);
			W[j] = W[j - 16] + s0 + W[j - 7] + s1;
		}
		unsigned int a = H[0];
		unsigned int b = H[1];
		unsigned int c = H[2];
		unsigned int d = H[3];
		unsigned int e = H[4];
		unsigned int f = H[5];
		unsigned int g = H[6];
		unsigned int h = H[7];
		for(int j=0;j<64;j++)
		{
			unsigned int S1 = _rotr(e, 6) ^ _rotr(e, 11) ^ _rotr(e, 25);
			unsigned int ch = (e & f) ^ (~e & g);
			unsigned int temp1 = h + S1 + ch + K[j] + W[j];
			unsigned int S0 = _rotr(a, 2) ^ _rotr(a, 13) ^ _rotr(a, 22);
			unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
			unsigned int temp2 = S0 + maj;
			h=g;g=f;f=e;
			e=d+temp1;
			d=c;c=b;b=a;
			a=temp1+temp2;
		}
		H[0]+=a;
		H[1]+=b;
		H[2]+=c;
		H[3]+=d;
		H[4]+=e;
		H[5]+=f;
		H[6]+=g;
		H[7]+=h;
	}
	vector<unsigned char> _hash;
	for(int i=0;i<8;i++)
	{
		_hash.push_back(H[i] >> 24);
		_hash.push_back((H[i] >> 16) & 0xff);
		_hash.push_back((H[i] >> 8) & 0xff);
		_hash.push_back(H[i] & 0xff);
	}
	return _hash;
}
string PD_hash_sha256_text(string a)
{
	return byteArrayToHexString(_PD_hash_sha256_text(a));
}
