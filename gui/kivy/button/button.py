import kivy

from kivy.app import App
from kivy.uix.button import Button


class MyApp(App):
    def build(self):
        self.btn = Button(text='hello world')
        self.btn.bind(on_press=self.pressed)

        return self.btn


    def pressed(self, instance):
        print('pressed')


if __name__ == '__main__':
    MyApp().run()


