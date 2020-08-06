import kivy
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.app import App
import threading
import time


class MyBox(BoxLayout):
    def counter_func(self):
        counter = 0
        for i in range(30000):
            counter = counter + 1
        time.sleep(4)
        self.ids.txtinput.text += str(counter)

    def docalinthread(self):
        self.t = threading.Thread(target=self.counter_func)
        self.t.start()

    def docalinmainloop(self):
        self.counter_func()


class MyApp(App):
    def build(self):
        return MyBox()


if __name__ == '__main__':
    MyApp().run()
