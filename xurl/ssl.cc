#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/error-ssl.h>
#include "ssl.h"

using namespace xurl;

int xe_ssl_ctx::init(){
	data = wolfSSL_CTX_new(wolfTLS_client_method());

	if(!data)
		return XE_ENOMEM;
	return 0;
}

int xe_ssl_ctx::load_verify_locations(xe_cstr cafile, xe_cstr capath){
	WOLFSSL_CTX* ctx = (WOLFSSL_CTX*)data;

	if(wolfSSL_CTX_load_verify_locations(ctx, cafile, capath) != WOLFSSL_SUCCESS){
		wolfSSL_CTX_free(ctx);

		return XE_SSL_BADCERTS;
	}

	return 0;
}

void xe_ssl_ctx::close(){
	if(data)
		wolfSSL_CTX_free((WOLFSSL_CTX*)data);
}

int xe_ssl::init(xe_ssl_ctx& ctx){
	WOLFSSL* ssl = wolfSSL_new((WOLFSSL_CTX*)ctx.data);

	if(!ssl)
		return XE_ENOMEM;
	wolfSSL_set_verify(ssl, SSL_VERIFY_NONE, null);

	data = ssl;

	return 0;
}

void xe_ssl::close(){
	if(data)
		wolfSSL_free((WOLFSSL*)data);
}

void xe_ssl::set_fd(int fd){
	wolfSSL_set_fd((WOLFSSL*)data, fd);
}

int xe_ssl::verify_host(const xe_string_view& host){
	WOLFSSL* ssl = (WOLFSSL*)data;
	xe_string host_copy;

	int err = 0;

	wolfSSL_set_verify(ssl, SSL_VERIFY_PEER, null);

	if(wolfSSL_UseSNI(ssl, WOLFSSL_SNI_HOST_NAME, host.c_str(), host.length()) != WOLFSSL_SUCCESS)
		return XE_ENOMEM;
	if(!host_copy.copy(host))
		return XE_ENOMEM;
	if(wolfSSL_check_domain_name(ssl, host_copy.c_str()) != WOLFSSL_SUCCESS)
		err = XE_ENOMEM;
	host_copy.free();

	return err;
}

int xe_ssl::set_alpn(const xe_string_view& protocols){
	WOLFSSL* ssl = (WOLFSSL*)data;

	if(wolfSSL_UseALPN(ssl, (char*)protocols.data(), protocols.length(), WOLFSSL_ALPN_CONTINUE_ON_MISMATCH) != WOLFSSL_SUCCESS)
		return XE_ENOMEM;
	return 0;
}

int xe_ssl::connect(int flags){
	WOLFSSL* ssl = (WOLFSSL*)data;

	wolfSSL_SetIOReadFlags(ssl, flags);
	wolfSSL_SetIOWriteFlags(ssl, flags);

	if(wolfSSL_connect(ssl) == WOLFSSL_SUCCESS)
		return 0;
	int err;

	switch(wolfSSL_get_error(ssl, -1)){
		case WOLFSSL_ERROR_WANT_READ:
		case WOLFSSL_ERROR_WANT_WRITE:
			return XE_EAGAIN;
		case DOMAIN_NAME_MISMATCH:
			return XE_SSL_PEER_VERIFICATION_FAILED;
		case ASN_NO_SIGNER_E:
			return XE_SSL_NO_SIGNER;
		case SOCKET_ERROR_E:
			if((err = xe_errno()))
				return err;
			return XE_ECONNRESET;
	}

	return XE_SSL;
}

int xe_ssl::get_alpn_protocol(xe_string_view& proto){
	WOLFSSL* ssl = (WOLFSSL*)data;

	char* name;
	ushort size;

	int err = wolfSSL_ALPN_GetProtocol(ssl, &name, &size);

	switch(err){
		case WOLFSSL_SUCCESS:
			proto = xe_string_view(name, size);

			return 0;
		case WOLFSSL_ALPN_NOT_FOUND:
			return XE_SSL;
		default:
			return XE_SSL;
	}
}

int xe_ssl::recv(xe_ptr buffer, size_t len, int flags){
	WOLFSSL* ssl = (WOLFSSL*)data;

	wolfSSL_SetIOReadFlags(ssl, flags);

	int recv = wolfSSL_read(ssl, buffer, xe_max(len, (size_t)INT_MAX));

	if(recv >= 0)
		return recv;
	switch(wolfSSL_get_error(ssl, -1)){
		case WOLFSSL_ERROR_WANT_READ:
		case WOLFSSL_ERROR_WANT_WRITE:
			return XE_EAGAIN;
		case SOCKET_ERROR_E:
			return xe_errno();
	}

	return XE_SSL;
}

int xe_ssl::send(xe_cptr buffer, size_t len, int flags){
	WOLFSSL* ssl = (WOLFSSL*)data;

	wolfSSL_SetIOWriteFlags(ssl, flags);

	if(len > INT_MAX)
		len = INT_MAX;
	int sent = wolfSSL_write(ssl, buffer, len);

	if(sent >= 0)
		return sent;
	switch(wolfSSL_get_error(ssl, -1)){
		case WOLFSSL_ERROR_WANT_READ:
		case WOLFSSL_ERROR_WANT_WRITE:
			return XE_EAGAIN;
		case SOCKET_ERROR_E:
			return xe_errno();
	}

	return XE_SSL;
}

int xurl::xe_ssl_init(){
	if(wolfSSL_Init() != WOLFSSL_SUCCESS)
		return XE_SSL;
	return 0;
}

void xurl::xe_ssl_cleanup(){
	wolfSSL_Cleanup();
}