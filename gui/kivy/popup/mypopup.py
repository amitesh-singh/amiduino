import kivy
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.clock import Clock


class MyPopup:
    def dismiss_popup(self, dt):
        self.popup.dismiss()
        print("dismisspopup is called.")

    def show(self, title, msg):
        self.popup = Popup(title=title,
                      title_color=[1, 0, 0, 1],
                      content=Label(text=msg),
                      size_hint=(0.5, 0.5), size=(200, 200),
                      pos_hint={'right': 0.9, 'top': 1})
        
        Clock.schedule_once(self.dismiss_popup, 2)

        self.popup.open()
