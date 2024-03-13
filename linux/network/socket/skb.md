
## Type

#### struct sk_buff

```c
struct sk_buff {
	union {
		struct {
			/* These two members must be first. */
			struct sk_buff		*next;
			struct sk_buff		*prev;

			union {
				struct net_device	*dev;
				/* Some protocols might use this space to store information,
				 * while device pointer would be NULL.
				 * UDP receive path is one user.
				 */
				unsigned long		dev_scratch;
			};
		};
		struct rb_node		rbnode; /* used in netem, ip4 defrag, and tcp stack */
		struct list_head	list;
	};

	union {
		struct sock		*sk;
		int			ip_defrag_offset;
	};

	union {
		ktime_t		tstamp;
		u64		skb_mstamp_ns; /* earliest departure time */
	};
	/*
	 * This is the control buffer. It is free to use for every
	 * layer. Please put your private variables there. If you
	 * want to keep them across layers you have to do a skb_clone()
	 * first. This is owned by whoever has the skb queued ATM.
	 */
	char			cb[48] __aligned(8);

	union {
		struct {
			unsigned long	_skb_refdst;
			void		(*destructor)(struct sk_buff *skb);
		};
		struct list_head	tcp_tsorted_anchor;
	};

#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	unsigned long		 _nfct;
#endif
	unsigned int		len, data_len;
	__u16			mac_len, hdr_len;

	/* Following fields are _not_ copied in __copy_skb_header()
	 * Note that queue_mapping is here mostly to fill a hole.
	 */
	__u16			queue_mapping;

/* if you move cloned around you also must adapt those constants */
#ifdef __BIG_ENDIAN_BITFIELD
#define CLONED_MASK	(1 << 7)
#else
#define CLONED_MASK	1
#endif
#define CLONED_OFFSET()		offsetof(struct sk_buff, __cloned_offset)

	__u8			__cloned_offset[0];
	__u8			cloned:1,
				nohdr:1,
				fclone:2,
				peeked:1,
				head_frag:1,
				pfmemalloc:1;
#ifdef CONFIG_SKB_EXTENSIONS
	__u8			active_extensions;
#endif
	/* fields enclosed in headers_start/headers_end are copied
	 * using a single memcpy() in __copy_skb_header()
	 */
	/* private: */
	__u32			headers_start[0];
	/* public: */

/* if you move pkt_type around you also must adapt those constants */
#ifdef __BIG_ENDIAN_BITFIELD
#define PKT_TYPE_MAX	(7 << 5)
#else
#define PKT_TYPE_MAX	7
#endif
#define PKT_TYPE_OFFSET()	offsetof(struct sk_buff, __pkt_type_offset)

	__u8			__pkt_type_offset[0];
	__u8			pkt_type:3;
	__u8			ignore_df:1;
	__u8			nf_trace:1;
	__u8			ip_summed:2;
	__u8			ooo_okay:1;

	__u8			l4_hash:1;
	__u8			sw_hash:1;
	__u8			wifi_acked_valid:1;
	__u8			wifi_acked:1;
	__u8			no_fcs:1;
	/* Indicates the inner headers are valid in the skbuff. */
	__u8			encapsulation:1;
	__u8			encap_hdr_csum:1;
	__u8			csum_valid:1;

#ifdef __BIG_ENDIAN_BITFIELD
#define PKT_VLAN_PRESENT_BIT	7
#else
#define PKT_VLAN_PRESENT_BIT	0
#endif
#define PKT_VLAN_PRESENT_OFFSET()	offsetof(struct sk_buff, __pkt_vlan_present_offset)
	__u8			__pkt_vlan_present_offset[0];
	__u8			vlan_present:1;
	__u8			csum_complete_sw:1;
	__u8			csum_level:2;
	__u8			csum_not_inet:1;
	__u8			dst_pending_confirm:1;
#ifdef CONFIG_IPV6_NDISC_NODETYPE
	__u8			ndisc_nodetype:2;
#endif

	__u8			ipvs_property:1;
	__u8			inner_protocol_type:1;
	__u8			remcsum_offload:1;
#ifdef CONFIG_NET_SWITCHDEV
	__u8			offload_fwd_mark:1;
	__u8			offload_l3_fwd_mark:1;
#endif
#ifdef CONFIG_NET_CLS_ACT
	__u8			tc_skip_classify:1;
	__u8			tc_at_ingress:1;
#endif
#ifdef CONFIG_NET_REDIRECT
	__u8			redirected:1;
	__u8			from_ingress:1;
#endif
#ifdef CONFIG_TLS_DEVICE
	__u8			decrypted:1;
#endif
	__u8			scm_io_uring:1;
	__u8			gro_skip:1;

#ifdef CONFIG_NET_SCHED
	__u16			tc_index;	/* traffic control index */
#endif

	union {
		__wsum		csum;
		struct {
			__u16	csum_start;
			__u16	csum_offset;
		};
	};
	__u32			priority;
	int			skb_iif;
	__u32			hash;
	__be16			vlan_proto;
	__u16			vlan_tci;
#if defined(CONFIG_NET_RX_BUSY_POLL) || defined(CONFIG_XPS)
	union {
		unsigned int	napi_id;
		unsigned int	sender_cpu;
	};
#endif
#ifdef CONFIG_NETWORK_SECMARK
	__u32		secmark;
#endif

	union {
		__u32		mark;
		__u32		reserved_tailroom;
	};

	union {
		__be16		inner_protocol;
		__u8		inner_ipproto;
	};

	__u16			inner_transport_header;
	__u16			inner_network_header;
	__u16			inner_mac_header;

	__be16			protocol;
	__u16			transport_header;
	__u16			network_header;
	__u16			mac_header;

	/* private: */
	__u32			headers_end[0];
	/* public: */

	/* These elements must be at the end, see alloc_skb() for details.  */
	sk_buff_data_t		tail;
	sk_buff_data_t		end;
	unsigned char		*head,
				*data;
	unsigned int		truesize;
	refcount_t		users;

#ifdef CONFIG_SKB_EXTENSIONS
	/* only useable after checking ->active_extensions != 0 */
	struct skb_ext		*extensions;
#endif
	__u32			pkt_mark;
};
```

