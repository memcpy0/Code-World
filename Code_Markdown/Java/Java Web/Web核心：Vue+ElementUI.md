> * 能够使用VUE中常用指令和插值表达式
> * 能够使用VUE生命周期函数 `mounted`
> * 能够进行简单的 `Element` 页面修改

---
# 1. VUE
## 1.1  概述
接下来我们学习一款前端的框架，就是VUE。==Vue是一套前端框架，免除原生JavaScript中的DOM操作，简化书写==。

之前也学习过后端框架Mybatis，Mybatis是用来简化JDBC代码编写的；而Vue是前端的框架，是用来简化JavaScript代码编写的。我们做了一个综合性的案例，里面进行了大量的DOM操作，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142034663.png)
学习Vue后，这部分代码我们就不需要再写了。那么Vue是如何简化DOM书写呢？——==Vue基于MVVM(Model-View-ViewModel)思想，实现了数据的双向绑定，将编程的关注点放在数据上==。之前我们是将关注点放在了DOM操作上；而要了解 `MVVM` 思想，必须先聊聊 `MVC` 思想，如下图就是 `MVC` 思想图解
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142036753.png)
C就是咱们JS代码（或者Servlet），M就是数据，而V是页面上展示的内容（HTML+CSS+Ajax；或者JSP），如下图是我们之前写的代码
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142038554.png)
`MVC` 思想是没法进行双向绑定的。==双向绑定是指当数据模型数据发生变化时，页面展示的会随之发生变化，而如果表单数据发生变化，绑定的模型数据也随之发生变化==。接下来我们聊聊 `MVVM` 思想，如下图是三个组件图解
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142049833.png)
图中的 `Model` 就是我们的数据，`View` 是视图，也就是页面标签，用户可以通过浏览器看到的内容；**`Model` 和 `View` 是通过 `ViewModel` 对象进行双向绑定的，而 `ViewModel` 对象是Vue提供的**。接下来让大家看一下双向绑定的效果，下图是提前准备的代码，**输入框绑定了 `username` 模型数据**，而**在页面上也使用 `{{}}` 绑定了 `username` 模型数据**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142059648.png)
通过浏览器打开该页面可以看到如下页面：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142101579.png)
当我们在输入框中输入内容，而输入框后面随之实时的展示我们输入的内容，这就是双向绑定的效果。
## 1.2 快速入门
没有提示的解决方法：File==>Setting……==>Plugins==>Marketplace，搜索vue，Install，ok==>File==>ReStart IDE。注意：网上下的 `vue.js` 和 `vue.min.js` 是不一样的

Vue使用起来是比较简单的，总共分为如下三步：
(1) **新建HTML页面，引入 Vue.js文件**：
```html
<script src="js/vue.js"></script>
```
(2) **在JS代码区域，创建Vue核心对象，进行数据绑定**：
```js
new Vue({
   el: "#app",
   data() {
	   return {
		   username: ""
	   }
   }
});
```
创建Vue对象时，需要传递一个JS对象，而该对象中需要如下属性：
   * `el` ：**用来指定哪些标签受Vue管理**。 该属性取值 `#app` 中的 `app` 需是受管理的标签的 `id` 属性值
   * `data` ：用来定义**数据模型**
   * `methods` ：用来定义函数。这个我们在后面就会用到

(3) **编写视图**（下面两个地方都是视图，**都看得到**，底层隐藏的是模型；第一个视图发生变化，影响了模型，模型也变化，于是第二个视图也发生了变化）：
```html
<div id="app">
   <input name="username" v-model="username" >
   {{username}}
</div>
```
**`{{}}` 是Vue中定义的 `插值表达式` ，在里面写数据模型，到时候会将该模型的数据值展示在这个位置**。

整体代码如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
    <input v-model="username">
    <!--插值表达式-->
    {{username}}
</div>
<script src="js/vue.js"></script>
<script>
    //1. 创建Vue核心对象
    new Vue({
        el:"#app",
        /*data: function () {
            return {
                username:""
            }
        } 这样写比较麻烦 */
        data() {  // data() 是 ECMAScript 6 版本的新的写法
            return {
                username:""
            }
        } 
    });
</script>
</body>
</html>
```
## 1.3 Vue指令
**指令**：HTML标签上带有 `v-` 前缀的特殊属性，不同指令具有不同含义。例如：`v-if, v-for` ......常用的指令有：
| **指令**  | **作用**                                            |
| --------- | --------------------------------------------------- |
| `v-bind`    | 为HTML标签的属性绑定模型数据，如设置 `href` , css样式等      |
| `v-model`   | **在表单元素上创建双向数据绑定**                        |
| `v-on `     | **为HTML标签绑定事**件                                  |
| `v-if`      | 条件性的渲染某元素，判定为 `true` 时渲染,否则不渲染     |
| `v-else`    |                                                     |
| `v-else-if` |                                                     |
| `v-show`    | 根据条件展示某元素，区别在于切换的是 `display` 属性的值 |
| `v-for`     | 列表渲染，遍历容器的元素或者对象的属性              |

接下来我们挨个学习这些指令。
### 1.3.1 `v-bind, v-model` 指令 
**v-bind**：==该指令可以给HTML自带标签的原有属性绑定模型数据。这样模型数据发生变化，标签属性值也随之发生变化==。例如：
```html
<a v-bind:href="url">百度一下</a>
```
上面的 `v-bind:"`  可以简化写成 `:`  ，如下：
```html
<!--
v-bind 可以省略
-->
<a :href="url">百度一下</a>
```
**v-model** ：该指令可以**给表单项标签绑定模型数据**。这样就能实现双向绑定效果。例如：
```html
<input name="username" v-model="username">
```
代码演示：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
    <a v-bind:href="url">点击一下</a>
    <a :href="url">点击一下</a>
    <input v-model="url">
</div>

<script src="js/vue.js"></script>
<script>
    //1. 创建Vue核心对象
    new Vue({
        el:"#app",
        data(){
            return {
                username:"",
                url:"https://www.baidu.com"
            }
        }
    });
</script>
</body>
</html>
```
通过浏览器打开上面页面，并且使用检查查看超链接的路径，该路径会根据输入框输入的路径变化而变化，这是因为超链接和输入框绑定的是同一个模型数据
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142140016.png)

### 1.3.2 `v-on` 指令
 我们在页面定义一个按钮，并给该按钮使用 `v-on` 指令绑定单击事件，HTML代码如下
```html
<input type="button" value="一个按钮" v-on:click="show()">
```
而使用 `v-on` 时还可以使用简化的写法，将 `v-on:` 替换成 `@` ，HTML代码如下：
```html
<input type="button" value="一个按钮" @click="show()">
```
上面代码绑定的 `show()` 需要在Vue对象中的 `methods` 属性中定义出来：
```js
new Vue({
    el: "#app",
    methods: {
        show() {
            alert("我被点了");
        }
    }
});
```
==注意==：`v-on:` 后面的事件名称是**之前原生事件属性名去掉 `on`** 。例如： 
* 单击事件：事件属性名是 `onclick` ，而在Vue中使用是 `v-on:click`
* 失去焦点事件：事件属性名是 `onblur` ，而在Vue中使用时 `v-on:blur`

整体页面代码如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
    <input type="button" value="一个按钮" v-on:click="show()"><br>
    <input type="button" value="一个按钮" @click="show()">
</div>
<script src="js/vue.js"></script>
<script>
    //1. 创建Vue核心对象
    new Vue({
        el:"#app",
        data() {
            return {
                username:"",
            }
        },
        methods:{
            show() {
                alert("我被点了...");
            }
        }
    });
</script>
</body>
</html>
```
### 1.3.3 条件判断指令 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142146519.png)
接下来通过代码演示一下。在 Vue中定义一个 `count` 的数据模型，如下
```js
//1. 创建Vue核心对象
new Vue({
    el:"#app",
    data(){
        return {
            count:3
        }
    }
});
```
现在要实现，当 `count` 模型的数据是3时，在页面上展示 `div1` 内容；当 `count` 模型的数据是4时，在页面上展示 `div2` 内容；`count` 模型数据是其他值时，在页面上展示 `div3` 。**这里为了动态改变模型数据 `count` 的值，再定义一个输入框绑定 `count` 模型数据**。HTML代码如下：
```html
<div id="app">
    <div v-if="count == 3">div1</div>
    <div v-else-if="count == 4">div2</div>
    <div v-else>div3</div>
    <hr>
    <input v-model="count">
</div>
```
整体页面代码如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
    <div v-if="count == 3">div1</div>
    <div v-else-if="count == 4">div2</div>
    <div v-else>div3</div>
    <hr>
    <input v-model="count">
</div>

<script src="js/vue.js"></script>
<script>
    //1. 创建Vue核心对象
    new Vue({
        el:"#app",
        data(){
            return {
                count : 3
            }
        }
    });
</script>
</body>
</html>
```

通过浏览器打开页面并在输入框输入不同的值，效果如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142150312.png)
然后我们在看看 `v-show` 指令的效果，如果模型数据 `count ` 的值是3时，展示 `div v-show` 内容，否则不展示。HTML页面代码如下：
```html
<div v-show="count == 3">div v-show</div>
<br>
<input v-model="count">
```
浏览器打开效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142154778.png)

通过上面的演示，发现 `v-show` 和 `v-if` 效果一样，那它们到底有什么区别呢？我们根据浏览器的检查功能查看源代码：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302142154475.png)

通过上图可以看出 **`v-show` 不展示的原理是给对应的标签添加 `display` CSS属性，并将该属性值设置为 `none`** ，这样就达到了隐藏的效果。而 **`v-if` 指令是条件不满足时根本就不会渲染**。
### 1.3.4 `v-for` 指令  
这个指令看到名字就知道是用来遍历的，该指令使用的格式如下（注意：**需要循环哪个标签，`v-for` 指令就写在哪个标签上**）：
```html
<标签 v-for="变量名 in 集合模型数据">
    {{变量名}}
</标签>
```
如果在页面需要使用到**集合模型数据的索引**，就需要使用如下格式：
```html
<标签 v-for="(变量名,索引变量) in 集合模型数据">
    <!--索引变量是从0开始，所以要表示序号的话，需要手动的加1-->
   {{索引变量 + 1}} {{变量名}}
