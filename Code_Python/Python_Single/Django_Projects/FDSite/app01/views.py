# �����ر�������
from django.shortcuts import HttpResponse, render, redirect
from .models import Publisher

def login(request):
    # ���и�������ص�, Django�����װ��request����
    # return HttpResponse(""Your request is accepted!"")
    if request.method == "POST":  # request.POST .GET�����Ǵ�д
        print(request.POST)
        if request.POST['Password'] == "123":
            return redirect("http://www.baidu.com")
        else:
            error_msg = "Failed, get away!"
            return render(request, "login.html", {"error_msg": error_msg}) # render�������Ǵ��ļ�, �����ַ���
    else:
        return render(request, "login.html")

def publisher_list(request):
    print("Coming --> publisher")

    ret = Publisher.objects.all()  # ȡ�����еĳ���������
    # print(ret)
    # for i in ret:
    #     print(i.ownId, i.name)
    # return HttpResponse("ok")
    return render(request, "publisher.html", {"publisher_list":ret})