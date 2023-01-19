/*
 * sfe_cm.h
 *	Shortcut forwarding engine.
 *
 * Copyright (c) 2013-2015 The Linux Foundation. All rights reserved.
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * connection flags.
 */
#define SFE_CREATE_FLAG_NO_SEQ_CHECK 0x1
					/* Indicates that we should not check sequence numbers */

/*L2TP on SFE Flags
*/
#ifndef SFE_SUPPORT_IPV6
#define SFE_SUPPORT_IPV6
#endif
#ifdef FEATURE_L2TP_OVER_SFE
#define SFE_PASS_L2TP_CONFIG_TO_SFE 0xA0
#define SFE_DEL_L2TP_CONFIG_FROM_SFE 0xA1
#define L2TP_GENERIC_IFACE_NAME "l2tpeth"
#define L2TP_ETH_MIN_LENGTH 7
#define NL_L2TP_PROTO_ID 24
#define SFE_L2TP_MAX_CONF 10
#define L2TP_TUNNEL_SIZE 8
#define COOKIE_SIZE 8
#define UDP_HDR_SIZE 8
#define OUTER_ETH_HDR_SIZE 14
#define IPV6_HDR_SIZE 40
#define INNER_L2TP_ETH_HDR_SIZE 14
#define CISCO_HDLC_SIZE 4
#define MAC_ADDR_SIZE 6
#define IPV6_ADDR_SIZE 16
#endif

/*
 * IPv6 address structure
 */
struct sfe_ipv6_addr {
	__be32 addr[4];
};

typedef union {
	__be32			ip;
	struct sfe_ipv6_addr	ip6[1];
} sfe_ip_addr_t;

#define MAX_WLAN_INDEX 4
typedef enum
{
	SFE_WLAN_LINK_INDEX_NONE = -1,
	SFE_WLAN_LINK_INDEX0 = 0,
	SFE_WLAN_LINK_INDEX1 = 1,
	SFE_WLAN_LINK_INDEX2 = 2,
	SFE_WLAN_LINK_INDEX3 = 3
}sfe_wlan_index_type;

#ifdef FEATURE_L2TP_OVER_SFE
/*
 * Data struct to represent L2TP Tunnel config.
 */
struct sfe_l2tp_config {
	uint8_t command;
	uint16_t local_tunnel_id;
	/* local Tunnel ID*/

	char parent_iface[MAX_IFACE_NAME_SIZE];
	/* Local iface on which tunnel is created*/

	char l2tp_iface[MAX_IFACE_NAME_SIZE];
	uint32_t session_id;
	uint32_t peer_session_id;
	uint8_t src_addr[IPV6_ADDR_SIZE];
	uint8_t dest_addr[IPV6_ADDR_SIZE];
	uint16_t src_port;
	uint16_t dest_port;
	uint8_t mac_addr_src[MAC_ADDR_SIZE];
	uint8_t mac_addr_dest[MAC_ADDR_SIZE];
};

struct sfe_l2tp_session_array{
	uint8_t num_sessions;
	struct sfe_l2tp_config session[SFE_L2TP_MAX_CONF];
};

extern struct sfe_l2tp_session_array sfe_l2tp_session_arr;

#endif

/*
 * connection creation structure.
 */