</标签>
```
代码演示：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
    <div v-for="addr in addrs">
        {{addr}} <br>
    </div>

    <hr>
    <div v-for="(addr,i) in addrs">
        {{i+1}}--{{addr}} <br>
    </div>
</div>

<script src="js/vue.js"></script>
<script> 
    //1. 创建Vue核心对象
    new Vue({
        el:"#app",
        data(){
            return {
                addrs:["北京","上海","西安"]
            }
        }
    });
</script>
</body>
</html>
```
通过浏览器打开效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150001456.png)
## 1.4 生命周期 
生命周期的八个阶段：==每触发一个生命周期事件，会自动执行一个生命周期方法，这些生命周期方法也被称为钩子方法==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150002976.png)
下图是Vue官网提供的、从创建Vue到效果Vue对象的整个过程、及各个阶段对应的钩子函数：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150003659.png)
看到上面的图，大家无需过多的关注这张图。这些钩子方法我们只关注 `mounted` 就行了——`mounted` 表示==挂载完成，Vue初始化成功，HTML页面渲染成功==。而==以后我们会在该方法中发送异步请求，加载数据==。
## 1.5 案例
### 1.5.1 需求
使用Vue简化我们在AJAX学完后做的品牌列表数据查询和添加功能
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150005307.png)
此案例只是使用Vue对前端代码进行优化，后端代码无需修改。
### 1.5.2 查询所有功能
(1) **在 brand.html 页面引入Vue的JS文件**：
```html
<script src="js/vue.js"></script>
```
(2) **创建Vue对象**：在Vue对象中定义模型数据；**在钩子函数中发送异步请求**，并将响应的数据赋值给数据模型。
```js
new Vue({
   el: "#app",
   data() {
	   return{
		   brands:[]
	   }
   },
   mounted() {
	   // 页面加载完成后，发送异步请求，查询数据
	   var _this = this;
	   axios({
		   method:"get",
		   url:"http://localhost:8080/brand-demo/selectAllServlet"
	   }).then(function (resp) {
		   _this.brands = resp.data;
	   })
   }
})
```
**修改视图**
* 定义 `<div id="app"></div>` ，指定该 `div` 标签受Vue管理
* 将 `body` 标签中所有的内容拷贝到上面 `div` 标签中
* 删除表格的多余数据行，只留下一个
* 在表格中的数据行上使用 `v-for` 指令遍历
```html
<tr v-for="(brand,i) in brands" align="center">
	 <td>{{i + 1}}</td>
	 <td>{{brand.brandName}}</td>
	 <td>{{brand.companyName}}</td>
	 <td>{{brand.ordered}}</td>
	 <td>{{brand.description}}</td>
	 <td>{{brand.statusStr}}</td>
	 <td><a href="#">修改</a> <a href="#">删除</a></td>
</tr>
```

整体页面代码如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
    <a href="addBrand.html"><input type="button" value="新增"></a><br>
    <hr>
    <table id="brandTable" border="1" cellspacing="0" width="100%">
        <tr>
            <th>序号</th>
            <th>品牌名称</th>
            <th>企业名称</th>
            <th>排序</th>
            <th>品牌介绍</th>
            <th>状态</th>
            <th>操作</th>
        </tr>
        <!--
            使用v-for遍历tr
        -->
        <tr v-for="(brand,i) in brands" align="center">
            <td>{{i + 1}}</td>
            <td>{{brand.brandName}}</td>
            <td>{{brand.companyName}}</td>
            <td>{{brand.ordered}}</td>
            <td>{{brand.description}}</td>
            <td>{{brand.statusStr}}</td>
            <td><a href="#">修改</a> <a href="#">删除</a></td>
        </tr>
    </table>
</div>
<script src="js/axios-0.18.0.js"></script>
<script src="js/vue.js"></script>
<script>
    new Vue({
        el : "#app",
        data() {
            return {
                brands : []
            }
        },
        mounted() {
            // 页面加载完成后，发送异步请求，查询数据
            var _this = this;
            axios({
                method:"get",
                url:"http://localhost:8080/brand-demo/selectAllServlet"
            }).then(function (resp) {
                _this.brands = resp.data;
            })
        }
    })
</script>
</body>
</html>
```
应该换成 `String jsonString = JSON.toJSONString(brands, SerializerFeature.IgnoreErrorGetter);` 。`JSON` 封装对象，获取属性值的方法就是调用属性值的 `get` 方法，我们定义 `statusStr` 和它的 `get` 方法，封装 `statusStr` 的时候会自动调用它的 `get` 方法。
### 1.5.3 添加功能
页面操作效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150058409.png)
整体流程如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150059035.png)
> 注意：**前端代码的关键点在于使用 `v-model` 指令给标签项绑定模型数据**，==利用双向绑定特性，在发送异步请求时提交数据==。

(1) **在 `addBrand.html` 页面引入Vue的JS文件**
```html
<script src="js/vue.js"></script>
```
(2) **创建Vue对象**
* 在Vue对象中定义模型数据 `brand`
* **定义一个 `submitForm()` 函数，用于给 `提交` 按钮提供绑定的函数**
* 在 `submitForm()` 函数中发送AJAX请求，并将模型数据 `brand` 作为参数进行传递
```js
new Vue({
   el: "#app",
   data() {
	   return {
		   brand: {}
	   }
   },
   methods: {
	   submitForm() {
		   // 发送ajax请求，添加
		   var _this = this;
		   axios({
			   method: "post",
			   url: "http://localhost:8080/brand-demo/addServlet",
			   data: _this.brand
		   }).then(function (resp) {
			   // 判断响应数据是否为 success
			   if(resp.data == "success"){
				   location.href = "http://localhost:8080/brand-demo/brand.html";
			   }
		   })
	   }
   }
})
```
(3) **修改视图**
* 定义 `<div id="app"></div>` ，指定该 `div` 标签受Vue管理
* 将 `body` 标签中所有的内容拷贝作为上面 `div` 标签中
* 给每一个表单项标签绑定模型数据。最后这些数据要被封装到 `brand` 对象中
```html
<div id="app">
	<h3>添加品牌</h3>
	<form action="" method="post">
	 品牌名称：<input id="brandName" v-model="brand.brandName" name="brandName"><br>
	 企业名称：<input id="companyName" v-model="brand.companyName" name="companyName"><br>
	 排序：<input id="ordered" v-model="brand.ordered" name="ordered"><br>
	 描述信息：<textarea rows="5" cols="20" id="description" v-model="brand.description" name="description"></textarea><br>
	 状态：
	 <input type="radio" name="status" v-model="brand.status" value="0">禁用
	 <input type="radio" name="status" v-model="brand.status" value="1">启用<br>

	 <input type="button" id="btn" @click="submitForm" value="提交">
	</form>
</div>
```

整体页面代码如下：
```html
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <title>添加品牌</title>
</head>
<body>
<div id="app">
    <h3>添加品牌</h3>
    <form action="" method="post">
        品牌名称：<input id="brandName" v-model="brand.brandName" name="brandName"><br>
        企业名称：<input id="companyName" v-model="brand.companyName" name="companyName"><br>
        排序：<input id="ordered" v-model="brand.ordered" name="ordered"><br>
        描述信息：<textarea rows="5" cols="20" id="description" v-model="brand.description" name="description"></textarea><br>
        状态：
        <input type="radio" name="status" v-model="brand.status" value="0">禁用
        <input type="radio" name="status" v-model="brand.status" value="1">启用<br>

        <input type="button" id="btn" @click="submitForm" value="提交">
    </form>
</div>
<script src="js/axios-0.18.0.js"></script>
<script src="js/vue.js"></script>
<script>
    new Vue({
        el: "#app",
        data(){
            return {
                brand:{}
            }
        },
        methods:{
            submitForm(){
                // 发送ajax请求，添加
                var _this = this; // 这里的this对象是Vue对象
                axios({
	                // axios里面的this对象是Window对象
                    method:"post",
                    url:"http://localhost:8080/brand-demo/addServlet",
                    data:_this.brand
                }).then(function (resp) {
                    // 判断响应数据是否为 success
                    if(resp.data == "success"){
                        location.href = "http://localhost:8080/brand-demo/brand.html";
                    }
                })
            }
        }
    })
</script>
</body>
</html>
```
通过上面的优化，前端代码确实简化了不少。但页面依旧是不怎么好看，那么接下来我们学习 `Element` ，它可以美化页面。

---
# 2. Element
Element是饿了么公司前端开发团队提供的**一套基于Vue的网站组件库**，用于**快速构建网页**。

Element提供了很多组件（组成网页的部件）供我们使用。例如超链接、按钮、图片、表格等等。如下图左边的是我们编写页面看到的按钮，右边的是Element提供的页面效果，效果一目了然。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150110075.png)
==我们学习Element，其实就是学习怎么从官网拷贝组件到我们自己的页面并进行修改==，官网网址是https://element.eleme.cn/#/zh-CN。进入官网直接点击 `组件` ，页面如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150110567.png)
## 2.1 快速入门
(1) 将 `element-ui` 文件夹直接拷贝到项目的 `webapp` 下。目录结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150111493.png)
(2) 创建页面，并在页面引入Element的CSS、JS文件和Vue.js：
```html
<script src="vue.js"></script>
<script src="element-ui/lib/index.js"></script>
<link rel="stylesheet" href="element-ui/lib/theme-chalk/index.css">
```
(3) 创建Vue核心对象——==Element是基于Vue的，所以使用Element时必须要创建Vue对象==：
```html
<script>
   new Vue({
	   el:"#app"
   })
</script>
```
(4) 官网复制Element组件代码：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150115848.png)
在左菜单栏找到 `Button 按钮` ，然后找到自己喜欢的按钮样式，点击 `显示代码` ，在下面就会展示出对应的代码，将这些代码拷贝到我们自己的页面即可。

整体页面代码如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
    <el-row>
     	<el-button>默认按钮</el-button>
        <el-button type="primary">主要按钮</el-button>
        <el-button type="success">成功按钮</el-button>
        <el-button type="info">信息按钮</el-button>
        <el-button type="warning">警告按钮</el-button>
        <el-button type="danger">删除</el-button>
    </el-row>
    <el-row>
        <el-button plain>朴素按钮</el-button>
        <el-button type="primary" plain>主要按钮</el-button>
        <el-button type="success" plain>成功按钮</el-button>
        <el-button type="info" plain>信息按钮</el-button>
        <el-button type="warning" plain>警告按钮</el-button>
        <el-button type="danger" plain>危险按钮</el-button>
    </el-row>

    <el-row>
        <el-button round>圆角按钮</el-button>
        <el-button type="primary" round>主要按钮</el-button>
        <el-button type="success" round>成功按钮</el-button>
        <el-button type="info" round>信息按钮</el-button>
        <el-button type="warning" round>警告按钮</el-button>
        <el-button type="danger" round>危险按钮</el-button>
    </el-row>

    <el-row>
        <el-button icon="el-icon-search" circle></el-button>
        <el-button type="primary" icon="el-icon-edit" circle></el-button>
        <el-button type="success" icon="el-icon-check" circle></el-button>
        <el-button type="info" icon="el-icon-message" circle></el-button>
        <el-button type="warning" icon="el-icon-star-off" circle></el-button>
        <el-button type="danger" icon="el-icon-delete" circle></el-button>
    </el-row>
</div>

<script src="js/vue.js"></script>
<script src="element-ui/lib/index.js"></script>
<link rel="stylesheet" href="element-ui/lib/theme-chalk/index.css">
<script>
    new Vue({
        el:"#app"
    })
</script>

</body>
</html>
```
## 2.2 Element布局
Element提供了两种布局方式，分别是：
* `Layout` 布局
* `Container` 布局容器
### 2.2.1 `Layout` 布局
通过基础的 24 分栏，迅速简便地创建布局。也就是默认将一行分为24栏，根据页面要求给每一列设置所占的栏数——下图第一行有24栏、1个格子，第二行有2个格子，每个格子12栏，第三行3x8，第四行4x6，第五行6x4（和bootstrap栅格系统类似）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150116710.png)
在左菜单栏找到 `Layout 布局` ，然后找到自己喜欢的按钮样式，点击 `显示代码` ，在下面就会展示出对应的代码，显示出的代码中有样式，有HTML标签。将样式拷贝我们自己页面的 `head` 标签内，将HTML标签拷贝到  `<div id="app"></div>` 标签内。

