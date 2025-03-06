#define _GNU_SOURCE

#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>




#define REGEX_NMATCH    (1 + 4 + 4)

/* IPv4 Field Regex
 */
#define IP4_BREGEX "([0-1]{1,8})"
#define IP4_DREGEX "([0-9]{1,3})"
#define IP4_HREGEX "([0-9a-fA-F]{1,2})"
#define IP4ADDR_REGEX0  IP4_BREGEX" "IP4_BREGEX" "IP4_BREGEX" "IP4_BREGEX
#define IP4ADDR_REGEX1  IP4_DREGEX"."IP4_DREGEX"."IP4_DREGEX"."IP4_DREGEX
#define IP4ADDR_REGEX2  IP4_HREGEX":"IP4_HREGEX":"IP4_HREGEX":"IP4_HREGEX
#define IP4ADDR_REGEX   IP4ADDR_REGEX1
/* IPv4 String Regex
 - ip4addr: 192.168.0.1                    # IPv4 Address
 - ip4mask: 192.168.0.1/24                 # IPv4 Address with netmask
 - iprang1: 192.168.10.0-255               # IPv4 Range 1
 - iprang2: 192.168.10.1 - 192.168.10.100  # IPv4 Range 2
 */
#define REGEX_IP4ADDR "^"IP4ADDR_REGEX"$"
#define REGEX_IP4MASK "^"IP4ADDR_REGEX"/([0-9]{1,2})$"
#define REGEX_IPRANG1 "^"IP4ADDR_REGEX"[-~]([0-9]{1,3})$"
#define REGEX_IPRANG2 "^"IP4ADDR_REGEX"[-~]"IP4ADDR_REGEX"$"
#define IP4(idx, val) ((val) << (idx * 8))
#define IP4ADDR(a4, a3, a2, a1) (IP4(3,a4) + IP4(2,a3) + IP4(1,a2) + IP4(0,a1))
#define _IP4(idx, addr) (((addr) >> (idx * 8)) & 0xFF)
#define IP42STR(a) _IP4(3, a), _IP4(2, a), _IP4(1, a), _IP4(0, a)
#define NETMASK(m) (~((1U << (32-m))-1))
/* IPv4 Address Format
 - IP4FMT0: 192.168.0.1
 - IP4FMT1: 192.168.0.1/24
 */
#define IP4ADDR_FMT1 "%hhu.%hhu.%hhu.%hhu"
#define IP4ADDR_FMT2 "%03hhu.%03hhu.%03hhu.%03hhu"
#define IP4MASK_FMT1 "%hhu.%hhu.%hhu.%hhu/%hhu"
#define IP4MASK_FMT2 "%03hhu.%03hhu.%03hhu.%03hhu/%02hhu"
#define IP4FMT0 IP4ADDR_FMT1
#define IP4FMT1 IP4MASK_FMT1
#define IP4FMT  IP4FMT0
#define _IP4OFST(addr, n) *((const unsigned char *)addr + n)
#define IP2STR(a) _IP4OFST(a,3),_IP4OFST(a,2),_IP4OFST(a,1),_IP4OFST(a,0)


#define IPF_VALID   (1U << 0)

#define IPv4    0
#define IPv6    1

struct ip4addr {
    struct ip4addr *prev, *next;
    int idx, map;
};

struct ipaddr {
    union {
        struct ip4addr ip4;
    };
};

struct subnet {
    int flags, proto;
    int addr, mask;
    struct ipaddr cur;
    struct ipaddr *static_pool, *active_pool;
    struct ipaddr *reserv_pool, *forbid_pool;
};



