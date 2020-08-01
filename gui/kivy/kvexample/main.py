import os

os.environ['KIVY_GL_BACKEND'] = 'angle_sdl2'


import kivy
from kivy.app import App
from kivy.uix.label import Label
#MyGrid will use it.
from kivy.uix.widget import Widget


class MyGrid(Widget):
    pass


class MyApp(App):
    def build(self):
        return MyGrid()


if __name__ == '__main__':
    MyApp().run()