整体页面代码如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>

    <style>
        .el-row {
            margin-bottom: 20px;
        }
        .el-col {
            border-radius: 4px;
        }
        .bg-purple-dark {
            background: #99a9bf;
        }
        .bg-purple {
            background: #d3dce6;
        }
        .bg-purple-light {
            background: #e5e9f2;
        }
        .grid-content {
            border-radius: 4px;
            min-height: 36px;
        }
        .row-bg {
            padding: 10px 0;
            background-color: #f9fafc;
        }
    </style>
</head>
<body>
<div id="app">
    <el-row>
        <el-col :span="24"><div class="grid-content bg-purple-dark"></div></el-col>
    </el-row>
    <el-row>
        <el-col :span="12"><div class="grid-content bg-purple"></div></el-col>
        <el-col :span="12"><div class="grid-content bg-purple-light"></div></el-col>
    </el-row>
    <el-row>
        <el-col :span="8"><div class="grid-content bg-purple"></div></el-col>
        <el-col :span="8"><div class="grid-content bg-purple-light"></div></el-col>
        <el-col :span="8"><div class="grid-content bg-purple"></div></el-col>
    </el-row>
    <el-row>
        <el-col :span="6"><div class="grid-content bg-purple"></div></el-col>
        <el-col :span="6"><div class="grid-content bg-purple-light"></div></el-col>
        <el-col :span="6"><div class="grid-content bg-purple"></div></el-col>
        <el-col :span="6"><div class="grid-content bg-purple-light"></div></el-col>
    </el-row>
    <el-row>
        <el-col :span="4"><div class="grid-content bg-purple"></div></el-col>
        <el-col :span="4"><div class="grid-content bg-purple-light"></div></el-col>
        <el-col :span="4"><div class="grid-content bg-purple"></div></el-col>
        <el-col :span="4"><div class="grid-content bg-purple-light"></div></el-col>
        <el-col :span="4"><div class="grid-content bg-purple"></div></el-col>
        <el-col :span="4"><div class="grid-content bg-purple-light"></div></el-col>
    </el-row>
</div>
<script src="js/vue.js"></script>
<script src="element-ui/lib/index.js"></script>
<link rel="stylesheet" href="element-ui/lib/theme-chalk/index.css">

<script>
    new Vue({
        el:"#app"
    })
</script>
</body>
</html>
```
现在需要添加一行，要求该行显示8个格子，通过计算每个格子占3栏，具体的HTML代码如下：
```html
<!--
添加一行，8个格子  24/8 = 3
-->
<el-row>
    <el-col :span="3"><div class="grid-content bg-purple"></div></el-col>
    <el-col :span="3"><div class="grid-content bg-purple-light"></div></el-col>
    <el-col :span="3"><div class="grid-content bg-purple"></div></el-col>
    <el-col :span="3"><div class="grid-content bg-purple-light"></div></el-col>
    <el-col :span="3"><div class="grid-content bg-purple"></div></el-col>
    <el-col :span="3"><div class="grid-content bg-purple-light"></div></el-col>
    <el-col :span="3"><div class="grid-content bg-purple"></div></el-col>
    <el-col :span="3"><div class="grid-content bg-purple-light"></div></el-col>
</el-row>
```
### 2.2.2 `Container` 布局容器
用于布局的容器组件，方便快速搭建页面的基本结构。如下图就是布局容器效果。如下图是官网提供的 `Container` 布局容器实例：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150118362.png)

该效果代码中包含了样式、页面标签、模型数据。**将里面的样式 `<style>` 拷贝到我们自己页面的 `head` 标签中**；将HTML标签拷贝到 `<div id="app"></div>` 标签中，再**将数据模型拷贝到Vue对象的 `data()` 中**。整体页面代码如下：
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>

    <style>
        .el-header {
            background-color: #B3C0D1;
            color: #333;
            line-height: 60px;
        }

        .el-aside {
            color: #333;
        }
    </style>
</head>
<body>
<div id="app">
    <el-container style="height: 500px; border: 1px solid #eee">
        <el-aside width="200px" style="background-color: rgb(238, 241, 246)">
            <el-menu :default-openeds="['1', '3']">
                <el-submenu index="1">
                    <template slot="title"><i class="el-icon-message"></i>导航一</template>
                    <el-menu-item-group>
                        <template slot="title">分组一</template>
                        <el-menu-item index="1-1">选项1</el-menu-item>
                        <el-menu-item index="1-2">选项2</el-menu-item>
                    </el-menu-item-group>
                    <el-menu-item-group title="分组2">
                        <el-menu-item index="1-3">选项3</el-menu-item>
                    </el-menu-item-group>
                    <el-submenu index="1-4">
                        <template slot="title">选项4</template>
                        <el-menu-item index="1-4-1">选项4-1</el-menu-item>
                    </el-submenu>
                </el-submenu>
                <el-submenu index="2">
                    <template slot="title"><i class="el-icon-menu"></i>导航二</template>
                    <el-submenu index="2-1">
                        <template slot="title">选项1</template>
                        <el-menu-item index="2-1-1">选项1-1</el-menu-item>
                    </el-submenu>
                </el-submenu>
                <el-submenu index="3">
                    <template slot="title"><i class="el-icon-setting"></i>导航三</template>
                    <el-menu-item-group>
                        <template slot="title">分组一</template>
                        <el-menu-item index="3-1">选项1</el-menu-item>
                        <el-menu-item index="3-2">选项2</el-menu-item>
                    </el-menu-item-group>
                    <el-menu-item-group title="分组2">
                        <el-menu-item index="3-3">选项3</el-menu-item>
                    </el-menu-item-group>
                    <el-submenu index="3-4">
                        <template slot="title">选项4</template>
                        <el-menu-item index="3-4-1">选项4-1</el-menu-item>
                    </el-submenu>
                </el-submenu>
            </el-menu>
        </el-aside>

        <el-container>
            <el-header style="text-align: right; font-size: 12px">
                <el-dropdown>
                    <i class="el-icon-setting" style="margin-right: 15px"></i>
                    <el-dropdown-menu slot="dropdown">
                        <el-dropdown-item>查看</el-dropdown-item>
                        <el-dropdown-item>新增</el-dropdown-item>
                        <el-dropdown-item>删除</el-dropdown-item>
                    </el-dropdown-menu>
                </el-dropdown>
                <span>王小虎</span>
            </el-header>

            <el-main>
                <el-table :data="tableData">
                    <el-table-column prop="date" label="日期" width="140">
                    </el-table-column>
                    <el-table-column prop="name" label="姓名" width="120">
                    </el-table-column>
                    <el-table-column prop="address" label="地址">
                    </el-table-column>
                </el-table>
            </el-main>
        </el-container>
    </el-container>
</div>
<script src="js/vue.js"></script>
<script src="element-ui/lib/index.js"></script>
<link rel="stylesheet" href="element-ui/lib/theme-chalk/index.css">

<script>
    new Vue({
        el:"#app",
        data() {
            const item = {
                date: '2016-05-02',
                name: '王小虎',
                address: '上海市普陀区金沙江路 1518 弄'
            };
            return {
                tableData: Array(20).fill(item)
            }
        }
    })
</script>
</body>
</html>
```
## 2.3 案例
其他的组件我们通过完成一个页面来学习。我们要完成如下页面效果
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150120897.png)
要完成该页面，我们需要先对这个页面进行分析，看页面由哪儿几部分组成，然后到官网进行拷贝并修改。页面总共有如下组成部分：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150128374.png)
还有一个是当我们点击 `新增` 按钮，会在页面正中间弹出一个对话框，如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150129440.png)
### 2.3.1 准备基本页面
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div id="app">
	
</div>

<script src="js/vue.js"></script>
<script src="element-ui/lib/index.js"></script>
<link rel="stylesheet" href="element-ui/lib/theme-chalk/index.css">

<script>
    new Vue({
        el: "#app"
    })