struct sfe_connection_create {
	int protocol;
	struct net_device *src_dev;
	struct net_device *dest_dev;
#ifdef FEATURE_L2TP_OVER_SFE
	struct net_device *parent_dev;
#endif
	uint32_t flags;
	uint32_t src_mtu;
	uint32_t dest_mtu;
	sfe_ip_addr_t src_ip;
	sfe_ip_addr_t src_ip_xlate;
	sfe_ip_addr_t dest_ip;
	sfe_ip_addr_t dest_ip_xlate;
	__be16 src_port;
	__be16 src_port_xlate;
	__be16 dest_port;
	__be16 dest_port_xlate;
	uint8_t src_mac[ETH_ALEN];
	uint8_t src_mac_xlate[ETH_ALEN];
	uint8_t dest_mac[ETH_ALEN];
	uint8_t dest_mac_xlate[ETH_ALEN];
	uint8_t src_td_window_scale;
	uint32_t src_td_max_window;
	uint32_t src_td_end;
	uint32_t src_td_max_end;
	uint8_t dest_td_window_scale;
	uint32_t dest_td_max_window;
	uint32_t dest_td_end;
	uint32_t dest_td_max_end;
	uint32_t mark;
#ifdef CONFIG_XFRM
	uint32_t original_accel;
	uint32_t reply_accel;
#endif
	uint32_t src_priority;
	uint32_t dest_priority;
	uint32_t src_dscp;
	uint32_t dest_dscp;
#ifdef FEATURE_L2TP_OVER_SFE
	bool l2tp_traffic;
	struct sfe_l2tp_session_array sfe_config_array;
#endif
};

/*
 * connection destruction structure.
 */
struct sfe_connection_destroy {
	int protocol;
	sfe_ip_addr_t src_ip;
	sfe_ip_addr_t dest_ip;
	__be16 src_port;
	__be16 dest_port;
};

typedef enum sfe_sync_reason {
	SFE_SYNC_REASON_STATS,	/* Sync is to synchronize stats */
	SFE_SYNC_REASON_FLUSH,	/* Sync is to flush a entry */
	SFE_SYNC_REASON_DESTROY	/* Sync is to destroy a entry(requested by connection manager) */
} sfe_sync_reason_t;

/*
 * Structure used to sync connection stats/state back within the system.
 *
 * NOTE: The addresses here are NON-NAT addresses, i.e. the true endpoint addressing.
 * 'src' is the creator of the connection.
 */
struct sfe_connection_sync {
	struct net_device *src_dev;
	struct net_device *dest_dev;
	int is_v6;			/* Is it for ipv6? */
	int protocol;			/* IP protocol number (IPPROTO_...) */
	sfe_ip_addr_t src_ip;		/* Non-NAT source address, i.e. the creator of the connection */
	sfe_ip_addr_t src_ip_xlate;	/* NATed source address */
	__be16 src_port;		/* Non-NAT source port */
	__be16 src_port_xlate;		/* NATed source port */
	sfe_ip_addr_t dest_ip;		/* Non-NAT destination address, i.e. to whom the connection was created */
	sfe_ip_addr_t dest_ip_xlate;	/* NATed destination address */
	__be16 dest_port;		/* Non-NAT destination port */
	__be16 dest_port_xlate;		/* NATed destination port */
	uint32_t src_td_max_window;
	uint32_t src_td_end;
	uint32_t src_td_max_end;
	uint64_t src_packet_count;
	uint64_t src_byte_count;
	uint32_t src_new_packet_count;
	uint32_t src_new_byte_count;
	uint32_t dest_td_max_window;
	uint32_t dest_td_end;
	uint32_t dest_td_max_end;
	uint64_t dest_packet_count;
	uint64_t dest_byte_count;
	uint32_t dest_new_packet_count;
	uint32_t dest_new_byte_count;
	uint32_t reason;		/* reason for stats sync message, i.e. destroy, flush, period sync */
	uint64_t delta_jiffies;		/* Time to be added to the current timeout to keep the connection alive */
};

/*
 * connection mark structure
 */
struct sfe_connection_mark {
	int protocol;
	sfe_ip_addr_t src_ip;
	sfe_ip_addr_t dest_ip;
	__be16 src_port;
	__be16 dest_port;
	uint32_t mark;
};

#ifdef FEATURE_L2TP_OVER_SFE
/*
 * sfe_l2tp_find_parent_dev()
 * Function used to find the parent dev from the iface name.
 * Parent dev is crucial to update the dev info in post-routing chain.
 * Same dev is used to find a connection match for subsequent packets.
 */
