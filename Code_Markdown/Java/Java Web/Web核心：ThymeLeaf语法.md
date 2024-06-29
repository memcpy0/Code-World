
thymeleaf语法：（@后面是填写路径的，$后面是填写变量数据的）
th:href="@{/css/global.css}" 去static下面用thymeleaf去找静态资源
th:src="${map.user.headerUrl} 引用Model的值
通过Thymeleaf遍历集合：
th:each="map:${discussPosts}" 每次循环得到集合中的一个对象，这里是Map，我们给它命名为map。

th:class="| string ${usernameMsg!=null?'isValid':''} |" `| |` 里面拼接为字符串
`th:text="${map.user.headerUrl}` => map.get("user") -> User -> user.getHeaderUrl()
`th:utext="${}"`
`th:if="${map.post.type==1}` 判断是否显示这个标签对

`th:text="${#dates.format(map.post.createTime,'yyyy-MM-dd HH:mm:ss')}"` `#dates.format` 调用函数进行日期格式化
`th:each="i:${#numbers.sequence(page.from,page.to)}"`

th:action="@{/register}" 
input输入框中，th:value="${user!=null?user.username:''}"