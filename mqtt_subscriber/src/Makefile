BIN:=mqtt_subscriber
OBJS := arguments_parse.o main.o mqtt_sub.o topics_load.o logger.o
LDFLAGS:=-lubox -lmosquitto -largp  -luci -lsqlite3

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC)  -o $@ $^ $(LDFLAGS)

clean:
	$(RM) -f $(BIN) $(OBJS)