static inline void sfe_l2tp_find_parent_dev
(
	char *source_intf_name,
	struct sfe_l2tp_session_array *sfe_l2tp_session_arr,
	struct net_device **src_dev
)
{
	int i=0;
	for (i=0;i<SFE_L2TP_MAX_CONF;i++) {
		if (strncmp(source_intf_name,
			sfe_l2tp_session_arr->session[i].l2tp_iface,
			MAX_IFACE_NAME_SIZE) == 0) {
			*src_dev =
			dev_get_by_name(
				&init_net,
				sfe_l2tp_session_arr->session[i].parent_iface);

			DEBUG_INFO(
			"session_idx =%d, parent_iface = %s\n",
			sfe_l2tp_session_arr->session[i].session_id,
			sfe_l2tp_session_arr->session[i].parent_iface);
		}
	}
}

/*
 * A L2TP over UDP V3 header
 * help with performance on some platforms (see the definition of
 * SFE_IPV6_UNALIGNED_STRUCT)
 */
struct sfe_l2tp_udp_hdr {
	__u16 flags_and_ver;
	__u16 reserved;
	__u32 session_id;
	__u64 cookie;
} SFE_IPV6_UNALIGNED_STRUCT;

/*
 * find_l2tp_dev_in_sfe_l2tp_arr()
 * common api to find session based on xmit dev
 */
static inline int find_l2tp_dev_in_sfe_l2tp_arr(char* cm_xmit_dev, int len)
{
	int i=0;
	for (i=0;i<SFE_L2TP_MAX_CONF;i++)
	{
		if (strncmp(sfe_l2tp_session_arr.session[i].l2tp_iface, cm_xmit_dev, len) == 0) {
			DEBUG_TRACE_LOW("L2TP Session found %d: l2tp1:%s l2tp2:%s parent:%s",
				i, cm_xmit_dev, sfe_l2tp_session_arr.session[i].l2tp_iface,
				sfe_l2tp_session_arr.session[i].parent_iface);
			return i;
		}
	}
	DEBUG_TRACE_LOW("L2TP Session not found wrong dev: %s",cm_xmit_dev);
	return -1;
}

/*
 * sfe_dev_get_bridge()
 * get master bridge dev
 */
static inline struct net_device *sfe_dev_get_bridge(struct net_device *dev)
{
	struct net_device *master;
	rcu_read_lock();
	master = netdev_master_upper_dev_get_rcu(dev);
	if (master)
		dev_hold(master);

	rcu_read_unlock();
	return master;
}

/*
 * build_l2tp_over_udp_hdr()
 * common api to api to build the Outer ETH + IPV6 + UDP + L2TP headers
 */
