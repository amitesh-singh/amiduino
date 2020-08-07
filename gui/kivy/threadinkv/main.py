import kivy
from kivy.uix.boxlayout import BoxLayout
from kivy.app import App
import time


#check kv file on how to call a function in a thread
class MyBoxLayout(BoxLayout):

    def counter_func(self):
        self.counter = 0
        for i in range(40):
            self.counter = self.counter + 1
        time.sleep(1)
        self.ids.txtinput.text += str(self.counter)

    def btnclick(self):
        self.counter_func()


class MyApp(App):
    def build(self):
        return MyBoxLayout()


MyApp().run()
