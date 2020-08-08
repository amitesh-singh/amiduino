import kivy
from kivy.uix.label import Label
from kivy.uix.button import ButtonBehavior
from kivy.app import App


class LabelButton(ButtonBehavior, Label):
    pass


class MyApp(App):
    pass


MyApp().run()