static inline int build_l2tp_over_udp_hdr(struct sk_buff *skb, unsigned int len, int session_idx)
{
	int ret = 0;
	struct ipv6hdr* ip6_hdr;
	struct udphdr* udp_hdr;
	struct sfe_l2tp_udp_hdr* l2tp_hdr;
	struct ethhdr* eth_mac_hdr;
	struct net_device* vlan_dev;

	/* only build header and update dev if we get non-NULL dev in dev_get_by_name */
	vlan_dev = dev_get_by_name(
				&init_net,
				sfe_l2tp_session_arr.session[session_idx].parent_iface);
	if(!vlan_dev){
		DEBUG_TRACE_LOW("Dev null, skipping header addition, parent iface:%s",
			sfe_l2tp_session_arr.session[session_idx].parent_iface);
		return 1;
	}

	if (skb_headroom(skb) < sizeof(struct sfe_l2tp_udp_hdr) + sizeof(struct udphdr)+ sizeof(struct ipv6hdr) + sizeof(struct ethhdr)){
		ret = pskb_expand_head(skb,
				HH_DATA_ALIGN(sizeof(struct sfe_l2tp_udp_hdr) + sizeof(struct udphdr)+ sizeof(struct ipv6hdr) + sizeof(struct ethhdr) - skb_headroom(skb)),
				0,
				GFP_ATOMIC);
		if (ret) {
			kfree_skb(skb);
			DEBUG_TRACE_LOW(
				"pskb_expand_head failed = %d",
				ret);
			return 0;
		}
	}

	/* Build the Outer eth + IPV6 + UDP + L2TP headers */

	/*Insert L2TP header*/
	l2tp_hdr = (struct sfe_l2tp_udp_hdr*)__skb_push(skb, sizeof(struct sfe_l2tp_udp_hdr));
	memset(l2tp_hdr, 0, sizeof(struct sfe_l2tp_udp_hdr));
	l2tp_hdr->flags_and_ver = htons(0x0003);
	l2tp_hdr->reserved = htons(0x0000);
	l2tp_hdr->session_id = htonl(sfe_l2tp_session_arr.session[session_idx].peer_session_id);

	/*Insert UDP header*/
	__skb_push(skb, sizeof(struct udphdr));
	skb_reset_transport_header(skb);
	udp_hdr = (struct udphdr *)skb_transport_header(skb);
	udp_hdr->source = htons(sfe_l2tp_session_arr.session[session_idx].src_port);
	udp_hdr->dest = htons(sfe_l2tp_session_arr.session[session_idx].dest_port);
	udp_hdr->len = htons(len + sizeof(struct sfe_l2tp_udp_hdr) + sizeof(struct udphdr) + sizeof(struct ethhdr));

	/*Insert ipv6 header*/
	__skb_push(skb, sizeof(struct ipv6hdr));
	skb_reset_network_header(skb);
	ip6_hdr = (struct ipv6hdr*)skb_network_header(skb);
	memset(ip6_hdr, 0, sizeof(struct ipv6hdr));
	ip6_hdr->version = 6; //IPv6
	ip6_hdr->payload_len = htons(len + sizeof(struct sfe_l2tp_udp_hdr) + sizeof(struct udphdr) + sizeof(struct ethhdr));
	ip6_hdr->nexthdr = IPPROTO_UDP;
	ip6_hdr->hop_limit = 64;
	memcpy(ip6_hdr->saddr.s6_addr, sfe_l2tp_session_arr.session[session_idx].src_addr,
		sizeof(sfe_l2tp_session_arr.session[session_idx].src_addr)); /* source IPv6 addr */
	memcpy(ip6_hdr->daddr.s6_addr, sfe_l2tp_session_arr.session[session_idx].dest_addr,
		sizeof(sfe_l2tp_session_arr.session[session_idx].dest_addr)); /* dest IPv6 addr */

	/*Checksum calculations for UDP header*/
	DEBUG_TRACE_LOW("current len:%d",udp_hdr->len);
		skb->ip_summed = CHECKSUM_PARTIAL;
		skb->csum_start = skb_transport_header(skb) - skb->head;
		skb->csum_offset = offsetof(struct udphdr, check);
		udp_hdr->check = ~udp_v6_check(ntohs(udp_hdr->len), &ip6_hdr->saddr, &ip6_hdr->daddr, 0);

	/*Insert eth mac header*/
	__skb_push(skb, sizeof(struct ethhdr));
	skb_reset_mac_header(skb); /* this adds the mac header offset to the head pointer */
	eth_mac_hdr = (struct ethhdr*)skb_mac_header(skb); /* this returns the ptr of head ptr + L2 layer offset*/
	memcpy(eth_mac_hdr->h_dest, sfe_l2tp_session_arr.session[session_idx].mac_addr_dest,
		sizeof(sfe_l2tp_session_arr.session[session_idx].mac_addr_dest)); /* source mac addr */
	memcpy(eth_mac_hdr->h_source, sfe_l2tp_session_arr.session[session_idx].mac_addr_src,
		sizeof(sfe_l2tp_session_arr.session[session_idx].mac_addr_src)); /* dest mac addr */
	eth_mac_hdr->h_proto = htons(0x86dd);

	/*Change skb proto to v6 and disable checksum and update xmit dev*/
	skb->protocol = htons(ETH_P_IPV6);
	skb->dev = vlan_dev;

	return 1;
}

