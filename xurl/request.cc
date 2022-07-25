#include "request.h"
#include "xutil/xutil.h"

using namespace xurl;

xe_request::xe_request(){
	data = null;
	callbacks.state = null;
	callbacks.write = null;
	callbacks.done = null;
	state = XE_REQUEST_STATE_IDLE;
}

xe_request::~xe_request(){
	if(data) xe_delete(data);
}

void xe_request::set_state_cb(state_cb cb){
	callbacks.state = cb;
}

void xe_request::set_write_cb(write_cb cb){
	callbacks.write = cb;
}

void xe_request::set_done_cb(done_cb cb){
	callbacks.done = cb;
}

void xe_request::set_port(ushort port){
	((xe_net_common_data*)data) -> set_port(port);
}

void xe_request::set_connect_timeout(uint timeout_ms){
	((xe_net_common_data*)data) -> set_connect_timeout(timeout_ms);
}

void xe_request::set_ssl_verify(bool verify){
	((xe_net_common_data*)data) -> set_ssl_verify(verify);
}

void xe_request::set_ip_mode(xe_ip_mode mode){
	((xe_net_common_data*)data) -> set_ip_mode(mode);
}

void xe_request::set_recvbuf_size(uint size){
	((xe_net_common_data*)data) -> set_recvbuf_size(size);
}

void xe_request::set_max_redirects(uint max_redirects){
	((xe_http_common_data*)data) -> set_max_redirects(max_redirects);
}

void xe_request::set_follow_location(bool follow){
	((xe_http_common_data*)data) -> set_follow_location(follow);
}

int xe_request::set_http_header(const xe_string_view& key, const xe_string_view& value, bool copy){
	return ((xe_http_specific*)data) -> set_header(key, value, copy);
}

int xe_request::set_http_method(const xe_string_view& method, bool copy){
	return ((xe_http_specific*)data) -> set_method(method, copy);
}

void xe_request::set_http_statusline_cb(xe_http_statusline_cb cb){
	((xe_http_specific*)data) -> set_statusline_cb(cb);
}

void xe_request::set_http_singleheader_cb(xe_http_singleheader_cb cb){
	((xe_http_specific*)data) -> set_singleheader_cb(cb);
}

void xe_request::set_http_response_cb(xe_http_response_cb cb){
	((xe_http_specific*)data) -> set_response_cb(cb);
}

void xe_request::set_http_trailer_cb(xe_http_singleheader_cb cb){
	((xe_http_specific*)data) -> set_trailer_cb(cb);
}

int xe_request::ws_send(xe_websocket_message_type type, xe_cptr data, size_t size){
	return ((xe_websocket_data*)data) -> send(type, data, size);
}

int xe_request::ws_ping(){
	return ((xe_websocket_data*)data) -> ping();
}

int xe_request::ws_pong(){
	return ((xe_websocket_data*)data) -> pong();
}

void xe_request::set_ws_ready_cb(xe_websocket_ready_cb cb){
	((xe_websocket_data*)data) -> set_ready_cb(cb);
}

void xe_request::set_ws_ping_cb(xe_websocket_ping_cb cb){
	((xe_websocket_data*)data) -> set_ping_cb(cb);
}

void xe_request::set_ws_message_cb(xe_websocket_message_cb cb){
	((xe_websocket_data*)data) -> set_message_cb(cb);
}

void xe_request::close(){
	if(data){
		xe_delete(data);

		data = null;
	}
}