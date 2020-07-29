#!/usr/bin/python
from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout


class MyGrid(GridLayout):
    def __init__(self):
        super(MyGrid, self).__init__()
        self.cols = 2
        self.rows = 2
        self.add_widget(Button(text='Hello1'))
        self.add_widget(Button(text='Hello2'))
        self.add_widget(Button(text='Hello3'))
        self.add_widget(Button(text='Hello4'))


class MyApp(App):
    def build(self):
        return MyGrid()


if __name__ == '__main__':
    MyApp().run()




