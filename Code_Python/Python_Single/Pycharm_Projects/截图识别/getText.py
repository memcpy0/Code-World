# -*- coding:utf-8 -*-
# -*- topic:      -*- 
# -*- arthor:张平   -*-
import win32con
import win32clipboard as w

class GetText:
    """
    把图像识别出的文字保存到剪切板
    """
    @staticmethod
    def getText():
        w.OpenClipboard()  # 打开剪切板
        d = w.GetClipboardData(win32con.CF_UNICODETEXT) # 获得剪切板的信息
        w.CloseClipboard() # 关闭剪切板
        return d

    @classmethod
    def setText(cls, aString):
        w.OpenClipboard()
        w.EmptyClipboard()
        w.SetClipboardData(win32con.CF_UNICODETEXT, aString)
        w.CloseClipboard()

if __name__ == "__main__":
    GetText.setText("管他")
    GetText.getText()

