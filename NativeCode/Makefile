SRCS=AudioPluginUtil.cpp \
VBAP.cpp\
VBAP_Spatializer.cpp \
Plugin_SpatializerReverb.cpp \
hrtftable.cpp
OBJS=$(SRCS:.cpp=.o)
OUTPUT=AudioPluginDemo.so
CXXFLAGS=-I/user/lcoal/include -O2 -fPIC -std=c++11 
LDFLAGS=-shared -rdynamic -fPIC -L/usr/local/lib -lgsl -lgslcblas -lm
CXX=g++

all: $(OUTPUT)
clean:
	rm -f $(OUTPUT) $(OBJS)

$(OUTPUT): $(OBJS)
	$(CXX) -o $(OUTPUT) $(OBJS) $(LDFLAGS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<
