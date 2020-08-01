from sys import platform
import os
if platform == 'win32':
    os.environ['KIVY_GL_BACKEND'] = 'angle_sdl2'

import kivy
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.properties import ObjectProperty


class MyGrid(Widget):
    name = ObjectProperty(None)
    email = ObjectProperty(None)

    def btnclick(self):
        print("Name: ", self.name.text, " Email: ", self.email.text)


class MyApp(App):
    def build(self):
        return MyGrid()


if __name__ == '__main__':
    MyApp().run()