</script>
</body>
</html>
```
### 2.3.2 完成表格展示
使用Element整体的思路就是 ==拷贝 + 修改==。
#### 2.3.2.1 拷贝
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150134219.png)
在左菜单栏找到 `Table 表格`并点击，右边主体就会定位到表格这一块，找到我们需要的表格效果（如上图），点击 `显示代码` 就可以看到这个表格的代码了。将HTML标签拷贝到 `<div id="app"></div>` 中，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150224206.png)
将CSS样式拷贝到我们页面的 `head` 标签中，如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150224416.png)
将方法和模型数据拷贝到Vue对象指定的位置
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150225396.png)
拷贝完成后，通过浏览器打开可以看到表格的效果： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150225236.png)
表格效果出来了，但是显示的表头和数据并不是我们想要的，所以接下来就需要对页面代码进行修改了。
#### 2.3.2.2 修改
1.**修改表头和数据**。下面是对表格代码进行分析的图解。根据下图说明修改自己的列数和列名：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150230629.png)
修改完页面后，还需要对绑定的模型数据进行修改，下图是对模型数据进行分析的图解
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150231350.png)

2.**给表格添加操作列**。从之前的表格拷贝一列出来并对其进行修改。按钮是从官网的 `Button 按钮` 组件中拷贝并修改的：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150232452.png)
3.**给表格添加复选框列和标号列**。给表格添加复选框和标号列，效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150233007.png)
此效果也是从Element官网进行拷贝，先找到对应的表格效果，然后将其对应代码拷贝到我们的代码中，如下是复选框列官网效果图和代码：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150233830.png)
这里需要注意在 `<el-table>` 标签上有一个事件 `@selection-change="handleSelectionChange"` ，这里绑定的函数也需要从官网拷贝到我们自己的页面代码中，函数代码如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150233763.png)
从该函数中又发现还需要一个模型数据 `multipleSelection ` ，所以还需要定义出该模型数据。标号列也用同样的方式进行拷贝并修改。
### 2.3.3 完成搜索表单展示
在Element官网找到横排的表单效果，然后拷贝代码并进行修改：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150234395.png)
点击上面的 `显示代码` 后，就会展示出对应的代码，下面是对这部分代码进行分析的图解
![img|530x700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150235695.png)
然后根据我们要的效果修改代码。
### 2.3.4 完成批量删除和新增按钮展示
从Element官网找具有着色效果的按钮，并将代码拷贝到我们自己的页面上：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150236138.png)
### 2.3.5 完成对话框展示
在Element官网找对话框，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150237655.png)
下面对官网提供的代码进行分析：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150237876.png)
上图分析出来的模型数据需要在Vue对象中进行定义。
### 2.3.6 完成分页条展示
在Element官网找到 `Pagination 分页` ，在页面主体部分找到我们需要的效果，如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150237866.png)
点击 `显示代码` ，找到 `完整功能` 对应的代码，接下来对该代码进行分析
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150238178.png)
上面代码属性说明：
* `page-size` ：每页显示的条目数
* `page-sizes` ： 每页显示个数选择器的选项设置。`:page-sizes="[100,200,300,400]"`  对应的页面效果如下：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150239688.png)
* `currentPage` ：当前页码。我们点击那个页码，此属性值就是几。
* `total` ：总记录数。用来设置总的数据条目数，该属性设置后（从数据库中查出），Element会自动计算出需分多少页并给我们展示对应的页码。

事件说明：
* `size-change` ：`pageSize` 改变时会触发。也就是当我们改变了每页显示的条目数后，该事件会触发。
* `current-change` ：`currentPage` 改变时会触发。也就是当我们点击了其他的页码后，该事件会触发。

### 2.3.7 完整页面代码
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
    <style>
        .el-table .warning-row {
            background: oldlace;
        }
        .el-table .success-row {
            background: #f0f9eb;
        }
    </style>
</head>
<body>
<div id="app">
    <!--搜索表单-->
    <el-form :inline="true" :model="brand" class="demo-form-inline">
        <el-form-item label="当前状态">
            <el-select v-model="brand.status" placeholder="当前状态">
                <el-option label="启用" value="1"></el-option>
                <el-option label="禁用" value="0"></el-option>
            </el-select>
        </el-form-item>

        <el-form-item label="企业名称">
            <el-input v-model="brand.companyName" placeholder="企业名称"></el-input>
        </el-form-item>

        <el-form-item label="品牌名称">
            <el-input v-model="brand.brandName" placeholder="品牌名称"></el-input>
        </el-form-item>

        <el-form-item>
            <el-button type="primary" @click="onSubmit">查询</el-button>
        </el-form-item>
    </el-form>

    <!--按钮-->
    <el-row>
        <el-button type="danger" plain>批量删除</el-button>
        <el-button type="primary" plain @click="dialogVisible = true">新增</el-button>
    </el-row>
    
    <!--添加数据对话框表单-->
    <el-dialog
            title="编辑品牌"
            :visible.sync="dialogVisible"
            width="30%">
        <el-form ref="form" :model="brand" label-width="80px">
            <el-form-item label="品牌名称">
                <el-input v-model="brand.brandName"></el-input>
            </el-form-item>

            <el-form-item label="企业名称">
                <el-input v-model="brand.companyName"></el-input>
            </el-form-item>

            <el-form-item label="排序">
                <el-input v-model="brand.ordered"></el-input>
            </el-form-item>

            <el-form-item label="备注">
                <el-input type="textarea" v-model="brand.description"></el-input>
            </el-form-item>

            <el-form-item label="状态">
                <el-switch v-model="brand.status"
                           active-value="1"
                           inactive-value="0"
                ></el-switch>
            </el-form-item>
            <el-form-item>
                <el-button type="primary" @click="addBrand">提交</el-button>
                <el-button @click="dialogVisible = false">取消</el-button>
            </el-form-item>
        </el-form>
    </el-dialog>

    <!--表格-->
    <template>
        <el-table
                :data="tableData"
                style="width: 100%"
                :row-class-name="tableRowClassName"
                @selection-change="handleSelectionChange">
            <el-table-column
                    type="selection"
                    width="55">
            </el-table-column>
            <el-table-column
                    type="index"
                    width="50">
            </el-table-column>
            <el-table-column
                    prop="brandName"
                    label="品牌名称"
                    align="center">
            </el-table-column>
            <el-table-column
                    prop="companyName"
                    label="企业名称"
                    align="center">
            </el-table-column>
            <el-table-column
                    prop="ordered"
                    align="center"
                    label="排序">
            </el-table-column>
            <el-table-column
                    prop="status"
                    align="center"
                    label="当前状态">
            </el-table-column>
            <el-table-column
                    align="center"
                    label="操作">
                <el-row>
                    <el-button type="primary">修改</el-button>
                    <el-button type="danger">删除</el-button>
                </el-row>
            </el-table-column>

        </el-table>
    </template>

    <!--分页工具条-->
    <el-pagination
            @size-change="handleSizeChange"
            @current-change="handleCurrentChange"
            :current-page="currentPage"
            :page-sizes="[5, 10, 15, 20]"
            :page-size="5"
            layout="total, sizes, prev, pager, next, jumper"
            :total="400">
    </el-pagination>

</div>
<script src="js/vue.js"></script>
<script src="element-ui/lib/index.js"></script>
<link rel="stylesheet" href="element-ui/lib/theme-chalk/index.css">
<script>
    new Vue({
        el: "#app",
        methods: {
            tableRowClassName({row, rowIndex}) {
                if (rowIndex === 1) {
                    return 'warning-row';
                } else if (rowIndex === 3) {
                    return 'success-row';
                }
                return '';
            },
            // 复选框选中后执行的方法
            handleSelectionChange(val) {
                this.multipleSelection = val;

                console.log(this.multipleSelection)
            },
            // 查询方法
            onSubmit() {
                console.log(this.brand);
            },
            // 添加数据
            addBrand(){
                console.log(this.brand);
            },
            //分页
            handleSizeChange(val) {
                console.log(`每页 ${val} 条`);
            },
            handleCurrentChange(val) {
                console.log(`当前页: ${val}`);
            }
        },
        data() {
            return {
                // 当前页码
                currentPage: 4,
                // 添加数据对话框是否展示的标记
                dialogVisible: false,

                // 品牌模型数据
                brand: {
                    status: '',
                    brandName: '',
                    companyName: '',
                    id:"",
                    ordered:"",
                    description:""
                },
                // 复选框选中数据集合
                multipleSelection: [],
                // 表格数据
                tableData: [{
                    brandName: '华为',
                    companyName: '华为科技有限公司',
                    ordered: '100',
                    status: "1"
                }, {
                    brandName: '华为',
                    companyName: '华为科技有限公司',
                    ordered: '100',
                    status: "1"
                }, {
                    brandName: '华为',
                    companyName: '华为科技有限公司',
                    ordered: '100',
                    status: "1"
                }, {
                    brandName: '华为',
                    companyName: '华为科技有限公司',
                    ordered: '100',
                    status: "1"
                }]
            }
        }
    })
</script>
</body>
</html>
```

---
# 3. 综合案例
## 3.1 功能介绍
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150240391.png)
以上是我们在综合案例要实现的功能。对数据的除了对数据的增删改查功能外，还有一些复杂功能，**如 `批量删除`、`分页查询`、`条件查询` 等功能**：
* `批量删除` 功能：每条数据前都有复选框，当我选中多条数据并点击 `批量删除` 按钮后，会发送请求到后端并删除数据库中指定的多条数据。
* `分页查询` 功能：当数据库中有很多数据时，我们不可能将所有的数据展示在一页里，这个时候就需要分页展示数据。
* `条件查询` 功能：数据库量大的时候，我们就需要精确的查询一些想看到的数据，这个时候就需要通过条件查询。

这里的 `修改品牌` 和 `删除品牌` 功能不做讲解，留作练习。
## 3.2 环境准备
环境准备我们主要完成以下两件事即可
* 将 `brand-case` 模块导入到IDEA中
* 执行 `tb_brand.sql` 脚本
### 3.2.1 工程准备
将 `brand-case` 工程导入到我们自己的IDEA中。工程结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150324231.png)
### 3.2.2 创建表
下面是创建表的语句：
```sql
-- 删除tb_brand表
drop table if exists tb_brand;
-- 创建tb_brand表
create table tb_brand (
    -- id 主键
    id           int primary key auto_increment,
    -- 品牌名称
    brand_name   varchar(20),
    -- 企业名称
    company_name varchar(20),
    -- 排序字段
    ordered      int,
    -- 描述信息
    description  varchar(100),
    -- 状态：0：禁用  1：启用
    status       int
);
-- 添加数据
insert into tb_brand (brand_name, company_name, ordered, description, status)
values 
       ('华为', '华为技术有限公司', 100, '万物互联', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('格力', '格力电器股份有限公司', 30, '让世界爱上中国造', 1),
       ('阿里巴巴', '阿里巴巴集团控股有限公司', 10, '买买买', 1),
       ('腾讯', '腾讯计算机系统有限公司', 50, '玩玩玩', 0),
       ('百度', '百度在线网络技术公司', 5, '搜搜搜', 0),
       ('京东', '北京京东世纪贸易有限公司', 40, '就是快', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('三只松鼠', '三只松鼠股份有限公司', 5, '好吃不上火', 0),
       ('华为', '华为技术有限公司', 100, '万物互联', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('格力', '格力电器股份有限公司', 30, '让世界爱上中国造', 1),
       ('阿里巴巴', '阿里巴巴集团控股有限公司', 10, '买买买', 1),
       ('腾讯', '腾讯计算机系统有限公司', 50, '玩玩玩', 0),
       ('百度', '百度在线网络技术公司', 5, '搜搜搜', 0),
       ('京东', '北京京东世纪贸易有限公司', 40, '就是快', 1),
       ('华为', '华为技术有限公司', 100, '万物互联', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('格力', '格力电器股份有限公司', 30, '让世界爱上中国造', 1),
       ('阿里巴巴', '阿里巴巴集团控股有限公司', 10, '买买买', 1),
       ('腾讯', '腾讯计算机系统有限公司', 50, '玩玩玩', 0),
       ('百度', '百度在线网络技术公司', 5, '搜搜搜', 0),
       ('京东', '北京京东世纪贸易有限公司', 40, '就是快', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('三只松鼠', '三只松鼠股份有限公司', 5, '好吃不上火', 0),
       ('华为', '华为技术有限公司', 100, '万物互联', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('格力', '格力电器股份有限公司', 30, '让世界爱上中国造', 1),
       ('阿里巴巴', '阿里巴巴集团控股有限公司', 10, '买买买', 1),
       ('腾讯', '腾讯计算机系统有限公司', 50, '玩玩玩', 0),
       ('百度', '百度在线网络技术公司', 5, '搜搜搜', 0),
       ('京东', '北京京东世纪贸易有限公司', 40, '就是快', 1),
       ('华为', '华为技术有限公司', 100, '万物互联', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('格力', '格力电器股份有限公司', 30, '让世界爱上中国造', 1),
       ('阿里巴巴', '阿里巴巴集团控股有限公司', 10, '买买买', 1),
       ('腾讯', '腾讯计算机系统有限公司', 50, '玩玩玩', 0),
       ('百度', '百度在线网络技术公司', 5, '搜搜搜', 0),
       ('京东', '北京京东世纪贸易有限公司', 40, '就是快', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('三只松鼠', '三只松鼠股份有限公司', 5, '好吃不上火', 0),
       ('华为', '华为技术有限公司', 100, '万物互联', 1),
       ('小米', '小米科技有限公司', 50, 'are you ok', 1),
       ('格力', '格力电器股份有限公司', 30, '让世界爱上中国造', 1),
       ('阿里巴巴', '阿里巴巴集团控股有限公司', 10, '买买买', 1),
       ('腾讯', '腾讯计算机系统有限公司', 50, '玩玩玩', 0),
       ('百度', '百度在线网络技术公司', 5, '搜搜搜', 0),
       ('京东', '北京京东世纪贸易有限公司', 40, '就是快', 1);
```
## 3.3 查询所有功能
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150324212.png)
如上图所示是查询所有品牌数据在页面展示的效果。要实现这个功能，要先搞明白如下问题：
* **什么时候发送异步请求**？
  页面加载完毕后就需要在页面上看到所有的品牌数据。所以在 `mounted()` 这个构造函数中写发送异步请求的代码。
* **请求需要携带参数吗**？
  查询所有功能不需要携带什么参数。
