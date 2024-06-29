# -*- coding:utf-8 -*-
"""
This script is used to give my program a GUI with tkinter
"""

"""
Implement a GUI for viewing and updating class instances stored in a shelve;
the shelve lives on the machine this script runs on, as 1 or more local files;
"""
import tkinter
from tkinter import *
from tkinter.messagebox import showerror
import shelve
# 存储文件shelve名称
shelvename = 'password-classes-shelve'
# 密码管理字段
fieldnames = ('name', 'account_name', 'password')

def makeWidgets():
    global entries
    window = tkinter.Tk()
    window.title('Password Shelve')
    form = tkinter.Frame(window)
    form.pack()
    entries = {}
    for (ix, label) in enumerate(('key',) + fieldnames): #
        lab = tkinter.Label(form, text=label)
        ent = tkinter.Entry(form)
        lab.grid(row=ix, column=0)
        ent.grid(row=ix, column=1)
        entries[label] = ent #
    tkinter.Button(window, text="Get",  command=getRecord).pack(side=LEFT)
    tkinter.Button(window, text="Update", command=updateRecord).pack(side=LEFT)
    tkinter.Button(window, text="Quit",   command=window.quit).pack(side=RIGHT)
    return window

def getRecord():
    key = entries['key'].get()
    try:
        record = db[key]                      # fetch by key, show in GUI
    except:
        showerror(title='Error', message='No such key!')
    else:
        for field in fieldnames:
            entries[field].delete(0, END)
            entries[field].insert(0, repr(getattr(record, field)))

def updateRecord():
    key = entries['key'].get()
    if key in db:
        record = db[key]                      # update existing record
    else:
        from password_manager import PasswordManager           # make/store new one for key
        record = PasswordManager(name='?', account_name='?', password='?')    # eval: strings must be quoted
    for field in fieldnames:
        setattr(record, field, eval(entries[field].get()))
    db[key] = record

db = shelve.open(shelvename)
window = makeWidgets()
window.mainloop()
db.close() # back here after quit or window close
