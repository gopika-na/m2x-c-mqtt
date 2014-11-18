/* Paho-compatible Network implementation using OpenSSL */
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "MQTTLinux.h"

typedef struct OpenSSLNetwork {
  int socket;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);

  SSL *handle;
  SSL_CTX *context;
} OpenSSLNetwork;

void NewOpenSSLNetwork(OpenSSLNetwork*);
int ConnectOpenSSLNetwork(OpenSSLNetwork*, char*, int);
