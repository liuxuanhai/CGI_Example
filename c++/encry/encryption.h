/******************************************    *******************************

      > File Name: encryption.h

      > Author: Ywl

      > Descripsion:

      > Created Time:     Thu 17 May 2018 06:53:29 PM PDT

      > Modify Time: 

 *********************************    ***************************************/

#ifndef _ENCRYPTION_H
#define _ENCRYPTION_H

#ifdef __cplusplus
extern "C" {
#endif

enum{
	MD5_TYPE = 0,
	SHA256_TYPE,
	SHA512_TYPE,
	DES_EN_TYPE,
	DES_DE_TYPE,
};

#define OWNER_DES_KEY		"yuyuan34"

unsigned char *openssl_lib(unsigned char encrypt_type, const unsigned char *dest_str, unsigned char len, unsigned char *md);

#define md5(str, len, md)	openssl_lib(MD5_TYPE, str, (len), md)
#define sha256(str, len, md)	openssl_lib(SHA256_TYPE, str, (len), md)
#define des_en(str, len, md)	openssl_lib(DES_EN_TYPE, str, (len), md)
#define des_de(str, len, md)	openssl_lib(DES_DE_TYPE, str, (len), md)

#ifdef __cplusplus
}
#endif

#endif
