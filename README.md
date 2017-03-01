C (MQTT) M2X API Client
=========================

[AT&T M2X](http://m2x.att.com) is a cloud-based fully managed time-series data storage service for network connected machine-to-machine (M2M) devices and the Internet of Things (IoT). 

The [AT&T M2X API](https://m2x.att.com/developer/documentation/overview) provides all the needed operations and methods to connect your devices to AT&T's M2X service. This library aims to provide a simple wrapper to interact with the AT&T M2X API for [C](http://www.open-std.org/jtc1/sc22/wg14/) using the [MQTT](http://mqtt.org) protocol. Refer to the [Glossary of Terms](https://m2x.att.com/developer/documentation/glossary) to understand the nomenclature used throughout this documentation.

Getting Started
===============
1. Signup for an M2X Account: https://m2x.att.com/signup
2. Obtain your *Master Key* from the Master Keys tab of your Account Settings: https://m2x.att.com/account
3. Create your first Device and copy its *Device ID*: https://m2x.att.com/devices
4. Review the M2X API Documentation: https://m2x.att.com/developer/documentation/overview

If you have questions about any M2X specific terms, please consult the M2X glossary: https://m2x.att.com/developer/documentation/glossary


Setup
=============

This library is written using [C99](http://en.wikipedia.org/wiki/ANSI_C) standard. Any decent C compilers, including gcc and clang, can be used to compile the library. The only external dependencies needed are [paho](http://git.eclipse.org/c/paho/org.eclipse.paho.mqtt.embedded-c.git/) for MQTT communications, and [frozen](https://github.com/cesanta/frozen) for JSON parsing.

The library uses no dynamic memory allocation except for the JSON parser part in order to achieve maximum compatability with embedded boards.

To bulid the library and examples, use the following steps:

```
$ git clone https://github.com/attm2x/m2x-c-mqtt.git
$ cd m2x-c-mqtt
$ make setup
$ make
$ make examples
```

The library can be found at `build/m2x.a`, while examples can be found at `build/examples`.

API
===

## Initialization

To use this library, you need to create an M2X context object first:

```
const char *key = "<Your M2X key>";
m2x_context ctx;
m2x_open(key, &ctx);
```

Notice there's no need to create the context object on the heap, dynamically memory allocation is not a requirement here.

## SSL support

By default, this library implements SSL support via [OpenSSL](https://www.openssl.org/). However, you can also disable the SSL support completely:

```
$ make SSL=false
$ make examples SSL=false
```

It is also possible to build with SSL support, but turn it off for the current request. Just set `use_ssl` in `m2x_context` will do the trick.

For SSL connections, the default port used is `8883`, while non-SSL connections use `1883` port.

## Calling API functions

All API functions will have the following type of signature:

```
m2x_response m2x_some_api_function(m2x_context *ctx, const char *arg1, const char *arg2, ...);
```

Notice that this library wraps the MQTT communications completely: you don't need to do the MQTT connection manually, the library will take care of that for you. All you need to do is call the correct functions.

Depending the exact function in use, different number of arguments may be present. For example, below is a function used to list stream values of a device:

```
m2x_response m2x_device_list_stream_values(m2x_context *ctx, const char *id, const char *name, const char *query);
```

It requires the context object, device ID, stream name and a query string. The query string is the same you would normally see in an HTTP request, like the following:

```
max=100&limit=10
```

We will talk about `m2x_response` in the next section.

Notice that there is no need to connect to MQTT broker before calling the API functions. The library will check if a connection is available first, and connect if necessary. By default, the connection will also be closed once the API request is fulfilled, however, there's another mode that will keep an MQTT connection open. We will talk about it later.

## Response object

All API functions will return an `m2x_response` object. This object contains the status code, raw response as well as JSON-parsed response. The type of this object is as follows:

```
typedef struct m2x_response {
  int status;

  char *raw;
  int raw_length;

  void *data;
} m2x_response;
```

* `status` contains the same status code as those you can see in an HTTP request
* `raw` contains the raw reply from the MQTT server(not necessarily NULL-terminated)
* `data` contains the reply from our JSON parser

Though we use [frozen](https://github.com/cesanta/frozen) as the library to parse JSON, we shall see later that it is even possible to plug-in another JSON parsing library of your choice. This is the reason that `data` has type `void *`: you can store anything you want here and cast it to the actual type later.

A handful of helpers are created for better usage of this object:

```
int m2x_is_success(const m2x_response *response);
int m2x_is_client_error(const m2x_response *response);
int m2x_is_server_error(const m2x_response *response);
int m2x_is_error(const m2x_response *response);
void m2x_release_response(m2x_context *ctx, m2x_response *response);
```

What's worth mentioning here, is that `m2x_release_response` only frees the memory allocated in `data`. `raw` is pointing directly to the memory buffer in MQTT connection, which will be available until the next API call.

## Reading data

By default, we use [frozen](https://github.com/cesanta/frozen) to parse the JSON data. Below is an example of how you can read the values via frozen:

```
// Simplified from the list_devices example
m2x_response response;
struct json_token *arr = NULL, *tok = NULL;

response = m2x_device_list(&ctx, "");
if (m2x_is_success(&response)) {
  arr = (struct json_token *) response.data;
  tok = find_json_token(arr, "body.total");
  if (tok) {
    printf("Total values: ");
    fwrite(tok->ptr, sizeof(char), tok->len, stdout);
    printf("\n");
  }
}
m2x_release_response(&ctx, &response);
```

Below is a sample reply that the code piece above is handling:

```
{
  "id": "3FAXPM6QDTBTPNFQ1LMWSQAABL0RK107",
  "status": 200,
  "body": {
    "total": 1,
    "pages": 1,
    "limit": 10,
    "current_page": 1,
    "devices": [
      {
        "id": "059c21d01876a31c8a85db65a8dad9c8",
        "url": "http://api-m2x.att.com/v2/devices/059c21d01876a31c8a85db65a8dad9c8",
        "name": "API Batch",
        "status": "enabled",
        "serial": "ABC1234",
        "tags": [],
        "location": {
          "name": "room",
          "latitude": 10,
          "longitude": 12,
          "elevation": null,
          "timestamp": "2013-12-31T02:23:40.000Z"
        },
        "visibility": "public",
        "description": "Created in API",
        "created": "2013-12-31T01:32:00.000Z",
        "updated": "2014-05-14T23:35:53.485Z",
        "key": "fea432ba14239b79aaead32ca93565f8"
      }
    ]
  }
}
```

Options
=======

## Keepalive

The default behaviour of this library is to create a new connection each time an API function is called, however, this can be changed by changing `keepalive` field in the context object to 1:

```
m2x_context ctx;
m2x_open(M2X_KEY, &ctx);
ctx.keepalive = 1;
```

In this case, the library will maintain a connection to MQTT, and reuse it each time.

NOTE: due to the protocol of MQTT, if you are using the keepalive option, you need to call `m2x_mqtt_yield` at certain periods(at least once per 10 seconds by default) to maintain the connection. In other words, you need to maintain such a loop:

```
m2x_context ctx;
while (true) {
  // perform some other action
  m2x_mqtt_yield(&ctx)
}
```

Otherwise, the connection will be lost at certain time, and errors will occur.

## JSON

By default, we use [frozen](https://github.com/cesanta/frozen), but you can also plug-in your favourite JSON parser to handle the requests. All you need to do is implement this function:

```
typedef struct m2x_json_result {
  int status_code;
  void *data;
} m2x_json_result;

typedef enum m2x_json_status {
  M2X_JSON_OK = 0,
  M2X_JSON_INVALID = -1,
  M2X_JSON_ID_NOT_MATCH = -2,
} m2x_json_status;

typedef m2x_json_status (*m2x_json_parser) (const char *, int,
                                            const char *, int,
                                            m2x_json_result *);
```

The function takes the response JSON string(not NULL-terminated), and the designated message ID(also not NULL-terminated), you need to test the following things:

1. Is the JSON string valid?
2. Is the message ID obtained in the JSON string the same as the provided JSON string?

If the answers to both questions here are yes, we have a valid response here, you can fill the `m2x_json_result` with the status code and the returned data. Notice the `data` returned here will become the coresponding `data` field in `m2x_response` object.

After you implement this function, you can set it to the `json_parser` field in `m2x_context` object, the library will then use your specified JSON parser to parse the result!

By default, we expect the `data` pointer is pointed to a piece of memory allocated using `malloc`, and that we can use `free` to release the memory. In fact, this is also the only place in the library that uses dynamic memory allocation. However, if your JSON parser is working in another way (for example, if you use static memory, or use multiple malloc), you can implement the function used to release memory:

```
typedef void (*m2x_json_releaser) (void *);
```

Setting this function to `json_releaser` field in `m2x_context`, then the library will use your new memory releaser to do the job.

## Preparing data for writer functions

You can also send data to M2X service using this library. The following function can be used to update stream values:


```
m2x_response m2x_device_update_stream(m2x_context *ctx, const char *id, const char *name, const char *data);
```

`id` and `name` are just plain strings representing device ID and stream name respectively. The `data` field here is actually a JSON string containing the data to send. We use plain `const char *` type for maximum flexibility. You can you any JSON serializer that you like to do the job.

Notice the packed `frozen` library also has a JSON serializer, you can refer to [our example](https://github.com/attm2x/m2x-c-mqtt/blob/master/examples/create_device.c#L33) and the [README](https://github.com/cesanta/frozen/blob/master/README.md) of frozen for how this is working.

Last but not least, you can always use `sprintf` to prepare the JSON string if it is not very complicated.

License
=======

The M2X C (MQTT) Client is available under the MIT license. See the LICENSE file for more information.
