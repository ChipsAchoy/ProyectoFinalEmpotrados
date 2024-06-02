import socket

def read_file(filename):
    with open(filename, 'rb') as f:
        data = f.read()
    return data

def write_file(filename, data):
    with open(filename, 'wb') as f:
        f.write(data)

def main():
    host = '127.0.0.1'
    port = 8777
    input_filename = 'input.mp3'
    output_filename = 'output.mp3'

    data = read_file(input_filename)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))

    # Enviar datos al servidor en chunks de 1024 bytes
    chunk_size = 1024
    for i in range(0, len(data), chunk_size):
        end = min(i + chunk_size, len(data))
        chunk = data[i:end]
        s.sendall(chunk)
    print("Audio data sent successfully!")

    # Recibir datos procesados del servidor en chunks de 1024 bytes
    received_data = bytearray()
    while True:
        try:
            packet = s.recv(1024)
            if not packet:
                break
            received_data.extend(packet)
        except socket.error as e:
            print(f"Socket error: {e}")
            break

    print("Processed audio data received successfully!")
    write_file(output_filename, received_data)
    s.close()

if __name__ == "__main__":
    main()

