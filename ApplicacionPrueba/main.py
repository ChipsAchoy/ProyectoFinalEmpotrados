from kivy.lang import Builder
from kivy.core.window import Window
from kivy.uix.boxlayout import BoxLayout
from kivymd.app import MDApp
from kivymd.uix.label import MDLabel
from kivymd.uix.button import MDRaisedButton
from kivymd.uix.dialog import MDDialog
from kivymd.uix.filemanager import MDFileManager
from ApiController import send_audio, receive_audio
import os

class AudioProcessorApp(MDApp):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.file_manager = MDFileManager(
            exit_manager=self.exit_manager,
            select_path=self.select_path,
            preview=True
        )
        self.selected_file = None
        self.dialog = None

    def build(self):
        self.theme_cls.primary_palette = 'Blue'
        return Builder.load_file('main.kv')

    def select_audio_file(self):
        self.file_manager.show('/')  # abre el administrador de archivos en la raíz del sistema

    def exit_manager(self, *args):
        self.file_manager.close()

    def select_path(self, path):
        self.selected_file = path
        self.root.ids.status_label.text = f'Selected file: {os.path.basename(path)}'
        self.exit_manager()

    def process_audio(self):
        if not self.selected_file:
            self.show_dialog('Error', 'No audio file selected.')
            return

        try:
            output_file = 'output.mp3'
            send_audio(self.selected_file)
            receive_audio(output_file)
            self.root.ids.status_label.text = 'Audio processed successfully!'
        except Exception as e:
            self.show_dialog('Error', str(e))

    def show_dialog(self, title, text):
        if not self.dialog:
            self.dialog = MDDialog(
                title=title,
                text=text,
                size_hint=(0.8, 1),
                buttons=[
                    MDRaisedButton(
                        text="Close",
                        on_release=lambda x: self.dialog.dismiss()
                    ),
                ],
            )
        self.dialog.title = title
        self.dialog.text = text
        self.dialog.open()

if __name__ == '__main__':
    AudioProcessorApp().run()
