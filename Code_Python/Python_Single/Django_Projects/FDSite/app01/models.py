from django.db import models

# Create your models here.

# ����һ���м�ģ�� �����������������ݿ� дSQL��� ִ��SQL���  �õ����


class Publisher(models.Model):
    ownId = models.IntegerField(primary_key=True)
    name = models.CharField(max_length=20)


# models.py  ���ݿ�
# 1. ��   -->  ��
# 2. ���� -->  ������
# 3. ���� -->  �ֶ�
#
# �﷨:
#     1. ȡ������
#     ����.objects.all()
#     ����.objects.filter(id=?) ��ȡ���
#     ����.objects.get(id=?)    �õ��� ��ȡһ�� û�оͱ���
