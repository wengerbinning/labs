#ifndef __PROVISION_H__
#define __PROVISION_H__

/* MTD image head */
typedef struct image_head_t {
    union {
        struct {
            unsigned int Signature;
            unsigned int Version;
            unsigned int Size;
            unsigned int SizeUsed;

            unsigned short Id;
            unsigned short Checksum;
            unsigned short Year;
            unsigned short MonthDay;
            unsigned short HourMinute;

            unsigned short model_key;
            unsigned short FwVerMask;
            unsigned short supported_bits[4];
            unsigned short oem_id;

            unsigned int CompatibleVersion[2];
            unsigned int ProvisionCounter;

            unsigned char rsa_signature[256];
            unsigned char reserved0[156];

            unsigned short phase;
        };

        unsigned short pad[0x200 >> 1];
    };
} image_head_t;



int prov_func (void *data, size_t *dlen);

#endif /* __PROVISION_H__ */