* **响应的数据格式是什么样**？
  后端是需要将 `List<Brand>` 对象转换为 JSON 格式的数据并响应回给浏览器。响应数据格式如下：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150325588.png)

整体流程如下： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150325990.png)
我们先实现后端程序，然后再实现前端程序。
### 3.3.1 后端实现
#### 3.3.1.1 DAO方法实现
在 `com.itheima.mapper.BrandMapper` 接口中定义抽象方法，并使用 `@Select` 注解编写SQL语句：
```java
/**
     * 查询所有
     * @return
     */
@Select("select * from tb_brand")
List<Brand> selectAll();
```
由于表中有些字段名和实体类中的属性名没有对应，所以需要在 `com/itheima/mapper/BrandMapper.xml` 映射配置文件中定义结果映射 ，使用`resultMap` 标签。映射配置文件内容如下：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.itheima.mapper.BrandMapper">
    <resultMap id="brandResultMap" type="brand">
        <result property="brandName" column="brand_name" />
        <result property="companyName" column="company_name" />
    </resultMap>
</mapper>
```
定义完结果映射关系后，在接口 `selectAll()` 方法上引用该结构映射。使用 `@ResultMap("brandResultMap")` 注解。完整接口的 `selectAll()` 方法如下：
```java
/**
     * 查询所有
     * @return
     */
@Select("select * from tb_brand")
@ResultMap("brandResultMap")
List<Brand> selectAll();
```
#### 3.3.1.2 Service方法实现
在 `com.itheima.service` 包下创建 `BrandService` 接口，在该接口中定义查询所有的抽象方法：
```java
public interface BrandService {

    /**
     * 查询所有
     * @return
     */
    List<Brand> selectAll();
}
```
并**在 `com.itheima.service` 下再创建 `impl` 包；`impl` 表示是放 `service` 层接口的实现类的包**。 在该包下创建名为 `BrandServiceImpl` 类：
```java
public class BrandServiceImpl implements BrandService {
    @Override
    public List<Brand> selectAll() {
    }
}
```
此处为什么要给Service定义接口呢？**因为Service定义了接口后，在Servlet就可以使用多态的形式创建Service实现类的对象**，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150328726.png)
这里使用多态，是因为方便我们后期解除 `Servlet` 和 `service` 的耦合。从上面的代码我们可以看到 `SelectAllServlet` 类和 `BrandServiceImpl` 类之间是耦合在一起的，如果后期 `BrandService` 有其它更好的实现类（例如叫 `BrandServiceImpl`），那就需要修改 `SelectAllServlet` 类中的代码。后面我们学习了 `Spring` 框架后就可以解除 `SelectAllServlet` 类和红色框括起来的代码耦合。而现在咱们还做不到解除耦合，在这里只需要理解为什么定义接口即可。

`BrandServiceImpl` 类代码如下：
```java
public class BrandServiceImpl implements BrandService {
    //1. 创建SqlSessionFactory 工厂对象
    SqlSessionFactory factory = SqlSessionFactoryUtils.getSqlSessionFactory();

    @Override
    public List<Brand> selectAll() {
        //2. 获取SqlSession对象
        SqlSession sqlSession = factory.openSession();
        //3. 获取BrandMapper
        BrandMapper mapper = sqlSession.getMapper(BrandMapper.class);
        //4. 调用方法
        List<Brand> brands = mapper.selectAll();
        //5. 释放资源
        sqlSession.close();
        return brands;
    }
}
```
#### 3.3.1.3 Servlet实现
在 `com.itheima.web.servlet` 包下定义名为 `SelectAllServlet` 的查询所有的 `servlet`。该 `servlet` 逻辑如下：
* 调用Service的 `selectAll()` 方法查询所有的品牌数据，并接口返回结果
* 将返回的结果转换为JSON数据
* 响应JSON数据

代码如下：
```java
@WebServlet("/selectAllServlet")
public class SelectAllServlet extends HttpServlet {

    private BrandService brandService = new BrandServiceImpl();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 调用service查询
        List<Brand> brands = brandService.selectAll();
        //2. 转为JSON
        String jsonString = JSON.toJSONString(brands);
        //3. 写数据
        response.setContentType("text/json;charset=utf-8"); //告知浏览器响应的数据是什么,告知浏览器使用什么字符集进行解码
        response.getWriter().write(jsonString);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
#### 3.3.1.4 测试后端程序
在浏览器输入访问Servlet的资源路径 `http://localhost:8080/brand-case/selectAllServlet` ，如果没有报错，并能看到如下信息表明后端程序没有问题
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150330853.png)
### 3.3.2 前端实现
前端需要在页面加载完毕后发送AJAX请求，所以发送请求的逻辑应该放在 `mounted()` 钩子函数中。而**响应回来的数据需要赋值给表格绑定的数据模型**，从下图可以看出表格绑定的数据模型是 `tableData` ：
![img|470x200](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150330152.png)
前端代码如下：
```js
 mounted(){
     //当页面加载完成后，发送异步请求，获取数据
     var _this = this;

     axios({
         method:"get",
         url:"http://localhost:8080/brand-case/selectAllServlet"
     }).then(function (resp) {
         _this.tableData = resp.data;
     })
 }
```
## 3.4 添加功能
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150129440.png)
上图是添加数据的对话框，当点击 `提交` 按钮后就需要将数据提交到后端，并将数据保存到数据库中。下图是整体的流程：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150332079.png)
页面发送请求时，需要将输入框输入的内容提交给后端程序，而这里是以 json 格式进行传递的。而具体的数据格式如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150333540.png)
> ==注意：由于是添加数据，所以上述JSON数据中id是没有值的。==
### 3.4.1 后端实现
#### 3.4.1.1 DAO方法实现
在 `BrandMapper` 接口中定义 `add()` 添加方法，并使用 `@Insert` 注解编写SQL语句：
```java
/**
 * 添加数据
 * @param brand
 */
@Insert("insert into tb_brand values(null,#{brandName},#{companyName},#{ordered},#{description},#{status})")
void add(Brand brand);
```
#### 3.4.1.2 Service方法实现
在 `BrandService` 接口中定义 `add()` 添加数据的业务逻辑方法：
```java
/**
 * 添加数据
 * @param brand
 */
void add(Brand brand);
```
在 `BrandServiceImpl` 类中重写 `add()` 方法，并进行业务逻辑实现：
```java
@Override
public void add(Brand brand) {
    //2. 获取SqlSession对象
    SqlSession sqlSession = factory.openSession();
    //3. 获取BrandMapper
    BrandMapper mapper = sqlSession.getMapper(BrandMapper.class);

    //4. 调用方法
    mapper.add(brand);
    sqlSession.commit();//提交事务

    //5. 释放资源
    sqlSession.close();
}
```
> ==注意：增删改操作一定要提交事务。==
#### 3.4.1.3 Servlet实现
在 `com.itheima.web.servlet` 包写定义名为 `AddServlet` 的Servlet。该Servlet的逻辑如下：
* 接收页面提交的数据。页面到时候提交的数据是 json 格式的数据，所以此处需要使用输入流读取数据
* 将接收到的数据转换为 `Brand` 对象
* 调用Service的 `add()` 方法进行添加的业务逻辑处理
* 给浏览器响应添加成功的标识，这里直接给浏览器响应 `success` 字符串表示成功

Servlet代码实现如下：
```java
@WebServlet("/addServlet")
public class AddServlet extends HttpServlet {

    private BrandService brandService = new BrandServiceImpl();

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        //1. 接收品牌数据
        BufferedReader br = request.getReader();
        String params = br.readLine();//json字符串
        //转为Brand对象
        Brand brand = JSON.parseObject(params, Brand.class);
        //2. 调用service添加
        brandService.add(brand);
        //3. 响应成功的标识
        response.getWriter().write("success");
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        this.doGet(request, response);
    }
}
```
### 3.4.2 前端实现
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150334431.png)
上图左边是页面效果，里面的 `提交` 按钮可以通过上图右边看出绑定了一个单击事件，而该事件绑定的是 `addBrand` 函数，所以添加数据功能的逻辑代码应该写在 `addBrand()`  函数中。在此方法中需要发送异步请求并将表单中输入的数据作为参数进行传递。如下：
```js
// 添加数据
addBrand() {
    var _this = this;
    // 发送ajax请求，添加数据
    axios({
        method:"post",
        url:"http://localhost:8080/brand-case/addServlet",
        data:_this.brand
    }).then(function (resp) {
       	//响应数据的处理逻辑
    })
}
```
在 `then` 函数中的匿名函数是成功后的回调函数，而 `resp.data` 就可以获取到响应回来的数据，如果值是 `success` 表示数据添加成功。成功后我们需要做一下逻辑处理：
(1) **关闭新增对话框窗口**——如下图所示是添加数据的对话框代码，从代码中可以看到此对话框绑定了 `dialogVisible` 数据模型，只需要将该数据模型的值设置为 `false` ，就可以关闭新增对话框窗口了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150335169.png)

(2) **重新查询数据**——数据添加成功与否，用户只要能在页面上查看到数据说明添加成功。而此处需要重新发送异步请求获取所有的品牌数据，而这段代码在 `查询所有` 功能中已经实现，所以我们可以将此功能代码进行抽取，抽取到一个 `selectAll()` 函数中：
```js
// 查询所有数据
selectAll(){
   var _this = this;

   axios({
	   method:"get",
	   url:"http://localhost:8080/brand-case/selectAllServlet"
   }).then(function (resp) {
	   _this.tableData = resp.data;
   })
}
```
那么就需要将 `mounted()` 钩子函数中代码改进为：
```js
mounted(){
   //当页面加载完成后，发送异步请求，获取数据
   this.selectAll();
}
```
同时在新增响应的回调中调用 `selectAll()` 进行数据的重新查询。

(3) **弹出消息给用户提示添加成功**——下图左边就是ElementUI官网提供的成功提示代码，而下图右边是具体的效果。==注意：下面的 `this` 需要的是表示Vue对象的 `this` 。==
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302150336211.png)

综上所述，前端代码如下：
```js
// 添加数据
addBrand() {
    var _this = this;

    // 发送ajax请求，添加数据
    axios({
        method:"post",
        url:"http://localhost:8080/brand-case/addServlet",
        data:_this.brand
    }).then(function (resp) {
        if(resp.data == "success"){
            //添加成功
            //关闭窗口
            _this.dialogVisible = false;
            // 重新查询数据
            _this.selectAll();
            // 弹出消息提示
            _this.$message({
                message: '恭喜你，添加成功',
                type: 'success'
            });
        }
    })
}
```
添加数据库乱码的，配置连接数据库可以这样写 `jdbc:mysql:///库名?useSSL=false&amp;useUnicode=true&amp;characterEncoding=UTF-8` 。

