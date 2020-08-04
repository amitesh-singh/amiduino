from kivy.app import App
from kivy.lang import Builder
from kivy.uix.boxlayout import BoxLayout


kv = """
<Test>:
    orientation: "vertical"
    RelativeLayout:
        GridLayout:
            pos: (self.parent.width-self.width)/2, (self.parent.height-self.height)/2
            size_hint: None, None
            size: 100, 100
            canvas:
                Color:
                    rgba: 1, 0, 0, 1
                Rectangle:
                    pos: self.pos
                    size: self.size

    Button:
        text: "Next"
"""

Builder.load_string(kv)


class Test(BoxLayout):
    def __init__(self, **kwargs):
        super(Test, self).__init__(**kwargs)


class TestApp(App):
    def build(self):
        return Test()

if __name__ == '__main__':
    TestApp().run()