#### struct iphdr

```c
struct iphdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8	    ihl:4,
		    version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
	__u8	version:4,
  		        ihl:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif

	__u8	tos;
	__be16	tot_len;
	__be16	id;
	__be16	frag_off;
	__u8	ttl;
	__u8	protocol;
	__sum16	check;
	__be32	saddr;
	__be32	daddr;
	/*The options start here. */
};
```
#### struct ethhdr

```c
struct ethhdr {
        unsigned char   h_dest[ETH_ALEN];       /* destination eth addr */
        unsigned char   h_source[ETH_ALEN];     /* source ether addr    */
        __be16          h_proto;                /* packet type ID field */
} __attribute__((packed));
```

#### struct tcphdr

```c
struct tcphdr {
	__be16	source;
	__be16	dest;
	__be32	seq;
	__be32	ack_seq;
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u16	res1:4,
		doff:4,
		fin:1,
		syn:1,
		rst:1,
		psh:1,
		ack:1,
		urg:1,
		ece:1,
		cwr:1;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u16	doff:4,
		res1:4,
		cwr:1,
		ece:1,
		urg:1,
		ack:1,
		psh:1,
		rst:1,
		syn:1,
		fin:1;
#else
#error	"Adjust your <asm/byteorder.h> defines"
#endif	
	__be16	window;
	__sum16	check;
	__be16	urg_ptr;
};
```

#### struct udphdr

```c
struct udphdr {
	__be16	source;
	__be16	dest;
	__be16	len;
	__sum16	check;
};
```

## Interface

#### skb_put

```c
void *skb_put (struct sk_buff *skb, unsigned int len)
{
	void *tmp = skb_tail_pointer(skb);
	SKB_LINEAR_ASSERT(skb);
	skb->tail += len;
	skb->len  += len;
	if (unlikely(skb->tail > skb->end))
		skb_over_panic(skb, len, __builtin_return_address(0));
	return tmp;
}
```

#### skb_push

```c
void *skb_push(struct sk_buff *skb, unsigned int len)
{
	skb->data -= len;
	skb->len  += len;
	if (unlikely(skb->data < skb->head))
		skb_under_panic(skb, len, __builtin_return_address(0));
	return skb->data;
}
```

#### skb_pull

```c
void *skb_pull(struct sk_buff *skb, unsigned int len)
{
	return skb_pull_inline(skb, len);
}
EXPORT_SYMBOL(skb_pull);
```

#### skb_network_header

```c
static inline unsigned char *skb_network_header (const struct sk_buff *skb)
{
	return skb->head + skb->network_header;
}
```


#### skb_network_offset

```c
static inline int skb_network_offset (const struct sk_buff *skb)
{
	return skb_network_header(skb) - skb->data;
}
```

## Example



```c
if (skb->protocol == htons(ETH_P_IP)) {
    struct iphdr *iph = ip_hdr(skb);

    switch (iph->protocol) {
        case IPPROTO_TCP: {
            struct tcphdr *th = tcp_hdr(skb);
            printk("src ip: %pI4 dst ip: %pI4, TCP,  src port:%u, dst port:%u;",
                &iph->saddr, &iph->daddr,
                ntohs(th->source), ntohs(th->dest));
        }
            break;
        case IPPROTO_UDP: {
            struct udphdr *uh = udp_hdr(skb);
            printk("src ip: %pI4 dst ip: %pI4, UDP, src port:%u, dst port:%u;",
                &iph->saddr, &iph->daddr,
                ntohs(uh->source), ntohs(uh->dest));
        }
            break;
        default:
            printk("src ip: %pI4 dst ip: %pI4, protocol: %d", 
                &iph->saddr, &iph->daddr, iph->protocol);
    }
}
```