---
# 5. servlet优化
## 5.1 问题导入
==Web层的Servlet个数太多了，不利于管理和编写==——通过之前的两个功能，我们发现每一个功能都需要定义一个 `servlet`，一个模块需要实现增删改查功能，就需要4个Servlet，模块一多就会造成Servlet泛滥。此时我们就想Servlet能不能像Service一样，==将原有的各个Servlet进行归类，对同一个实体的多个操作都写到一个Servlet中==，比如 `BrandServlet, UserServlet` ，这样**一个模块只定义一个Servlet，而每一个功能只需要在该Servlet中定义对应的方法**，。例如下面代码：
```java
@WebServlet("/brand/*")
public class BrandServlet {
    //查询所有
	public void selectAll(...) {}
    
    //添加数据
    public void add(...) {}
    
     //修改数据
    public void update(...) {}
    
    //删除删除
    public void delete(...) {}
}
```
而我们知道发送请求Servlet，Tomcat会自动的调用 `service()` 方法，之前我们在自定义的Servlet中重写 `doGet()` 方法和 `doPost()` 方法，**当我们访问该Servlet时会根据请求方式将请求分发给 `doGet()` 或者 `doPost()`  方法**，如下图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302151345230.png)
那么我们也可以**仿照这样请求分发的思想**，在 `service()` 方法中根据具体的操作调用对应的方法，如：查询所有就调用 `selectAll()` 方法，添加企业信息就调用 `add()` 方法。
> 跨域访问我是用的jsp动态获取当前request请求的url然后拼字符串；局域网记得把前面的http://localhost:8080去掉，默认就是在同项目内访问，就不会跨域问题了

为了做到通用，我们定义一个通用的 `servlet` 类，在定义其他的 `servlet` 时不需要继承 `HttpServlet`，而**继承我们定义的 `BaseServlet`** ，在` BaseServlet` 中调用具体 `servlet`（如`BrandServlet`）中的对应方法。
```java
public class BaseServlet extends HttpServlet {
    @Override
    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //进行请求的分发
    }
}
```
`BrandServlet` 定义就需要修改为如下：
```java
@WebServlet("/brand/*")
public class BrandServlet extends BaseServlet {
    //用户实现分页查询
	public void selectAll(...) {} 
    
    //添加企业信息
    public void add(...) {}
    
    //修改企业信息
    public void update(...) {}
    
    //删除企业信息
    public void delete(...) {}
}
```
那么如何在 `BaseServlet` 中调用对应的方法呢？比如查询所有就调用 `selectAll()` 方法。可以规定==在发送请求时，请求资源的二级路径（`/brandServlet/selectAll`）和需要调用的方法名相同==，如：
- 查询所有数据的路径以后就需要写成： `http://localhost:8080/brand-case/brandServlet/selectAll`
- 添加数据的路径以后就需要写成： `http://localhost:8080/brand-case/brandServlet/add`
- 修改数据的路径以后就需要写成： `http://localhost:8080/brand-case/brandServlet/update`
- 删除数据的路径以后就需要写成： `http://localhost:8080/brand-case/brandServlet/delete`

这样的话，在 `BaseServlet` 中就需要获取到资源的二级路径作为方法名，然后调用该方法（Servlet不管谁继承谁重写，机制就是调用时必执行 `service` 方法）：
```java
public class BaseServlet extends HttpServlet {
    @Override
    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //1. 获取请求路径
        String uri = req.getRequestURI(); // 例如路径为：/brand-case/brand/selectAll
        //2. 获取最后一段路径，方法名
        int index = uri.lastIndexOf('/');
        String methodName = uri.substring(index + 1); //  获取到资源的二级路径  selectAll
        
        //2. 执行方法
        //2.1 获取如BrandServlet或者UserServlet它的字节码对象 Class
        //System.out.println(this);
        Class<? extends BaseServlet> cls = this.getClass();
        //2.2 获取方法 Method对象
        try {
            Method method = cls.getMethod(methodName, ???);
            //4,调用该方法
            //this:谁调用我(这个this所在的方法),我(这个this)代表谁
            //this代表将来的BrandServlet或UserServlet对象
            //谁调用service方法呢?
            method.invoke(this, ???);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }
}
```
通过上面代码发现，**根据方法名获取对应方法的 `Method` 对象时，需要指定方法参数的字节码对象**。解决这个问题，可以将方法的参数类型规定死，而方法中可能需要用到 `request` 对象和 `response` 对象，所以指定方法的参数为 `HttpServletRequest` 和 `HttpServletResponse`，那么 `BrandServlet`（子类继承父类，当子类执行时会调用父类方法）代码就可以改进为：
```java
@WebServlet("/brand/*")
public class BrandServlet extends BaseServlet {
    //用户实现分页查询
	public void selectAll(HttpServletRequest req, HttpServletResponse resp) {}
    
    //添加企业信息
    public void add(HttpServletRequest req, HttpServletResponse resp) {}
    
    //修改企业信息
    public void update(HttpServletRequest req, HttpServletResponse resp) {}
    
    //删除企业信息
    public void delete(HttpServletRequest req, HttpServletResponse resp) {}
}
```
`BaseServlet` 代码可以改进为：
```java
public class BaseServlet extends HttpServlet {
    //根据请求的最后一段路径来进行方法分发
    @Override
    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //1. 获取请求路径
        String uri = req.getRequestURI(); // 例如路径为：/brand-case/brand/selectAll
        //2. 获取最后一段路径，方法名
        int index = uri.lastIndexOf('/');
        String methodName = uri.substring(index + 1); //  获取到资源的二级路径  selectAll   

        //2. 执行方法
        //2.1 获取BrandServlet /UserServlet 字节码对象 Class
        //System.out.println(this);

        Class<? extends BaseServlet> cls = this.getClass();
        //2.2 获取方法 Method对象
        try {   
            Method method = cls.getMethod(methodName, HttpServletRequest.class, HttpServletResponse.class);
            //2.3 执行方法
            method.invoke(this,req,resp);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }
}
```
## 5.2  代码优化
### 5.2.1 后端优化
定义了 `BaseServlet` 后，针对品牌模块我们定义一个 `BrandServlet` 的Servlet，并使其继承 `BaseServlet` 。在`BrandServlet`中定义 以下功能的方法：
* `查询所有`  功能：方法名声明为 `selectAll` ，并将之前的 `SelectAllServlet` 中的逻辑代码拷贝到该方法中
*  `添加数据` 功能：方法名声明为 `add` ，并将之前的 `AddServlet` 中的逻辑代码拷贝到该方法中

具体代码如下：
```java
@WebServlet("/brand/*")
public class BrandServlet extends BaseServlet {
    private BrandService brandService = new BrandServiceImpl();

    public void selectAll(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 调用service查询
        List<Brand> brands = brandService.selectAll();

        //2. 转为JSON
        String jsonString = JSON.toJSONString(brands);
        //3. 写数据
        response.setContentType("text/json;charset=utf-8");
        response.getWriter().write(jsonString);
    }

    public void add(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //1. 接收品牌数据
        BufferedReader br = request.getReader();
        String params = br.readLine(); //json字符串
        //转为Brand对象
        Brand brand = JSON.parseObject(params, Brand.class);
        //2. 调用service添加
        brandService.add(brand);
        //3. 响应成功的标识
        response.getWriter().write("success");
    }
}
```
### 5.2.2 前端优化
**页面中之前发送的请求的路径都需要进行修改**，`selectAll()` 函数中发送异步请求的 `url` 应该改为 `http://localhost:8080/brand-case/brand/selectAll` 。具体代码如下：
```js
// 查询分页数据
selectAll(){
    var _this = this;
    axios({
        method:"get",
        url:"http://localhost:8080/brand-case/brand/selectAll"
    }).then(function (resp) {
        _this.tableData = resp.data;
    })
}
```
`addBrand()` 函数中发送异步请求的 `url` 应该改为 `http://localhost:8080/brand-case/brand/add` 。具体代码如下：
```js
// 添加数据
addBrand() {
    //console.log(this.brand);
    var _this = this;

    // 发送ajax请求，添加数据
    axios({
        method:"post",
        url:"http://localhost:8080/brand-case/brand/add",
        data:_this.brand
    }).then(function (resp) {
        if(resp.data == "success"){
            //添加成功
            //关闭窗口
            _this.dialogVisible = false;
            // 重新查询数据
            _this.selectAll();
            // 弹出消息提示
            _this.$message({
                message: '恭喜你，添加成功',
                type: 'success'
            });
        }
    })
}
```
---
# 6. 批量删除
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152046520.png)
如上图所示点击多条数据前的复选框就意味着要删除这些数据，而点击了 `批量删除` 按钮后，需要让用户确认一下，因为有可能是用户误操作的，当用户确定后需要给后端发送请求并携带者需要删除数据的多个ID值，后端程序删除数据库中的数据。具体的流程如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152047223.png)
==注意==：前端发送请求时需要将要删除的多个ID值以JSON格式提交给后端，而该JSON格式数据如下：
```json
[1,2,3,4]
```
## 6.1 后端实现
### 6.1.1 `dao` 方法实现
在 `BrandMapper` 接口中定义 `deleteByIds()` 添加方法，==由于这里面要用到动态SQL ，属于复杂的SQL操作，建议使用**映射配置文件**==。接口方法声明如下：
```java
/**
 * 批量删除
 * @param ids
 */
void deleteByIds(@Param("ids") int[] ids);
```
在 `BrandMapper.xml` 映射配置文件中添加 `statement` ：
```xml
<delete id="deleteByIds">
    delete from tb_brand where id in
    <foreach collection="ids" item="id" separator="," open="(" close=")">
        #{id}
    </foreach>
</delete>
```
### 6.1.2 `service` 方法实现
在 `BrandService` 接口中定义 `deleteByIds()` 批量删除的业务逻辑方法：
```java
/**
 * 批量删除
 * @param ids
 */
void deleteByIds(int[] ids);
```
在 `BrandServiceImpl` 类中重写 `deleteByIds()` 方法，并进行业务逻辑实现：
```java
@Override
public void deleteByIds(int[] ids) {
    //2. 获取SqlSession对象
    SqlSession sqlSession = factory.openSession();
    //3. 获取BrandMapper
    BrandMapper mapper = sqlSession.getMapper(BrandMapper.class);

    //4. 调用方法
    mapper.deleteByIds(ids);

    sqlSession.commit();//提交事务

    //5. 释放资源
    sqlSession.close();
}
```
### 6.1.3 Servlet实现
在 `BrandServlet` 类中定义 `deleteByIds()`  方法。而该方法的逻辑如下： 
* 接收页面提交的数据。页面到时候提交的数据是JSON格式的数据，所以此处需要使用输入流读取数据
* **将接收到的数据转换为 `int[]` 数组**
* 调用 `service` 的 `deleteByIds()` 方法进行批量删除的业务逻辑处理（或者可以一个个删除）
* 给浏览器响应添加成功的标识，这里直接给浏览器响应 `success` 字符串表示成功

Servlet中 `deleteByIds()` 方法代码实现如下：
```java
public void deleteByIds(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    //1. 接收数据 json  [1,2,3]
    BufferedReader br = request.getReader();
    String params = br.readLine();//json字符串
    //转为 int[]
    int[] ids = JSON.parseObject(params, int[].class);
    //2. 调用service添加
    brandService.deleteByIds(ids);
    //3. 响应成功的标识
    response.getWriter().write("success");
}
```
### 6.2 前端实现
此功能的前端代码实现稍微有点麻烦，分为以下几步实现
### 6.2.1 获取选中的id值
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152102187.png)
从上图可以看出表格复选框绑定了一个 `selection-change` 事件，该事件是当选择项发生变化时会触发。该事件绑定了 `handleSelectionChange` 函数，而该函数有一个参数 `val` ，该参数是获取选中行的数据，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152103964.png)
而我们只需要将所有选中数据的id值提交给服务端即可，获取ID的逻辑我们书写在 `批量删除` 按钮绑定的函数中。

