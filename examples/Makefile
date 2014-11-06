CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c89 -I..
LDFLAGS=-lcurl

BUILD_DIR=../build
EXAMPLES_BUILD_DIR=$(BUILD_DIR)/examples
M2X_LIB=$(BUILD_DIR)/m2x.a
BINS=$(addprefix $(EXAMPLES_BUILD_DIR)/,m2x_status)
OBJS=$(addsuffix .o, $(BINS))

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
