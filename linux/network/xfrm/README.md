
## 基础原理

#### 安全策略

#### 安全关联

## 源码文件

Kconfig
Makefile

xfrm_algo.c
xfrm_device.c
xfrm_hash.c
xfrm_hash.h
xfrm_inout.h
xfrm_input.c
xfrm_interface.c
xfrm_ipcomp.c
xfrm_output.c
xfrm_policy.c
xfrm_proc.c
xfrm_replay.c
xfrm_state.c
xfrm_sysctl.c
xfrm_user.c

## 源码分析

### 符号表

#### 框架管理

* xfrm4_init

* xfrm_lookup
* xfrm_lookup_route
* xfrm_migrate

* xfrmi4_init
* xfrmi4_fini

* xfrmi6_init
* xfrmi6_fini

* xfrmi_init
* xfrmi_fini

#### 安全策略

* xfrm_policy_timer
* xfrm_policy_alloc
* xfrm_policy_destroy_rcu
* xfrm_policy_destroy
* xfrm_policy_kill

* policy_hash_bysel

* xfrm_policy_inexact_alloc_chain
* xfrm_policy_inexact_prune_bin
* xfrm_policy_inexact_insert
* xfrm_policy_hash_rebuild
* xfrm_policy_requeue
* xfrm_policy_mark_match
* xfrm_policy_insert_list
* xfrm_policy_insert
* xfrm_policy_byid
* xfrm_policy_flush_secctx_check
* xfrm_policy_walk


* xfrm_policy_delete

* xfrmi_rcv_cb

##### IPv4

* xfrm_input
* xfrmi4_err

* xfrmi_esp4_protocol - struct xfrm4_protocol
* xfrmi_ah4_protocol - struct xfrm4_protocol
* xfrmi_ipcomp4_protocol - struct xfrm4_protocol

##### IPv6

* xfrm6_rcv
* xfrmi4_err

* xfrmi_esp6_protocol - struct xfrm4_protocol
* xfrmi_ah6_protocol - struct xfrm4_protocol
* xfrmi_ipcomp6_protocol - struct xfrm4_protocol


#### 发送数据

* xfrm4_route_forward

* xfrm4_policy_check -> __xfrm_policy_check2 -> __xfrm_policy_check

xfrm4_output



#### 接收数据

* esp4_protocol - struct xfrm
* xfrm4_transport_input



* xfrm_input_init

* xfrm_input
* xfrm_input_resume
* xfrm_trans_reinject
* xfrm_trans_queue


* xfrm4_transport_input

* xfrm6_transport_input

#### ESP

esp_type - struct xfrm_type
esp_init_state
esp_destroy
esp_input
esp_output

esp4_protocol - struct xfrm4_protocol
xfrm4_rcv
xfrm_input
esp4_rcv_cb
esp4_err

### 调用链

xfrm_lookup -> xfrm_lookup_with_ifid
xfrm_lookup_with_ifid -> xfrm_resolve_and_create_bundle

#### 发送数据

`ip_route_output_flow` 进入 `xfrm_lookup_route`

* xfrm_lookup_route -> xfrm_lookup -> xfrm_lookup_with_ifid

* xfrm_lookup_with_ifid






xfrm4_output -> NF_HOOK_COND(NFPROTO_IPV4,NF_INET_POST_ROUTING,__xfrm4_output)

__xfrm4_output -> dst_output
__xfrm4_output -> xfrm4_output_finish
xfrm4_output_finish -> xfrm_output

xfrm_output -> xfrm_dev_offload_ok
xfrm_output -> xfrm_output_gso
xfrm_output -> xfrm_output2

xfrm_output2 -> xfrm_output_resume

xfrm_output_resume -> xfrm_output_one
xfrm_output_resume -> skb_dst(skb)->ops->local_out
xfrm_output_resume -> nf_hook(skb_dst(skb)->ops->family,NF_INET_POST_ROUTING,xfrm_output2)

xfrm_output_one -> xfrm_outer_mode_output
xfrm_output_one -> xfrm_offload
xfrm_output_one -> x->type_offload->encap
xfrm_output_one -> x->type->output



xfrm_outer_mode_output -> xfrm4_prepare_output
xfrm_outer_mode_output -> xfrm4_transport_output

xfrm4_prepare_output -> xfrm_inner_extract_output
xfrm4_prepare_output -> xfrm4_beet_encap_add
xfrm4_prepare_output -> xfrm4_tunnel_encap_add

xfrm4_tunnel_encap_add -> skb_set_inner_network_header
xfrm4_tunnel_encap_add -> skb_set_inner_transport_header
xfrm4_tunnel_encap_add -> skb_set_network_header
xfrm4_tunnel_encap_add -> INET_ECN_encapsulate

xfrm_outer_mode_output -> xfrm6_prepare_output
xfrm_outer_mode_output -> xfrm6_transport_output


xfrm4_transport_output


xfrm_input ->
xfrm_input -> x->type_offload->input_tail


esp_input -> crypto_aead_decrypt





######

xfrm_prepare_input -> afinfo->extract_input

xfrm_ip2inner_mode

#### 转发数据




xfrm4_policy_check -> xfrm_policy_check -> __xfrm_policy_check


* ip_forward -> xfrm4_route_forward

* xfrm4_route_forward -> xfrm_route_forward -> __xfrm_route_forward
* __xfrm_route_forward -> xfrm_lookup
* __xfrm_route_forward -> skb_dst_set

* xfrm_lookup -> xfrm_lookup_with_ifid



#### 接收数据

* xfrm4_rcv -> xfrm4_rcv_spi -> xfrm_input






##### ESP数据加密

esp_output -> crypto_aead_authsize
esp_output -> esp_output_tail

esp_output_tail -> crypto_aead_authsize
esp_output_tail -> crypto_aead_ivsize
esp_output_tail -> aead_request_set_callback
esp_output_tail -> aead_request_set_crypt
esp_output_tail -> esp_output_restore_header
esp_output_tail -> crypto_aead_encrypt

##### ESP数据解密

esp_input -> esp_input_set_header
esp_input -> aead_request_set_callback
esp_input -> aead_request_set_crypt
esp_input -> crypto_aead_decrypt
esp_input -> esp_input_done2(skb, err);

esp_input_done2 -> esp_remove_trailer
esp_input_done2 -> skb_pull_rcsum
esp_input_done2 -> skb_reset_transport_header
esp_input_done2 -> skb_set_transport_header




validate_xmit_xfrm ->