import requests
import base64
import time

BASE_URL = 'http://localhost:5000/api'

def send_audio(file_path, max_retries=3):
    with open(file_path, 'rb') as audio_file:
        audio_data = base64.b64encode(audio_file.read()).decode('utf-8')
    
    for attempt in range(max_retries):
        try:
            response = requests.post(f'{BASE_URL}/enviar_audio', json={'audio': audio_data})
            response.raise_for_status()
            print('Audio sent successfully')
            return
        except requests.exceptions.RequestException as e:
            print(f'Attempt {attempt + 1} failed: {e}')
            if attempt < max_retries - 1:
                time.sleep(2)
            else:
                print('Failed to send audio after several attempts')
                raise

def receive_audio(output_file_path, max_retries=3):
    for attempt in range(max_retries):
        try:
            response = requests.get(f'{BASE_URL}/recibir_audio')
            response.raise_for_status()
            audio_data = response.json().get('audio')
            if audio_data:
                with open(output_file_path, 'wb') as audio_file:
                    audio_file.write(base64.b64decode(audio_data))
                print('Audio received and saved successfully')
                return
            else:
                print('No audio data found in the response')
                return
        except requests.exceptions.RequestException as e:
            print(f'Attempt {attempt + 1} failed: {e}')
            if attempt < max_retries - 1:
                time.sleep(2)
            else:
                print('Failed to receive audio after several attempts')
                raise

if __name__ == '__main__':
    input_audio_file = 'input.mp3'
    output_audio_file = 'output.mp3'
    
    send_audio(input_audio_file)
    receive_audio(output_audio_file)

