import kivy
from kivy.uix.popup import Popup
from kivy.uix.label import Label


class MyPopup:
    def show(self, title, msg):
        popup = Popup(title=title,
                      title_color=[1, 0, 0, 1],
                      content=Label(text=msg),
                      size_hint=(0.5, 0.5), size=(200, 200),
                      pos_hint={'right': 0.9, 'top': 1})
        popup.open()
