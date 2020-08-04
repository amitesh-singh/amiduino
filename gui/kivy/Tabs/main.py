import kivy
from kivy.app import App
from kivy.uix.tabbedpanel import TabbedPanel


class Tabs(TabbedPanel):
    pass


class MyApp(App):
    def build(self):
        return Tabs()


if __name__ == '__main__':
    MyApp().run()