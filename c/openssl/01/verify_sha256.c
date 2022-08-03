#include <stdio.h>

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

#include "develop.h"
#include "hexdump.h"

// #define PUB_KEY	"gwn_firmware_public"
#define PRI_KEY "gwn_firmware_private"

// Devel public key
#define PUB_KEY \
"-----BEGIN PUBLIC KEY-----\n\
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAnUlVOE9ybHTMOYZ+OeUs\n\
8vyFSRGVERVPXi2torp1opkXIo5qG4y0n/bCfDSkP/F36bqOcczq9f1Vx4wvQ4Ky\n\
FM7yFMk3chEEh2gnrMNZYg16c8Q3EATRz8x/J5qKdq8HICNZBwcX/JXVGs8w3oJJ\n\
tqdVMeMNBRV1EaQt8CU34pC4DmEIoR0+/bHgW9HjVj+4UxQGyf3lAq0Y6UVPQcXk\n\
KlLojKDCLNtF8c2xYb9FzqcBSSKzyPMdM0cSuB8hOqiUMnkG7jTdSyBimzo/Ffdb\n\
qz0qTeMuwmEpCu5YApzr4rS2MjRfKlcTvUeu9P3+2ZkOq327s/JMEoSyUnq70arT\n\
0wIDAQAB\n\
-----END PUBLIC KEY-----\n"

// Digital Signature

#define RSA_SIGNATURE_SIZE  256


int rsa_signature_verify_sha256 (const void *kepub, void *signature, void *digest)
// Description: Verify RSA-signature of a SHA1 digest value.
//      Return 0 (failure) or 1 (success).
// ============================================================================
{
	int ret = 0;
	BIO *biobuf;
	EVP_PKEY *pkey;
	EVP_PKEY_CTX *ctx;

	/** BIO Buffer */
	if (!(biobuf = BIO_new_mem_buf((void *)kepub, -1))) {
		return -1;
    }
    BIO_set_mem_eof_return(biobuf, 1);
    if (!(pkey = PEM_read_bio_PUBKEY(biobuf, NULL, NULL, NULL))) {
        goto out_bio;
    }


    /** Context */
    if (!(ctx = EVP_PKEY_CTX_new(pkey, NULL))) {
        goto out_bio;
    }
    if (EVP_PKEY_verify_init(ctx) <= 0) {
        goto out_ctx;
    }

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
        goto out_ctx;
    }
	/** SHA256 */
   	if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0 ) {
    	goto out_ctx;
	}

    /** verify */
	if (!(ret = EVP_PKEY_verify(ctx, signature, 256, digest, 32))) {
		printf("Failed\n");
    } else {
		printf("Passed\n");
	}

out_ctx:
    EVP_PKEY_CTX_free(ctx);

out_bio:
    BIO_free(biobuf);

    return ret;
}



#define BUFIZE	1000
int main(int argc, char *argv[]) {
	EVP_MD_CTX *ctx;

	char sign[256];
	char hash[32];

	FILE *file1, *file2;
	size_t len = 1024;
	char *data, *buf;
	int ret, idx;
	int rlen;


	if (!argv[1])
		return 0;

	if (!(file1 = fopen(argv[1], "rb"))) {
		error(0, "Failed to open %s", argv[1]);
		return 0;
	}
	if (!(file2 = fopen(argv[2], "rb"))) {
		error(0, "Failed to open %s", argv[1]);
		return 0;
	}
	printf("%s\n", PUB_KEY);
	rlen = fread(sign, 1, 256, file1);
	printf("Signature \n");
	hexdump(sign, 256);

	rlen = fread(hash, 1, 32, file2);
	printf("Hash: SHA256 \n");
	hexdump(hash, 32);
	rsa_signature_verify_sha256(PUB_KEY, sign, hash);

	return 0;
}