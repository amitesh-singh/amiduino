import kivy
from kivy.app import App
from kivy.uix.popup import Popup
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from mypopup import MyPopup
from kivy.clock import Clock

global popup

class MyButton(BoxLayout):
    def dismiss_popup(self, dt):
        global popup
        popup.dismiss()

    def createpopup(self):
        global popup
        popup = Popup(title='Error',
                      title_color=[1, 0, 0, 1],
                      content=Label(text='Hello world'),
                      size_hint=(None, None), size=(100, 100),
                      )
        popup.open()
        Clock.schedule_once(self.dismiss_popup, 5)

    def createrightalignpopup(self):
        popup = Popup(title='Error',
                      title_color=[1, 0, 0, 1],
                      content=Label(text='unable to connect'),
                      size_hint=(0.5, 0.5), size=(200, 200),
                      pos_hint={'right': 0.9, 'top': 1})
        popup.open()

    def mypopup(self):
        MyPopup().show(title='error', msg='Unable to connct')


class MyApp(App):
    def build(self):
        return MyButton()


MyApp().run()
