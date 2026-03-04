#include <systemc.h> 
#include <iostream> 
#include <sys/socket.h> 
#include <sys/un.h>
#include <unistd.h>
// Required for non-blocking
#include <fcntl.h> 
#include <cstring>

#define SOCKET_PATH "/tmp/systemc_socket"

SC_MODULE(SystemCGPIO)
{
public:
	SC_HAS_PROCESS (SystemCGPIO);
	SystemCGPIO(sc_core::sc_module_name name): sc_module(name) 
	{
		setup_server();
		SC_THREAD(socket_listener);
	}

	void setup_server() 
	{
		socket_fd = socket (AF_UNIX, SOCK_STREAM, 0);
		if (socket_fd < 0) 
		{ 
			perror("Socket failed"); 
			return;
		}
		// Set the server socket to non-blocking
		fcntl(socket_fd, F_SETFL, O_NONBLOCK);
		
		struct sockaddr_un addr;
		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) -1 );
		unlink(SOCKET_PATH);
		
		if (bind(socket_fd, (struct sockaddr *)&addr, sizeof (addr)) < 0) 
		{
			perror("Bind failed");
		}
		listen (socket_fd, 5);
		std::cout << "SystemC Server listening on" << SOCKET_PATH << std::endl;
	}
	
	void socket_listener() 
	{
		while (true) 
		{
			// 1. Non-blocking accept for a new connection
			int client_fd = accept(socket_fd, NULL, NULL);
			
			if (client_fd >= 0)
			{
				// Set the client socket to non-blocking as well
				fcntl(client_fd, F_SETFL, O_NONBLOCK);
				std::cout << "QEMU connected at " << sc_time_stamp() << std::endl;
				// 2. INNER LOOP: Keep this connection alive
				while (true)
				{
					uint32_t value;
					ssize_t bytes_read = read (client_fd, &value, sizeof (value));
					
					if (bytes_read > 0 && 0 != value)
					{
						// Success: process the data
						std::cout << "data received from QEMU: 0x" << std::hex << value << std::endl;
						// ONLY send back when something is received 
						uint32_t response = 0xa5a5a5a5;
						write(client_fd, &response, sizeof(response)); 
						wait(1, SC_MS);
					}						
					else if (bytes_read == 0)
					{
						// Client closed connection
						std::cout << "QEMU disconnected at " << sc_time_stamp() << std::endl;
						break;
					}
					else if (errno != EAGAIN && errno != EWOULDBLOCK)
					{
						// Actual error occurred
						perror("Read error");
						break;
					}
				}
				close(client_fd);
			}
			wait(1,SC_MS);
		}
	}
	
	// Yield to allow SystemC kernel to process other events wait(1, SC MS);
	~SystemCGPIO() 
	{
		if (socket_fd = -1) 
			close (socket_fd); 
		unlink(SOCKET_PATH);
	}
private:
	int socket_fd = -1;
};

int sc_main(int argc, char* argv[])
{
	SystemCGPIO gpio("SystemCGPIO");
	sc_core::sc_start(); // Runs indefinitely until sc_stop()
	return 0;
}

// command to run this code
// g++ -o systemc_socket systemc-socket.cpp -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib -lsystemc -lm  
				
