BUILD_DIR=../build
EXAMPLES_BUILD_DIR=$(BUILD_DIR)/examples
M2X_LIB=$(BUILD_DIR)/m2x.a
BINS=$(addprefix $(EXAMPLES_BUILD_DIR)/,m2x_status)
OBJS=$(addsuffix .o, $(BINS))

PAHO_PATH=../thirdparty/paho
PAHO_BUILD_DIR=$(BUILD_DIR)/paho
PAHO_PACKET_PATH=$(PAHO_PATH)/MQTTPacket/src
PAHO_CLIENT_PATH=$(PAHO_PATH)/MQTTClient-C/src
PAHO_HOST_PATH=$(PAHO_CLIENT_PATH)/linux

CC=gcc
CFLAGS=-O3 -g -Wall -Wextra -std=gnu99 -Wno-unused-parameter -Wno-unused-variable -Wno-comment -I.. -I $(PAHO_PACKET_PATH) -I $(PAHO_HOST_PATH) -I $(PAHO_BUILD_DIR)
LDFLAGS=-lcurl

all: $(BINS)

$(EXAMPLES_BUILD_DIR)/%: $(EXAMPLES_BUILD_DIR)/%.o $(M2X_LIB)
	$(CC) -o $@ $(LDFLAGS) $^

$(EXAMPLES_BUILD_DIR)/%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

$(BINS): | $(EXAMPLES_BUILD_DIR)

$(OBJS): | $(EXAMPLES_BUILD_DIR)

$(EXAMPLES_BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(EXAMPLES_BUILD_DIR)

.PHONY: all clean
