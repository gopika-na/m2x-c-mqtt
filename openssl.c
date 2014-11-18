#include "openssl.h"

int openssl_mqttread(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
  OpenSSLNetwork *ssl_network = (OpenSSLNetwork *) n;
  int bytes, rc;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }
  setsockopt(ssl_network->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));

  bytes = 0;
  while (bytes < len) {
    rc = SSL_read(ssl_network->handle, (buffer + bytes), (len - bytes));
    if (rc < 0) {
      return rc;
    } else if (rc == 0) {
      return bytes;
    } else {
      bytes += rc;
    }
  }
  return bytes;
}

int openssl_mqttwrite(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
  OpenSSLNetwork *ssl_network = (OpenSSLNetwork *) n;
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }
  setsockopt(ssl_network->socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&interval, sizeof(struct timeval));
  return SSL_write(ssl_network->handle, buffer, len);
}

void openssl_disconnect(Network *n)
{
  OpenSSLNetwork *ssl_network = (OpenSSLNetwork *) n;
  if (ssl_network->socket > 0) {
    close(ssl_network->socket);
    ssl_network->socket = 0;
  }
  if (ssl_network->handle) {
    SSL_shutdown(ssl_network->handle);
    SSL_free(ssl_network->handle);
    ssl_network->handle = NULL;
  }
  if (ssl_network->context) {
    SSL_CTX_free(ssl_network->context);
    ssl_network->context = NULL;
  }
}

void NewOpenSSLNetwork(OpenSSLNetwork *n)
{
  n->socket = 0;
  n->mqttread = openssl_mqttread;
  n->mqttwrite = openssl_mqttwrite;
  n->disconnect = openssl_disconnect;
  n->handle = NULL;
  n->context = NULL;
}

int ConnectOpenSSLNetwork(OpenSSLNetwork *n, char *addr, int port)
{
  struct sockaddr_in address;
  int rc = -1;
  sa_family_t family;
  struct addrinfo *result = NULL;
  struct addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};
  struct addrinfo *res = NULL;
  if ((rc = getaddrinfo(addr, NULL, &hints, &result)) == 0) {
    res = result;
    while (res) {
      if (res->ai_family == AF_INET) {
        result = res;
        break;
      }
      res = res->ai_next;
    }
    if (result->ai_family == AF_INET) {
      address.sin_port = htons(port);
      address.sin_family = family = AF_INET;
      address.sin_addr = ((struct sockaddr_in*)(result->ai_addr))->sin_addr;
    } else {
      rc = -1;
    }
    freeaddrinfo(result);
  }
  if (rc < 0) { return rc; }
  n->socket = socket(family, SOCK_STREAM, 0);
  if (n->socket == -1) { return -1; }
  rc = connect(n->socket, (struct sockaddr *) &address, sizeof(address));
  if (rc < 0) { return rc; }
  /* SSL part */
  SSL_load_error_strings();
  SSL_library_init();
  n->context = SSL_CTX_new(SSLv23_client_method());
  if (n->context == NULL) {
    ERR_print_errors_fp(stderr);
    openssl_disconnect((Network *) n);
    return -1;
  }
  rc = SSL_CTX_set_options(n->context, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3);
  if (rc < 0) {
    ERR_print_errors_fp(stderr);
    openssl_disconnect((Network *) n);
    return -1;
  }
  n->handle = SSL_new(n->context);
  if (n->handle == NULL) {
    ERR_print_errors_fp(stderr);
    openssl_disconnect((Network *) n);
    return -1;
  }
  if (!SSL_set_fd(n->handle, n->socket)) {
    ERR_print_errors_fp(stderr);
    openssl_disconnect((Network *) n);
    return -1;
  }
  if (SSL_connect(n->handle) != 1) {
    ERR_print_errors_fp(stderr);
    openssl_disconnect((Network *) n);
    return -1;
  }
  return 0;
}
