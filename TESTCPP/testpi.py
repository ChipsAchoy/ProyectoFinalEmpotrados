import socket

# Create a TCP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
server_address = ('192.168.0.25', 8666)
sock.connect(server_address)

try:
    # Send the string "get_inputs" to the server
    message = "get_inputs"
    sock.sendall(message.encode())

    # Receive a 4-character string from the server
    data = sock.recv(4).decode()
    print("Received:", data)

finally:
    # Close the socket
    sock.close()