在 `批量删除` 按钮绑定单击事件，并给它绑定触发时调用的函数，如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152104762.png)
在Vue对象中的 `methods` 中定义 `deleteByIds()` 函数，**在该函数中从 `multipleSelection` 数据模型中获取所选数据的ID值**。要完成这个功能，需要在Vue对象中定义一个数据模型 `selectedIds:[]` ，在 `deleteByIds()` 函数中遍历 `multipleSelection` 数组，并获取到每一个所选数据的ID值存储到 `selectedIds` 数组中，代码实现如下：
```js
//1. 创建id数组 [1,2,3], 从 this.multipleSelection 获取即可
for (let i = 0; i < this.multipleSelection.length; i++) {
    let selectionElement = this.multipleSelection[i];
    this.selectedIds[i] = selectionElement.id;
}
```
### 6.2.2 发送异步请求
使用Axios发送异步请求，将「上一步获取到的存储所有ID的数组」作为请求参数：
```js
//2. 发送AJAX请求
var _this = this;

// 发送Ajax请求，添加数据
axios({
    method:"post",
    url:"http://localhost:8080/brand-case/brand/deleteByIds",
    data:_this.selectedIds
}).then(function (resp) {
    if(resp.data == "success"){
        // 删除成功
        // 重新查询数据
        _this.selectAll();
        // 弹出消息提示
        _this.$message({
            message: '恭喜你，删除成功',
            type: 'success'
        });
    }
})
```
### 6.2.3 确定框实现
由于删除操作是比较危险的；有时候可能是由于用户的误操作点击了 `批量删除` 按钮，所以在点击了按钮后需要先给用户确认提示。而确认框在ElementUI中也提供了，如下图 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152106151.png)
而在点击 `确定` 按钮后需要执行之前删除的逻辑。因此前端代码实现如下：
```js
 // 批量删除
deleteByIds(){
    // 弹出确认提示框
    this.$confirm('此操作将删除该数据, 是否继续?', '提示', {
        confirmButtonText: '确定',
        cancelButtonText: '取消',
        type: 'warning'
    }).then(() => {
        //用户点击确认按钮
        //1. 创建id数组 [1,2,3], 从 this.multipleSelection 获取即可
        for (let i = 0; i < this.multipleSelection.length; i++) {
            let selectionElement = this.multipleSelection[i];
            this.selectedIds[i] = selectionElement.id;
        }
        //2. 发送AJAX请求
        var _this = this;
        // 发送ajax请求，添加数据
        axios({
            method:"post",
            url:"http://localhost:8080/brand-case/brand/deleteByIds",
            data:_this.selectedIds
        }).then(function (resp) {
            if(resp.data == "success"){
                //删除成功
                // 重新查询数据
                _this.selectAll();
                // 弹出消息提示
                _this.$message({
                    message: '恭喜你，删除成功',
                    type: 'success'
                });
            }
        })
    }).catch(() => {
        //用户点击取消按钮
        this.$message({
            type: 'info',
            message: '已取消删除'
        });
    });
}
```
---
# 7. 分页查询
之前做的 `查询所有` 功能中将数据库中所有的数据查询出来并展示到页面上，试想如果数据库中的数据有很多（假设有十几万条）的时候，将数据全部展示出来肯定不现实，那如何解决这个问题呢？==几乎所有的网站都会使用分页解决这个问题==。每次只展示一页的数据，比如一页展示10条数据，如果还想看其他的数据，可以通过点击页码进行查询。
## 7.1 分析
### 7.1.1 分页查询SQL
==分页查询也是从数据库进行查询的，所以我们要分页对应的SQL语句应该怎么写==。分页查询使用 `LIMIT` 关键字，格式为：==`LIMIT 开始索引 每页显示的条数`==。以后前端页面在发送请求携带参数时，它并不明确开始索引是什么，但是它知道查询第几页。所以 `开始索引` 需要在后端进行计算，计算的公式是 ：==开始索引 =（当前页码 -  1）*  每页显示条数==。比如查询第一页的数据的SQL语句是：
```sql
select * from tb_brand limit 0,5;
```
查询第二页的数据的SQL语句是：
```sql
select * from tb_brand limit 5,5;
```
查询第三页的数据的SQL语句是：
```sql
select * from tb_brand  limit 10,5;
```
### 7.1.2 前后端数据分析
分页查询功能是比较复杂的，所以我们要先分析清楚以下两个问题：
* **前端需要传递什么参数给后端**——根据上一步对分页查询 SQL 语句分析得出，前端需要给后端两个参数
	* 当前页码：`currentPage`
	* 每页显示条数：`pageSize`
* **后端需要响应什么数据给前端**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152123053.png)
  上图是分页查询页面展示的效果，从上面我们可以看出需要响应以下数据：
  * **当前页需要展示的数据**。我们在后端一般会存储到 `List` 集合中
  * **总共记录数**。在上图页面中需要展示的总记录数，所以这部分响应数据也需要。ElementUI的分页组件会自动计算总的页面：总共记录数/当前页需展示数据的数目，我们不需要关心

	而这两部分需要封装到 `PageBean` 对象中，并将该对象转换为JSON格式的数据响应回给浏览器：
	 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152124460.png)

通过上面的分析，我们需要先在 `pojo` 包下创建 `PageBean` 类，为了做到通过会将其定义成泛型类，代码如下：
```java
//分页查询的JavaBean
public class PageBean<T> {
    // 总记录数
    private int totalCount;
    // 当前页数据
    private List<T> rows;
    public int getTotalCount() {
        return totalCount;
    }
    public void setTotalCount(int totalCount) {
        this.totalCount = totalCount;
    }
    public List<T> getRows() {
        return rows;
    }
    public void setRows(List<T> rows) {
        this.rows = rows;
    }
}
```
### 7.1.3 流程分析
后端需要响应 `总记录数` 和 `当前页的数据` 两部分数据给前端，所以在 `BrandMapper`  接口中需要定义两个方法：
* `selectByPage()` ：查询当前页的数据的方法
* `selectTotalCount()` ：查询总记录的方法

整体流程如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152129966.png)

## 7.2 后端实现
### 7.2.1 `dao` 方法实现
在 `BrandMapper` 接口中定义 `selectByPage()` 方法进行分页查询，代码如下：
```java
/**
 * 分页查询
 * @param begin
 * @param size
 * @return
 */
@Select("select * from tb_brand limit #{begin} , #{size}")
@ResultMap("brandResultMap")
List<Brand> selectByPage(@Param("begin") int begin, @Param("size") int size);
```
在 `BrandMapper` 接口中定义 `selectTotalCount()` 方法进行统计记录数，代码如下：
```java
/**
 * 查询总记录数
 * @return
 */
@Select("select count(*) from tb_brand ")
int selectTotalCount();
```
### 7.2.2 `service` 方法实现
在 `BrandService` 接口中定义 `selectByPage()` 分页查询数据的业务逻辑方法：
```java
/**
 * 分页查询
 * @param currentPage  当前页码
 * @param pageSize   每页展示条数
 * @return
 */
PageBean<Brand>  selectByPage(int currentPage,int pageSize);
```
在 `BrandServiceImpl` 类中重写 `selectByPage()` 方法，并进行业务逻辑实现：
```java
@Override
public PageBean<Brand> selectByPage(int currentPage, int pageSize) {
    //2. 获取SqlSession对象
    SqlSession sqlSession = factory.openSession();
    //3. 获取BrandMapper
    BrandMapper mapper = sqlSession.getMapper(BrandMapper.class);
    //4. 计算开始索引
    int begin = (currentPage - 1) * pageSize;
    // 计算查询条目数
    int size = pageSize;
    //5. 查询当前页数据
    List<Brand> rows = mapper.selectByPage(begin, size);
    //6. 查询总记录数
    int totalCount = mapper.selectTotalCount();
    //7. 封装PageBean对象
    PageBean<Brand> pageBean = new PageBean<>();
    pageBean.setRows(rows);
    pageBean.setTotalCount(totalCount);

    //8. 释放资源
    sqlSession.close();
    return pageBean;
}
```
### 7.2.3 Servlet实现
在 `BrandServlet` 类中定义 `selectByPage()`  方法。而该方法的逻辑如下：
* 获取页面提交的 `当前页码` 和 `每页显示条目数` 两个数据。这两个参数是在URL后进行拼接的，格式是  `url?currentPage=1&pageSize=5`。获取这样的参数需要使用 `requet.getparameter()` 方法获取。
* 调用 `service` 的 `selectByPage()` 方法进行分页查询的业务逻辑处理
* 将查询到的数据转换为JSON格式的数据
* 响应JSON数据

`servlet` 中 `selectByPage()` 方法代码实现如下：
```java
public void selectByPage(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    //1. 接收 当前页码 和 每页展示条数    url?currentPage=1&pageSize=5
    String _currentPage = request.getParameter("currentPage");
    String _pageSize = request.getParameter("pageSize");

    int currentPage = Integer.parseInt(_currentPage);
    int pageSize = Integer.parseInt(_pageSize);

    //2. 调用service查询
    PageBean<Brand> pageBean = brandService.selectByPage(currentPage, pageSize);

    //2. 转为JSON
    String jsonString = JSON.toJSONString(pageBean);
    //3. 写数据
    response.setContentType("text/json;charset=utf-8");
    response.getWriter().write(jsonString);
}
```
### 7.2.4 测试
在浏览器上地址栏输入 `http://localhost:8080/brand-case/brand/selectByPage?currentPage=1&pageSize=5` ，查询到以下数据
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152255224.png)
## 7.3 前端实现
### 7.3.1 `selectAll` 代码改进
`selectAll()` 函数之前是查询所有数据，现需要改成分页查询。 请求路径应改为 `http://localhost:8080/brand-case/brand/selectByPage?currentPage=1&pageSize=5` ，而 `currentPage`  和 `pageSize`  是需要携带的参数，分别是 当前页码 和 每页显示的条目数。

 刚才我们对后端代码进行测试可以看出响应回来的数据，所以在异步请求的成功回调函数（`then` 中的匿名函数）中**给页面表格的数据模型赋值 `_this.tableData = resp.data.rows;`** 。整体代码如下：
```js
var _this = this;
axios({
    method:"post",
    url:"http://localhost:8080/brand-case/brand/selectByPage？currentPage=1&pageSize=5"
}).then(resp =>{
    //设置表格数据
    _this.tableData = resp.data.rows; // {rows:[],totalCount:100}
})
```
响应的数据中还有总记录数，要进行总记录数展示需要在页面绑定数据模型：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152256110.png)
> ==注意：该数据模型需要在Vue对象中声明出来。==

