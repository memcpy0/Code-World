# 基础必备三件套
from django.shortcuts import HttpResponse, render, redirect
from .models import Publisher

def login(request):
    # 所有跟请求相关的, Django帮你封装到request里面
    # return HttpResponse(""Your request is accepted!"")
    if request.method == "POST":  # request.POST .GET必须是大写
        print(request.POST)
        if request.POST['Password'] == "123":
            return redirect("http://www.baidu.com")
        else:
            error_msg = "Failed, get away!"
            return render(request, "login.html", {"error_msg": error_msg}) # render本质上是打开文件, 返回字符串
    else:
        return render(request, "login.html")

def publisher_list(request):
    print("Coming --> publisher")

    ret = Publisher.objects.all()  # 取出所有的出版社数据
    # print(ret)
    # for i in ret:
    #     print(i.ownId, i.name)
    # return HttpResponse("ok")
    return render(request, "publisher.html", {"publisher_list":ret})