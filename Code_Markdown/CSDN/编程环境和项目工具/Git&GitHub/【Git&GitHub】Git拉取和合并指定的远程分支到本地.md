假设我们在远程仓库上，在某个分支上新建了一个远程分支。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310171420831.png)
可以在 Gitlab 页面上看到该分支：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310171421655.png)
但是 `git branch -r` 列出远程分支或 `git branch -a` **列出全部分支**，却看不到该远程分支。这是为什么呢？这个问题我们放到最后。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310171422655.png)
现在我们直接以该远程分支为源新建一个同名本地分支。发现报错了，这又是为什么呢？这个问题我们也放到最后。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310171422034.png)

实际上，这两个问题都是一个问题。先说说正确的步骤
1. 先 `git fetch origin 远程分支名字` 拉取该远程分支到本地
    ![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310171424919.png)
    此时 `git branch -r` 或 `git branch -a` 就可以看到该远程分支了：
    ![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310171425371.png)
2. 在本地创建一个同名的新分支，并切换到该新分支
   ```bash
   $ git checkout -b 本地新分支（与远程分支同名） origin/远程分支 // 等同于git checkout -b 远程分支
   ```
     提示 `set up to track remote branch` ，说明新建的本地分支与同名远程分支已经建立追踪关系。
3. 此时再 `git branch -a` ，就可以看到该远程分支对应的同名本地分支了：
    ![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310171429496.png)  

综上所述，推断出两点：
- 只有在 `git fetch` 之后，`git branch -r` 与 `git branch -a` 才能展示出远程仓库中新建的远程分支，可见 `git branch -r`并不会自动发送请求、去获取远程最新的分支情况
- 当我们使用 `git checkout -b 本地新分支(与远程分支同名) origin/远程分支` 时，**这个 `origin/` 后面跟着的所谓远程分支，必须是 `git branch -r` 所展示的远程分支**，不能仅以Gitlab页面上有这个远程分支名为准。