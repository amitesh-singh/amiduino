from sys import platform
import os
if platform == 'win32':
    os.environ['KIVY_GL_BACKEND'] = 'angle_sdl2'

import kivy
from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.button import Button
from kivy.uix.widget import Widget

i = 0


class MyApp(App):
    def build(self):
        f1 = FloatLayout()
        btn = Button(text='hello world',
                     size_hint=(0.3, 0.2),
                     pos=(300, 200))
        f1.add_widget(btn)
        btn.bind(on_press=self.pressed)
        return f1

    def pressed(self, instance):
        global i
        i += 1
        print('btn is pressed.', i, ' times')


if __name__ == '__main__':
    MyApp().run()