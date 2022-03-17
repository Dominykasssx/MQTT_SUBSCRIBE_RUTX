BIN:=mqtt_subscriber
OBJS := arguments_parse.o main.o mqtt_sub.o uci_load.o logger.o events.o mail.o
LDFLAGS:=-lubox -lmosquitto -largp  -luci -lsqlite3 -lcurl

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC)  -o $@ $^ $(LDFLAGS)

clean:
	$(RM) -f $(BIN) $(OBJS)