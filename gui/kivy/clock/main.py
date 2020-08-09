import kivy
from kivy.uix.button import Button
from kivy.app import App
from kivy.clock import Clock
from kivy.uix.boxlayout import BoxLayout


class MyBoxLayout(BoxLayout):
    # dt means delta-time
    def callback(self, dt):
        print("my callback is called")

    # call my_callback in 5 seconds
    def clickme(self):
        Clock.schedule_once(self.callback, 2)

    def clickme2(self):
        Clock.schedule_interval(self.callback, 5)


class MyApp(App):
    def build(self):
        return MyBoxLayout()


MyApp().run()

