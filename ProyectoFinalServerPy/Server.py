import socket
from flask import Flask, request, jsonify
import base64
import os

app = Flask(__name__)

def read_file(filename):
    with open(filename, 'rb') as f:
        return f.read()

def write_file(filename, data):
    with open(filename, 'wb') as f:
        f.write(data)

@app.route('/api/enviar_audio', methods=['POST'])
def enviar_audio():
    input_filename = 'input.mp3'
    try:
        data = request.json
        audio_data = data.get('audio')
        
        if not audio_data:
            return jsonify({'error': 'No audio data found'}), 400
        
        audio_bytes = base64.b64decode(audio_data)
        
        write_file(input_filename, audio_bytes)
        
        return jsonify({'message': 'Audio received successfully'}), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/recibir_audio', methods=['GET'])
def recibir_audio():
    host = '127.0.0.1'
    port = 8777
    input_filename = 'input.mp3'
    output_filename = 'output.mp3'
    
    try:
        data = read_file(input_filename)

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((host, port))

            chunk_size = 1024
            for i in range(0, len(data), chunk_size):
                end = min(i + chunk_size, len(data))
                chunk = data[i:end]
                s.sendall(chunk)
            print("Audio data sent successfully!")

            received_data = bytearray()
            while True:
                packet = s.recv(1024)
                if not packet:
                    break
                received_data.extend(packet)
            print("Processed audio data received successfully!")

        write_file(output_filename, received_data)
        
        if not os.path.exists(output_filename):
            return jsonify({'error': 'Audio file not found'}), 404

        audio_data = read_file(output_filename)
        encoded_audio = base64.b64encode(audio_data).decode('utf-8')

        return jsonify({'audio': encoded_audio}), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

