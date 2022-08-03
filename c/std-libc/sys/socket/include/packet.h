#ifndef __PACKET_H__
#define __PACKET_H__

#define DEFAULT_PACKET "default.packet"
#define ETH_802_1Q_PACKET "802.1Q.packet"

int new_packet (void **packet, const char *path);

void free_packet (void *packet);


#endif /* __PACKET_H__ */