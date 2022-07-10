#pragma once
#include "xe/types.h"
#include "../protocol.h"

namespace xurl{

class xe_net_common_data : public xe_protocol_specific{
protected:
	uint connect_timeout;
	uint recvbuf_size;
	ushort port;
	xe_ip_mode ip_mode;

	bool ssl_verify: 1;

	xe_net_common_data(xe_protocol_id id): xe_protocol_specific(id){
		connect_timeout = 0;
		recvbuf_size = 0;
		port = 0;
		ip_mode = XE_IP_ANY;
		ssl_verify = true;
	}
public:
	void set_connect_timeout(uint connect_timeout_){
		connect_timeout = connect_timeout_;
	}

	uint get_connect_timeout(){
		return connect_timeout;
	}

	void set_recvbuf_size(uint recvbuf_size_){
		recvbuf_size = recvbuf_size_;
	}

	uint get_recvbuf_size(){
		return recvbuf_size;
	}

	void set_port(ushort port_){
		port = port_;
	}

	ushort get_port(){
		return port;
	}

	void set_ip_mode(xe_ip_mode ip_mode_){
		ip_mode = ip_mode_;
	}

	xe_ip_mode get_ip_mode(){
		return ip_mode;
	}

	void set_ssl_verify(bool ssl_verify_){
		ssl_verify = ssl_verify_;
	}

	bool get_ssl_verify(){
		return ssl_verify;
	}
};

}