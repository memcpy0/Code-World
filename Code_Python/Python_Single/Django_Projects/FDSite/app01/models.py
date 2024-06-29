from django.db import models

# Create your models here.

# 利用一个中间模块 帮助我们来连接数据库 写SQL语句 执行SQL语句  拿到结果


class Publisher(models.Model):
    ownId = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=20)


# models.py  数据库
# 1. 类   -->  表
# 2. 对象 -->  数据行
# 3. 属性 -->  字段
#
# 语法:
#     1. 取出数据
#     类名.objects.all()
#     类名.objects.filter(id=?) 获取多个
#     类名.objects.get(id=?)    用的少 获取一个 没有就报错
