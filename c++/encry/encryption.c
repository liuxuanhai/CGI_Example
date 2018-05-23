/******************************************    *******************************

      > File Name: encryption.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Thu 17 May 2018 06:53:01 PM PDT

      > Modify Time: 

 *********************************    ***************************************/

#include <stdio.h>
#include <string.h>

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/des.h>

#include "encryption.h"

typedef unsigned char *(*encrypt_fun)(const unsigned char *, size_t, unsigned char *);
typedef unsigned char *(*abstract_fun)(unsigned char, const unsigned char *, unsigned char, unsigned char *);

static unsigned char *md5_sha256_abstract(unsigned char encrypt_type, const unsigned char *dest_str, unsigned char len, unsigned char *md)
{

	encrypt_fun encrypt;
	unsigned char tmp_md[64] = {0};
	char dest_key[128] = {0};
	char char_tmp[3] = {0};
	char i = 0;

	switch(encrypt_type)
	{
		case MD5_TYPE:
			encrypt = MD5;
			break;
		case SHA256_TYPE:
			encrypt = SHA256;
			break;
		default:
			return NULL;
	}
	if(NULL == encrypt(dest_str, len, tmp_md))
	{
		return NULL;
	}
	for(i = 0; i < 32; i++)
	{
		sprintf(char_tmp, "%02x", tmp_md[i]);
		strcat(dest_key, char_tmp);
	}
	dest_key[32] = '\0';
	memcpy(md, dest_key, 32);
	return md;
}


static unsigned char *md5_abstract(unsigned char encrypt_type, const unsigned char *dest_str, unsigned char len, unsigned char *md5)
{
	return md5_sha256_abstract(encrypt_type, dest_str, len, md5);
}

static unsigned char *sha256_abstract(unsigned char encrypt_type, const unsigned char *dest_str, unsigned char len, unsigned char *sha256)
{
	return md5_sha256_abstract(encrypt_type, dest_str, len, sha256);
}


#define DES_KEY_LEN	8

static unsigned char *__des_en_de__(int mode, const unsigned char *dest_str, unsigned char dest_len, unsigned char *key, unsigned char key_len, unsigned char *md)
{
	unsigned char vec_cipher_text[512] = {0};
	unsigned char tmp[DES_KEY_LEN] = {0};
	char char_tmp[3] = {0};
	
	int i = 0;

	DES_cblock key_encrypt;

	/* 构造8位长度的密钥 */
	memset(key_encrypt, 0, DES_KEY_LEN);
	if(key_len <= DES_KEY_LEN)
	{
		memcpy(key_encrypt, key, key_len);
	}
	else
	{
		memcpy(key_encrypt, key, DES_KEY_LEN);
	}
	/* 密钥置换 */
	DES_key_schedule key_schedule;
	DES_set_key_unchecked(&key_encrypt, &key_schedule);

	const_DES_cblock input_text;
	DES_cblock output_text;

	for(i = 0; i < dest_len / DES_KEY_LEN; i++)
	{
		memcpy(input_text, dest_str+i*DES_KEY_LEN, DES_KEY_LEN);
		DES_ecb_encrypt(&input_text, &output_text, &key_schedule, mode);
		memcpy(tmp, output_text, DES_KEY_LEN);

		memcpy(vec_cipher_text+i*DES_KEY_LEN, tmp, DES_KEY_LEN);
	}

	if(dest_len % DES_KEY_LEN != 0)
	{
		int tmp1 = dest_len / DES_KEY_LEN * DES_KEY_LEN;
		int tmp2 = dest_len - tmp1;
		memset(input_text, 0, DES_KEY_LEN);
		memcpy(input_text, dest_str+tmp1, tmp2);

		DES_ecb_encrypt(&input_text, &output_text, &key_schedule, mode);
		memcpy(tmp, output_text, DES_KEY_LEN);

		memcpy(vec_cipher_text+i*DES_KEY_LEN, tmp, DES_KEY_LEN);
		i++;
	}
	memcpy(md, vec_cipher_text, i*DES_KEY_LEN);

	return md;
}
static unsigned char *__des_encrypt(const unsigned char *dest_str, unsigned char dest_len, unsigned char *key, unsigned char key_len, unsigned char *md)
{
	return __des_en_de__(DES_ENCRYPT, dest_str, dest_len, key, key_len, md);
}
static unsigned char *__des_decrypt(const unsigned char *dest_str, unsigned char dest_len, unsigned char *key, unsigned char key_len, unsigned char *md)
{
	return __des_en_de__(DES_DECRYPT, dest_str, dest_len, key, key_len, md);
}

static unsigned char *des_encrypt(unsigned char encrypt_type, const unsigned char *dest_str, unsigned char len, unsigned char *md)
{
	return __des_encrypt(dest_str, len, (unsigned char *)OWNER_DES_KEY, strlen(OWNER_DES_KEY), md);
}
static unsigned char *des_decrypt(unsigned char encrypt_type, const unsigned char *dest_str, unsigned char len, unsigned char *md)
{
	return __des_decrypt(dest_str, len, (unsigned char *)OWNER_DES_KEY, strlen(OWNER_DES_KEY), md);
}

unsigned char *openssl_lib(unsigned char encrypt_type, const unsigned char *dest_str, unsigned char len, unsigned char *md)
{
	abstract_fun abstract;

	switch(encrypt_type)
	{
		case MD5_TYPE:
			abstract = md5_abstract;
			break;
		case SHA256_TYPE:
			abstract = sha256_abstract;
			break;
		case DES_EN_TYPE:
			abstract = des_encrypt;
			break;
		case DES_DE_TYPE:
			abstract = des_decrypt;
			break;
		default:
			return NULL;
	}

	return abstract(encrypt_type, dest_str, len, md);
}




#if 0
int main()
{
	unsigned char md[33] = {0};
	unsigned char md2[33] = {0};
	const unsigned char *pass = "password_key";

	md5(pass, strlen(pass), md);

	printf("md5: %s\n", md);

	memset(md, 0, sizeof(md));
	sha256(pass, strlen(pass), md);

	printf("sha256: %s\n", md);

	memset(md, 0, sizeof(md));
	des_en(pass, strlen(pass), md);

	printf("des: %s\n", md);

	des_de(md, strlen(md), md2);

	printf("des: %s\n", md2);
	return 0;
}
#endif
