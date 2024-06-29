# -*- coding:utf-8 -*-

"""
this program is used to manage my own accounts and passwords of different apps or websites
with it, I can add, find, delete, update the record in the CMD
and it can save and load(sort) my password database with file.

自动进行：
 * 读取数据：load_data 运行后， 所做的第一件事就是加载密码数据库，将保存的数据读到内存中
 * 数据排序：将加载的数据按照app_name首字母进行排序整理
 * 保存数据：添加数据、删除数据、更新数据时都会重新保存数据，退出也会进行保存
调用进行：
 * 增加密码：只支持逐个增加密码，不支持批量增加
 * 删除密码：只支持通过app名称逐个删除，不支持批量操作
 * 查找密码：支持用户通过app名称进行查找需要的应用密码
 * 更新密码：支持用户通过app名称修改应用密码
 * 显示密码：显示当前系统中保存的所有app账户和密码
 * 退出系统：支持用户退出
"""
import datetime
import shelve
from collections import namedtuple
PasswordList = namedtuple('PasswordList', ['app_name', 'account_name', 'password'])

out_string = """
* -{} 增加新密码
* -{} 删除旧密码
* -{} 查找旧密码
* -{} 更新旧密码
* -{} 显示全密码
* -{} 退出管理器
"""

class PasswordManager:
    def __init__(self, app_name, account_name, password):
        # self.__app_name = name
        # self.__account_name = account_name
        self.__pwdata = PasswordList(app_name, account_name, password)

    def __str__(self):
        return "[%s] => %s : %s" % (self.__pwdata.app_name, self.__pwdata.account_name, self.__pwdata.password)

    # @property
    # def password(self):
    #     return self.__password
    # @password.setter
    # def password(self, input_ps):
    #     self.__password = input_ps

    def sort_data(self):
        pass
    def load_data(self):
        self.__pwDataBase = shelve.open("Password-Shelve")
        pass

    def add_new_password(self):
        pass

    @staticmethod
    def run():
        print("*" * 60)
        date = datetime.datetime.now()
        print("今天是%s, 欢迎使用密码管理器!" % date.strftime('%Y-%m-%d,%H:%M:%S'))
        print("*" * 60, end='')
        print(out_string.format('a', 'd', 'f', 'u', 'p', 'q'), end='')
        print("*" * 60, end='')

        ch = input('\n请输入要进行的操作:')
        while (ch != 'q'):
            if ch == 'a':
                pass
            elif ch == 'f':
                pass
            elif ch == 'd':
                pass
            elif ch == 'u':
                pass
            elif ch == 'p':
                pass
            elif ch == 'q':
                break
            else:
                ch = input("您的输入有误，请重新输入:")


if __name__ == "__main__":
    ncre = PasswordManager("NCRE", "张平", "!wdNCREmm123")
    csdn = PasswordManager("CSDN", "myRealization", "Zx~MscksHv")
    qq = PasswordManager("QQ", "天七", "!@#$567890")
    github = PasswordManager("GITHUB", "myRealization", "!wdGitHubmm123")

    # for app in [ncre, csdn]:
    #     print(app.password)
    PasswordManager.run()
    # psdb = shelve.open("password-classes-shelve")
    #
    # psdb['ncre'] = ncre
    # psdb['csdn'] = csdn
    # psdb['qq'] = qq
    # psdb['github'] = github

    # psdb.close()
