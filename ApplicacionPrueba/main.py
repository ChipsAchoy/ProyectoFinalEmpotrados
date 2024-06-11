from kivy.lang import Builder
from kivy.core.window import Window
from kivy.core.audio import SoundLoader
from kivy.uix.widget import Widget
from kivymd.uix.boxlayout import MDBoxLayout
from kivymd.app import MDApp
from kivymd.uix.label import MDLabel
from kivymd.uix.button import (MDButton,MDButtonText)
from kivymd.uix.dialog import (MDDialog, MDDialogIcon, MDDialogHeadlineText, MDDialogSupportingText,MDDialogButtonContainer)
from kivymd.uix.filemanager import MDFileManager
from ApiController import send_audio, receive_audio
from kivy.properties import BooleanProperty
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
        self.theme_cls.primary_palette = 'Black'
        return Builder.load_file('main.kv')

    def select_audio_file(self):
        self.file_manager.show('/')  # abre el administrador de archivos en la raíz del sistema

    def exit_manager(self, *args):
        self.file_manager.close()

    def select_path(self, path):
        self.selected_file = path
        self.root.ids.status_label.text = f'Selected file: {os.path.basename(path)}'
        self.root.ids.ListenO.disabled=False
        self.exit_manager()


    def process_audio(self):
        if not self.selected_file:
            self.root.ids.Save.disabled = True
            self.root.ids.ListenM.disabled=True
            self.root.ids.ListenO.disabled=True
        
            self.show_dialog('Error', 'No audio file selected.')
            return

        try:
            output_file = 'output.mp3'
            send_audio(self.selected_file)
            receive_audio(output_file)
            self.root.ids.status_label.text = 'Audio processed successfully!'
            self.root.ids.Save.disabled = False
            self.root.ids.ListenM.disabled=False
        
        except Exception as e:
            self.root.ids.Save.disabled = True
            self.root.ids.ListenM.disabled=True
        
            self.show_dialog('Error', str(e))

    def show_dialog(self, title, text):
        if not self.dialog:
            self.dialog = MDDialog(
                MDDialogIcon(
                icon="alert",
                ),
                MDDialogHeadlineText(
                text=title,
                ),
                MDDialogSupportingText(
                text=text,
                ),
                MDDialogButtonContainer(
                    Widget(),
                    MDButton(
                        MDButtonText(text="Close"),
                        on_release=lambda x: self.dialog.dismiss()
                    ),
                    spacing="8dp",
                )
            )
        self.dialog.open()
    def play_audio(filename):
        sound = SoundLoader.load(filename)
        self.root.ids.status_label.text = 'Reproducing audio!'
        if sound:
            sound.play()

if __name__ == '__main__':
    AudioProcessorApp().run()