那异步请求的代码就可以优化为：
```js
var _this = this;
axios({
    method:"post",
    url:"http://localhost:8080/brand-case/brand/selectByPage?currentPage=1&pageSize=5"
}).then(resp =>{
    //设置表格数据
    _this.tableData = resp.data.rows; // {rows:[],totalCount:100}
    //设置总记录数
    _this.totalCount = resp.data.totalCount;
})
```
而页面中分页组件给 `当前页码` 和 `每页显示的条目数` 都绑定了数据模型：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152258031.png)
所以 **`selectAll()` 函数中发送异步请求的资源路径中不能将当前页码和每页显示条目数写死**，代码就可以优化为：
```js
var _this = this;
axios({
    method:"post",
    url:"http://localhost:8080/brand-case/brand/selectByPage?currentPage=" + this.currentPage + "&pageSize=" + this.pageSize
}).then(resp =>{
    //设置表格数据
    _this.tableData = resp.data.rows; // {rows:[],totalCount:100}
    //设置总记录数
    _this.totalCount = resp.data.totalCount;
})
```
### 7.3.2 改变每页条目数
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152304822.png)
**当我们改变每页显示的条目数后，需要重新发送异步请求**。而下图是分页组件代码，`@size-change` 就是每页显示的条目数发生变化时会触发的事件：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152306139.png)
而该事件绑定了一个 `handleSizeChange` 函数，整个逻辑如下：
```js
handleSizeChange(val) { //我们选择的是 ‘5条/页’ 此值就是 5.而我们选择了 `10条/页` 此值就是 10
    // 重新设置每页显示的条数
    this.pageSize  = val; 
    //调用 selectAll 函数重新分页查询数据
    this.selectAll();
}
```
### 7.3.3 改变当前页码
**当我们改变页码时，需要重新发送异步请求**。而下图是分页组件代码，`@current-change` 就是页码发生变化时会触发的事件：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152306823.png)
而该事件绑定了一个 `handleSizeChange` 函数，整个逻辑如下：
```js
handleCurrentChange(val) { //val 就是改变后的页码
    // 重新设置当前页码
    this.currentPage  = val;
    //调用 selectAll 函数重新分页查询数据
    this.selectAll();
}
```

---
# 8. 条件查询
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152307351.png)
上图就是用来输入条件查询的条件数据的。要做条件查询功能，先明确以下三个问题
* 3个条件之间什么关系？
  同时满足，所用 SQL 中多个条件需要使用 and 关键字连接
* 3个条件必须全部填写吗？
  不需要。**想根据哪个条件查询就写那个，所以这里需要使用动态SQL语句**
* 条件查询需要分页吗？
  **需要**

根据上面三个问题的明确，我们就可以确定SQL语句了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152308879.png)
整个条件分页查询流程如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152308747.png)
## 8.1 后端实现
### 8.1.1 `dao` 实现
在 `BrandMapper` 接口中定义 `selectByPageAndCondition()` 方法 和 `selectTotalCountByCondition` 方法，用来进行条件分页查询功能，方法如下：
```java
/**
 * 分页条件查询
 * @param begin
 * @param size
 * @return
 */
List<Brand> selectByPageAndCondition(@Param("begin") int begin,@Param("size") int size,@Param("brand") Brand brand);

/**
 * 根据条件查询总记录数
 * @return
 */
int selectTotalCountByCondition(Brand brand);
```
参数：
* `begin` 分页查询的起始索引
* `size` 分页查询的每页条目数
* `brand` 用来封装条件的对象

由于这是一个复杂的查询语句，需要使用动态SQL；所以我们在映射配置文件中书写SQL语句。`brand_name` 字段和 `company_name` 字段需要进行模糊查询，所以需要使用 `%` 占位符。映射配置文件中 `statement` 书写如下：
```xml
<!--查询满足条件的数据并进行分页-->
<select id="selectByPageAndCondition" resultMap="brandResultMap">
    select *
    from tb_brand
    <where>
        <if test="brand.brandName != null and brand.brandName != '' ">
            and  brand_name like #{brand.brandName}
        </if>

        <if test="brand.companyName != null and brand.companyName != '' ">
            and  company_name like #{brand.companyName}
        </if>

        <if test="brand.status != null">
            and  status = #{brand.status}
        </if>
    </where>
    limit #{begin} , #{size}
</select>

<!--查询满足条件的数据条目数-->
<select id="selectTotalCountByCondition" resultType="java.lang.Integer">
    select count(*)
    from tb_brand
    <where>
        <if test="brandName != null and brandName != '' ">
            and  brand_name like #{brandName}
        </if>
        <if test="companyName != null and companyName != '' ">
            and  company_name like #{companyName}
        </if>
        <if test="status != null">
            and  status = #{status}
        </if>
    </where>
</select>
```
### 8.1.2 `service` 实现
在 `BrandService` 接口中定义 `selectByPageAndCondition()` 分页查询数据的业务逻辑方法：
```java
/**
 * 分页条件查询
 * @param currentPage
 * @param pageSize
 * @param brand
 * @return
 */
PageBean<Brand>  selectByPageAndCondition(int currentPage,int pageSize,Brand brand);
```
在 `BrandServiceImpl` 类中重写 `selectByPageAndCondition()` 方法，并进行业务逻辑实现：
```java
 @Override
    public PageBean<Brand> selectByPageAndCondition(int currentPage, int pageSize, Brand brand) {
        //2. 获取SqlSession对象
        SqlSession sqlSession = factory.openSession();
        //3. 获取BrandMapper
        BrandMapper mapper = sqlSession.getMapper(BrandMapper.class);

        //4. 计算开始索引
        int begin = (currentPage - 1) * pageSize;
        // 计算查询条目数
        int size = pageSize;

        // 处理brand条件，模糊表达式
        String brandName = brand.getBrandName();
        if (brandName != null && brandName.length() > 0) {
            brand.setBrandName("%" + brandName + "%");
        }

        String companyName = brand.getCompanyName();
        if (companyName != null && companyName.length() > 0) {
            brand.setCompanyName("%" + companyName + "%");
        }

        //5. 查询当前页数据
        List<Brand> rows = mapper.selectByPageAndCondition(begin, size, brand);

        //6. 查询总记录数
        int totalCount = mapper.selectTotalCountByCondition(brand);

        //7. 封装PageBean对象
        PageBean<Brand> pageBean = new PageBean<>();
        pageBean.setRows(rows);
        pageBean.setTotalCount(totalCount);

        //8. 释放资源
        sqlSession.close();

        return pageBean;
    }
}
```
==注意==：`brandName` 和 `companyName` 属性值到时候需要进行模糊查询，所以前后需要拼接上 `%` 。
### 8.1.3 `servlet` 实现
在 `BrandServlet` 类中定义 `selectByPageAndCondition()`  方法。而该方法的逻辑如下：
* 获取页面提交的 `当前页码` 和 `每页显示条目数` 两个数据。这两个参数是在URL后进行拼接的，格式是  `url?currentPage=1&pageSize=5`。获取这样的参数需要使用 `requet.getparameter()` 方法获取。
* 获取页面提交的 `条件数据` ，并将数据封装到一个 `Brand` 对象中。由于这部分数据到时候是需要以JSON格式进行提交的，所以我们需要通过流获取数据，具体代码如下：
  ```java
  // 获取查询条件对象
  BufferedReader br = request.getReader();
  String params = br.readLine();//json字符串
  
  //转为 Brand
  Brand brand = JSON.parseObject(params, Brand.class);
  ```
* 调用 `service` 的 `selectByPageAndCondition()` 方法进行分页查询的业务逻辑处理
* 将查询到的数据转换为JSON格式的数据
* 响应JSON数据

Servlet中 `selectByPageAndCondition()` 方法代码实现如下：
```java
/**
 * 分页条件查询
 * @param request
 * @param response
 * @throws ServletException
 * @throws IOException
 */
public void selectByPageAndCondition(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    //1. 接收 当前页码 和 每页展示条数    url?currentPage=1&pageSize=5
    String _currentPage = request.getParameter("currentPage");
    String _pageSize = request.getParameter("pageSize");

    int currentPage = Integer.parseInt(_currentPage);
    int pageSize = Integer.parseInt(_pageSize);

    // 获取查询条件对象
    BufferedReader br = request.getReader();
    String params = br.readLine();//json字符串

    //转为 Brand
    Brand brand = JSON.parseObject(params, Brand.class);

    //2. 调用service查询
    PageBean<Brand> pageBean = brandService.selectByPageAndCondition(currentPage,pageSize,brand);

    //2. 转为JSON
    String jsonString = JSON.toJSONString(pageBean);
    //3. 写数据
    response.setContentType("text/json;charset=utf-8");
    response.getWriter().write(jsonString);
}
```
## 8.2 前端实现
前端代码我们从以下几方面实现：
1. **查询表单绑定查询条件对象模型**
   这一步在页面上已经实现了，页面代码如下：
   ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152314856.png)
2. **点击查询按钮查询数据**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302152314106.png)
   从上面页面可以看到给 `查询` 按钮绑定了 `onSubmit()` 函数，而在 `onSubmit()` 函数中只需要调用 `selectAll()` 函数进行条件分页查询。
3. **改进 `selectAll()` 函数**
   子页面加载完成后发送异步请求，需要携带当前页码、每页显示条数、查询条件对象。接下来先对携带的数据进行说明：
   * `当前页码` 和 `每页显示条数` 这两个参数我们会拼接到URL的后面
   * `查询条件对象` 这个参数需要以JSON格式提交给后端程序

   修改 `selectAll()` 函数逻辑为
   ```js
   var _this = this;
   axios({
       method:"post",
       url:"http://localhost:8080/brand-case/brand/selectByPageAndCondition?currentPage="+this.currentPage+"&pageSize="+this.pageSize,
       data:this.brand
   }).then(function (resp) {
       //设置表格数据
       _this.tableData = resp.data.rows; // {rows:[],totalCount:100}
       //设置总记录数
       _this.totalCount = resp.data.totalCount;
   })
   ```
---
# 9. 前端代码优化
咱们已经将所有的功能实现完毕。而针对前端代码中的发送异步请求的代码，如下：
```js
var _this = this;

axios({
    method:"post",
    url:"http://localhost:8080/brand-case/brand/selectByPageAndCondition?currentPage="+this.currentPage+"&pageSize="+this.pageSize,
    data:this.brand
}).then(function (resp) {
    //设置表格数据
    _this.tableData = resp.data.rows; // {rows:[],totalCount:100}
    //设置总记录数
    _this.totalCount = resp.data.totalCount;
})
```
**需要在成功的回调函数**（也就是 `then` 函数中的匿名函数）**中使用 `this`** ，**都需要在外边使用 `_this` 记录一下 `this` 所指向的对象**；因为在外边的 `this` 表示的是Vue对象，而**回调函数中的 `this` 表示的不是Vue对象**。这里我们可以使用ECMAScript6中的新语法（箭头函数）来简化这部分代码（箭头函数中的 `this` 看语义，是代表Vue对象）上面的代码可以简化为：
```js
axios({
    method:"post",
    url:"http://localhost:8080/brand-case/brand/selectByPageAndCondition?currentPage="+this.currentPage+"&pageSize="+this.pageSize,
    data:this.brand
}).then((resp) => {
    //设置表格数据
    this.tableData = resp.data.rows; // {rows:[],totalCount:100}
    //设置总记录数
    this.totalCount = resp.data.totalCount;
})
```
**箭头函数语法**：
```js
(参数) => {
	逻辑代码
}
```
**箭头函数的作用**：替换（简化）匿名函数。