#endif

/*Common API for sfe tcpdump enablement */
#ifdef ISKERNEL5_15
	bool sfe_tcpdump_enable = true;
#else
	static int sfe_tcpdump_enable = 1;
#endif

static inline int sfe_tcpdump_log(struct sk_buff *skb, struct packet_type *pt_prev)
{
	struct net_device *dev;
	int ret = true;
	dev = skb->dev;

	if (pt_prev) {
#if ISKERNEL4_14
		refcount_inc(&skb->users);
#else
		atomic_inc(&skb->users);
#endif
		ret = pt_prev->func(skb, skb->dev, pt_prev, dev);
	}
	return ret;
}

/*
 * Type used for a sync rule callback.
 */
typedef void (*sfe_sync_rule_callback_t)(struct sfe_connection_sync *);
/*
 * IPv4 APIs used by connection manager
 */
extern int sfe_ipv4_recv(struct net_device *dev, struct sk_buff *skb, struct packet_type *pt_prev);
extern int sfe_ipv4_create_rule(struct sfe_connection_create *sic);
extern void sfe_ipv4_destroy_rule(struct sfe_connection_destroy *sid);
extern void sfe_ipv4_destroy_all_rules_for_dev(struct net_device *dev);
extern void sfe_ipv4_register_sync_rule_callback(sfe_sync_rule_callback_t callback);
extern void sfe_ipv4_update_rule(struct sfe_connection_create *sic);
extern void sfe_ipv4_mark_rule(struct sfe_connection_mark *mark);

#ifdef SFE_SUPPORT_IPV6
/*
 * IPv6 APIs used by connection manager
 */
#ifdef FEATURE_L2TP_OVER_SFE
extern int sfe_l2tp_ipv6_recv(struct sk_buff *skb, unsigned int ihl,struct packet_type *pt_prev);
extern int sfe_l2tp_ipv6_udp_recv(struct sfe_l2tp_config *conn, struct sk_buff *skb, unsigned int ihl,struct packet_type *pt_prev);
#endif
extern int sfe_ipv6_recv(struct net_device *dev, struct sk_buff *skb, struct packet_type *pt_prev);
extern int sfe_ipv6_create_rule(struct sfe_connection_create *sic);
extern void sfe_ipv6_destroy_rule(struct sfe_connection_destroy *sid);
extern void sfe_ipv6_destroy_all_rules_for_dev(struct net_device *dev);
extern void sfe_ipv6_register_sync_rule_callback(sfe_sync_rule_callback_t callback);
extern void sfe_ipv6_update_rule(struct sfe_connection_create *sic);
extern void sfe_ipv6_mark_rule(struct sfe_connection_mark *mark);
#else

#ifdef FEATURE_L2TP_OVER_SFE
static inline int sfe_l2tp_ipv6_recv
(
	struct sk_buff *skb,
	unsigned int ihl,
	struct packet_type *pt_prev
)
{
	return 0;
}

static inline int sfe_l2tp_ipv6_udp_recv
(
	struct sfe_l2tp_config *conn,
	struct sk_buff *skb, unsigned int ihl,
	struct packet_type *pt_prev
);
{
	return 0;
}
#endif

static inline int sfe_ipv6_recv(struct net_device *dev, struct sk_buff *skb)
{
	return 0;
}

static inline int sfe_ipv6_create_rule(struct sfe_connection_create *sic)
{
	return -1;
}

static inline void sfe_ipv6_destroy_rule(struct sfe_connection_destroy *sid)
{
	return;
}

static inline void sfe_ipv6_destroy_all_rules_for_dev(struct net_device *dev)
{
	return;
}

static inline void sfe_ipv6_register_sync_rule_callback(sfe_sync_rule_callback_t callback)
{
	return;
}

static inline void sfe_ipv6_update_rule(struct sfe_connection_create *sic)
{
	return;
}

static inline void sfe_ipv6_mark_rule(struct sfe_connection_mark *mark)
{
	return;
}
#endif
