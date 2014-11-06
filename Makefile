AR=ar
CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c89
LDFLAGS=

BUILD_DIR=build
M2X_LIB=$(BUILD_DIR)/m2x.a
OBJS=$(addprefix $(BUILD_DIR)/,m2x.o)

PAHO_CFLAGS=-Wno-unused-variable -Wno-comment -std=c89
PAHO_PATH=thirdparty/paho
PAHO_BUILD_DIR=$(BUILD_DIR)/paho
PAHO_CLIENT_PATH=$(PAHO_PATH)/MQTTClient-C/src
PAHO_HOST_PATH=$(PAHO_CLIENT_PATH)/linux
PAHO_HOST_NAME=MQTTLinux
PAHO_PACKET_PATH=$(PAHO_PATH)/MQTTPacket/src
PAHO_PACKET_SOURCES=$(wildcard $(PAHO_PACKET_PATH)/*.c)
PAHO_PACKET_OBJS=$(patsubst $(PAHO_PACKET_PATH)/%.c, $(PAHO_BUILD_DIR)/%.o, $(PAHO_PACKET_SOURCES))

$(M2X_LIB): $(OBJS) $(BUILD_DIR)/parson.o $(PAHO_PACKET_OBJS) $(PAHO_BUILD_DIR)/MQTTClient.o $(PAHO_BUILD_DIR)/$(PAHO_HOST_NAME).o
	$(AR) -rcs $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

$(OBJS): | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

$(PAHO_BUILD_DIR)/%.o: $(PAHO_PACKET_PATH)/%.c
	$(CC) -o $@ $(PAHO_CFLAGS) -c $<

$(PAHO_BUILD_DIR)/MQTTClient.o: $(PAHO_BUILD_DIR) $(PAHO_BUILD_DIR)/MQTTClient.c $(PAHO_BUILD_DIR)/MQTTClient.h
	$(CC) -o $@ $(PAHO_CFLAGS) -c $(PAHO_BUILD_DIR)/MQTTClient.c -I $(PAHO_PACKET_PATH) -I $(PAHO_HOST_PATH)

$(PAHO_BUILD_DIR)/MQTTClient.c: $(PAHO_BUILD_DIR) $(PAHO_CLIENT_PATH)/MQTTClient.c
	cp $(PAHO_CLIENT_PATH)/MQTTClient.c $@

$(PAHO_BUILD_DIR)/MQTTClient.h: $(PAHO_BUILD_DIR) $(PAHO_CLIENT_PATH)/MQTTClient.h
	sed -e 's/"" \/\/.*$$/"MQTTLinux.h"/g' $(PAHO_CLIENT_PATH)/MQTTClient.h > $@

$(PAHO_BUILD_DIR)/$(PAHO_HOST_NAME).o: $(PAHO_BUILD_DIR) $(PAHO_HOST_PATH)/$(PAHO_HOST_NAME).c
	$(CC) -o $@ $(PAHO_CFLAGS) -c $(PAHO_HOST_PATH)/$(PAHO_HOST_NAME).c

$(PAHO_PACKET_OBJS): | $(PAHO_BUILD_DIR)

$(PAHO_BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/parson.o: thirdparty/parson/parson.c thirdparty/parson/parson.h
	$(CC) -o $@ $(CFLAGS) -c $<

examples:
	cd examples; make

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean examples
