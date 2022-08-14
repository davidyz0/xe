#pragma once
#include "xstd/types.h"
#include "xurl.h"
#include "url.h"

namespace xurl{

enum xe_protocol_id{
	XE_PROTOCOL_NONE = -1,
	XE_PROTOCOL_FILE,
	XE_PROTOCOL_HTTP,
	XE_PROTOCOL_WEBSOCKET,

	XE_PROTOCOL_LAST
};

class xe_protocol_specific{
protected:
	xe_protocol_id id_;

	xe_protocol_specific(xe_protocol_id id){
		id_ = id;
	}
public:
	xe_protocol_id id(){
		return id_;
	}

	virtual ~xe_protocol_specific(){}
};

class xe_protocol{
protected:
	xe_protocol_id id_;
	xurl_ctx* ctx;

	xe_protocol(xurl_ctx& ctx_, xe_protocol_id id){
		ctx = &ctx_;
		id_ = id;
	}
public:
	xe_protocol_id id() const{
		return id_;
	}

	virtual int start(xe_request_internal& request) = 0;
	virtual int transferctl(xe_request_internal& request, uint flags) = 0;
	virtual void end(xe_request_internal& request) = 0;

	virtual bool matches(const xe_string_view& scheme) const = 0;
	virtual int open(xe_request_internal& request, xe_url&& url) = 0;

	virtual ~xe_protocol(){}
};

}