CXX = g++
CXXFLAGS = -Wall -Wextra -g

CLIENT_DIR = src/client
SERVER_DIR = src/server

all: client server

client: $(CLIENT_DIR)/client.cpp
	$(CXX) $(CXXFLAGS) -o client $(CLIENT_DIR)/client.cpp $(CLIENT_DIR)/client_main.cpp src/utils.cpp	 

server: $(SERVER_DIR)/server.cpp 
	$(CXX) $(CXXFLAGS) -o server $(SERVER_DIR)/server.cpp $(SERVER_DIR)/server_main.cpp src/utils.cpp 

clean:
	rm -rf client server
