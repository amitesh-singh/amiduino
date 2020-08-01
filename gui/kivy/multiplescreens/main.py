from sys import platform
import os
if platform == 'win32':
    os.environ['KIVY_GL_BACKEND'] = 'angle_sdl2'


import kivy
from kivy.app import App
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.uix.gridlayout import GridLayout
from kivy.properties import ObjectProperty


class ScreenSwitcher(ScreenManager):
    def __init__(self, **kwargs):
        super(ScreenSwitcher, self).__init__(**kwargs)
        self.add_widget(ScreenOne(name='sone'))
        self.add_widget(ScreenTwo(name='stwo'))


class ScreenOne(Screen):
    pass


class ScreenTwo(Screen):
    pass


class MainScreen(GridLayout):
    pass


class MyApp(App):
    def build(self):
        #set the title of the window
        self.title = 'Eload'
        return MainScreen()


if __name__ == '__main__':
    MyApp().run()