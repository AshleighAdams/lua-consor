
CFLAGS += -fPIC -Wall --std=c++11
LFLAGS += -shared

CFLAGS += `pkg-config --cflags consor lua5.2`
LIBS   += `pkg-config --libs consor lua5.2`

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
