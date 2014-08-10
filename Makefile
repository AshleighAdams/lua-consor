PREFIX = $(DEST_DIR)/usr
LUA_VERSION = 5.2

CFLAGS += -fPIC -Wall --std=c++11
LFLAGS += -shared

CFLAGS += `pkg-config --cflags consor lua$(LUA_VERSION)`
LIBS   += `pkg-config --libs consor lua$(LUA_VERSION)`

#CFLAGS += -O3
CFLAGS += -g3

.PHONY: consor/core.so consor.o
all: consor/core.so

consor.o: src/main.cpp
	$(CXX) -std=c++11 -o consor.o -c $(CFLAGS) $<

consor/core.so: consor.o
	mkdir -p consor
	$(CXX) -std=c++11 -o $@ $(LFLAGS) $(LIBS) $<

clean:
	$(RM) *.o

install:
	cp "consor.lua" "$(PREFIX)/share/lua/$(LUA_VERSION)/consor.lua"
	rm -rf "$(PREFIX)/lib/lua/$(LUA_VERSION)/consor"; cp -r consor "$(PREFIX)/lib/lua/$(LUA_VERSION)/consor"
