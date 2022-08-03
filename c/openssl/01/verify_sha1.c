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
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2U8EdsP47EWqTCBVKq+i\n\
/JKpbZ+5jF5/7I8sLH2oBVKw5bbKoGZRxcA4GIYUASLFPnA4n0SMEA8QF+31WcWL\n\
Sv3bAPbjtic6YrKswzrg0z9CF13HcHmLYO1GJ+VrLDQaiqUnTi7rDQEZJM6lVHBJ\n\
2uF4gyt7JJVw+Gnf3hc2YoeGz3GjEbfsRrnGCFam3/QGMyTjx43np2pFCIqXyyCZ\n\
R3uSiv++/yYRGj0MiWenD8j/VwXHEJ8nZiN7mwyMMqjvT6lPV84OLpKQQQQGPlbg\n\
tFdl7nGNTLnFCVjw4+B7/6PgHmJb0iMPuxF/FDd2ZGoS8HMYMu0HOeVEbpj8C06z\n\
iQIDAQAB\n\
-----END PUBLIC KEY-----\n"

// Digital Signature

#define RSA_SIGNATURE_SIZE  256


enum signature_hash {
	SIGN_HASH_SHA1,
	SIGN_HASH_SHA256,
};

		int signature_verify (
			const void *pubkey,
			void *signature,
			unsigned char hash,
			void *digest
		)
		{
			int ret, dmlen;
			BIO *bio;
			EVP_PKEY *pkey;
			EVP_PKEY_CTX *ctx;

			/** BIO Buffer */
			if (!(bio = BIO_new_mem_buf((void *)pubkey, -1))) {
				return 0;
			}
			BIO_set_mem_eof_return(bio, 1);
			pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
			/** Context */
			ctx = EVP_PKEY_CTX_new(pkey, NULL);
			EVP_PKEY_verify_init(ctx);
			/** Hash */
			EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING);
			switch (hash) {
				case SIGN_HASH_SHA256:
					dmlen = 32;
					EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256());
					break;
				case SIGN_HASH_SHA1:
				default:
					dmlen = 20;
					EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha1());
			}
			/** verify */
			if ((ret = EVP_PKEY_verify(ctx, signature, 256, digest, dmlen))) {
				ret = 1;
			} else {
				ret = 0;
			}

			EVP_PKEY_CTX_free(ctx);
			BIO_free(bio);

			return ret;
		}





int rsa_signature_verify_sha1 (const void *kepub, void *signature, void *digest)
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
   	if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha1()) <= 0 ) {
    	goto out_ctx;
	}

    /** verify */
	if (!(ret = EVP_PKEY_verify(ctx, signature, 256, digest, 20))) {
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

	rlen = fread(hash, 1, 20, file2);
	printf("Hash: SHA1 \n");
	hexdump(hash, 20);
	if (signature_verify(PUB_KEY, sign, SIGN_HASH_SHA1, hash)) {
		printf("PASS\n");
	} else {
		printf("Failed\n");
	}

	return 0;
}