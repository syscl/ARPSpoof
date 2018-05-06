# (c) 2018 syscl (aka Yating Zhou)
# The pre-processor options used by the cpp (man cpp for more).
# -Wall: enable all warning
CPPFLAGS  = -Wall -std=c++11

# The C++ program compiler.
CXX    = g++

# The command used to delete file.
RM     = rm -f


all:
	$(CXX) $(CPPFLAGS) -o request FakeARPPacket.cpp request.cpp
	$(CXX) $(CPPFLAGS) -o reply FakeARPPacket.cpp reply.cpp


clean:
	$(RM) request reply *.out *.o