int ip4_parse_addr(struct subnet *subnet, const char *ipstr)
{
    unsigned char a, a1, a2, a3, a4;
    unsigned char e, e1, e2, e3, e4;
    unsigned char m;
    char *buf;
    regex_t regex;
    size_t nmatch = REGEX_NMATCH + 1;
    regmatch_t pmatch[REGEX_NMATCH + 1];

    buf = strdup(ipstr);

    /* IPv4 Address */
    regcomp(&regex, REGEX_IP4ADDR, REG_EXTENDED);
    if (0 == regexec(&regex, buf, nmatch, pmatch, 0)) {
        memset(buf + pmatch[1].rm_eo, 0, 1); a4 = atoi(buf + pmatch[1].rm_so);
        memset(buf + pmatch[2].rm_eo, 0, 1); a3 = atoi(buf + pmatch[2].rm_so);
        memset(buf + pmatch[3].rm_eo, 0, 1); a2 = atoi(buf + pmatch[3].rm_so);
        memset(buf + pmatch[4].rm_eo, 0, 1); a1 = atoi(buf + pmatch[4].rm_so);
        m = 32;
        subnet->flags |= IPF_VALID;
        subnet->proto = IPv4;
    }
    regfree(&regex);
    /* IPv4 Address with Mask */
    regcomp(&regex, REGEX_IP4MASK, REG_EXTENDED);
    if (0 == regexec(&regex, buf, nmatch, pmatch, 0)) {
        memset(buf + pmatch[1].rm_eo, 0, 1); a4 = atoi(buf + pmatch[1].rm_so);
        memset(buf + pmatch[2].rm_eo, 0, 1); a3 = atoi(buf + pmatch[2].rm_so);
        memset(buf + pmatch[3].rm_eo, 0, 1); a2 = atoi(buf + pmatch[3].rm_so);
        memset(buf + pmatch[4].rm_eo, 0, 1); a1 = atoi(buf + pmatch[4].rm_so);
        memset(buf + pmatch[5].rm_eo, 0, 1); m  = atoi(buf + pmatch[5].rm_so);
        subnet->flags |= IPF_VALID;
        subnet->proto = IPv4;
    }
    regfree(&regex);
    /* IPv4 Address Range 1 */
    regcomp(&regex, REGEX_IPRANG1, REG_EXTENDED);
    if (0 == regexec(&regex, buf, nmatch, pmatch, 0)) {
        // memset(buf + pmatch[0].rm_eo, 0, 1);
        memset(buf + pmatch[1].rm_eo, 0, 1); a4 = atoi(buf + pmatch[1].rm_so);
        memset(buf + pmatch[2].rm_eo, 0, 1); a3 = atoi(buf + pmatch[2].rm_so);
        memset(buf + pmatch[3].rm_eo, 0, 1); a2 = atoi(buf + pmatch[3].rm_so);
        memset(buf + pmatch[4].rm_eo, 0, 1); a1 = atoi(buf + pmatch[4].rm_so);
        memset(buf + pmatch[5].rm_eo, 0, 1); e1 = atoi(buf + pmatch[5].rm_so);
        subnet->flags |= IPF_VALID;
        subnet->proto = IPv4;
    }
    regfree(&regex);
    /* IPv4 Address Range 2 */
    regcomp(&regex, REGEX_IPRANG2, REG_EXTENDED);
    if (0 == regexec(&regex, buf, nmatch, pmatch, 0)) {
        // memset(buf + pmatch[0].rm_eo, 0, 1);
        memset(buf + pmatch[1].rm_eo, 0, 1); a4 = atoi(buf + pmatch[1].rm_so);
        memset(buf + pmatch[2].rm_eo, 0, 1); a3 = atoi(buf + pmatch[2].rm_so);
        memset(buf + pmatch[3].rm_eo, 0, 1); a2 = atoi(buf + pmatch[3].rm_so);
        memset(buf + pmatch[4].rm_eo, 0, 1); a1 = atoi(buf + pmatch[4].rm_so);
        memset(buf + pmatch[1].rm_eo, 0, 1); e4 = atoi(buf + pmatch[1].rm_so);
        memset(buf + pmatch[2].rm_eo, 0, 1); e3 = atoi(buf + pmatch[2].rm_so);
        memset(buf + pmatch[3].rm_eo, 0, 1); e2 = atoi(buf + pmatch[3].rm_so);
        memset(buf + pmatch[4].rm_eo, 0, 1); e1 = atoi(buf + pmatch[4].rm_so);
        subnet->flags |= IPF_VALID;
        subnet->proto = IPv4;
    }
    regfree(&regex);
    free(buf);

    if (!(subnet->flags & IPF_VALID)) {

        return -1;
    }
    switch (subnet->proto) {
    case IPv4:
        subnet->addr = IP4ADDR(a4, a3, a2, a1);
        printf(IP4FMT", %08X\n", a4, a3, a2, a1, IP4ADDR(a4, a3, a2, a1));
        subnet->mask = m;
        break;
    case IPv6:
        break;
    }

    return 0;
}

int main (int argc, char *argv[]) {
    struct subnet subnet = {0};


    if (!argv[1]) {
        printf("Please input Address:\n");
        return 0;
    }


    if (ip4_parse_addr(&subnet, argv[1]) < 0) {
        printf("Address %s Error!\n", argv[1]);
        return 0;
    }




    printf("Address:"IP4FMT"/%d\n", IP42STR(subnet.addr), subnet.mask);

    return 0;
}
