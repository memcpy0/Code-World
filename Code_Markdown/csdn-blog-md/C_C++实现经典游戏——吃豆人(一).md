@[toc]

## 开始
这是我的选修课小组作业。做一个吃豆人的游戏。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020205601113.png)
> 控制吃豆人吃掉迷宫里面的所有豆子，同时尽可能躲避小鬼怪。一旦吃豆人吃掉能量药丸，它就可以在一定时间内反过来欺负小鬼怪了。特别值得一提的是，迷宫的左右出口是相通的，灵活应用往往带来柳暗花明的奇效。

## 游戏模式
游戏的目的就是**控制游戏的主角小精灵吃掉藏在迷宫内所有的豆子**，**并且不能被幽灵抓到**。

**迷宫的四个角落有大的闪烁点称为大力丸，提供小精灵一小段时间，可以反过来吃掉幽灵**。**幽灵在这段时间内会变成深蓝色**，会往反方向逃逸，而且**通常移动时比较慢**。有趣的是，当幽灵被吃掉时，它的眼睛还在，会飞回鬼屋，并再生而回复正常的颜色。当深蓝色的幽灵反白闪动时，表示大力丸的效力即将消失，而随著游戏的进展，大力丸的有效时间会越来越短。在游戏的后段，大力丸被吃掉时，幽灵不会改变颜色，但仍会往相反方向逃。

奖赏物品：
- 樱桃，100 分
- 草莓，300 分
- 橘子，500 分
- 苹果，700 分
- 葡萄，1000 分
- 旗舰，2000 分
- 球，3000 分
- 钥匙，5000 分

同样，实现的是吃豆人的子集，一个朴素的小游戏。

## 游戏类结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020214846445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020221130953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
这里game.h实现game类，是游戏的调度类。

map是地图类。mover是移动者类，player和monster继承mover。

app.cpp是游戏的运行程序，很简洁，仅调用game.h中的Game类实例就可以类。
```cpp
#include "game.h"

int main()
{
	Game game;
	game.Load();
	return 0;
}
```
define.h是一些游戏常量的头文件，定义了一些通用的值。
```cpp
#pragma once

#define MAP_WIDTH 10  //地图大小
#define MAP_HEIGHT 10

#define DIR_NONE 0  //方向
#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4

#define MONSTER_CHASE_SPEED 0.2 //怪物的移动速度(不同状态)
#define MONSTER_ESCAPE_SPEED 0.05 
#define MONSTER_ESCAPE_STATUS 100

#define MAP_POINT_SIZE 10 //地图点的大小

#define MOVER_SIZE 10 //移动者的大小
```
## 游戏逻辑

game

- game.Load
  - game.Start
  - game.Loop
    - 初始化 wall,bean,goldBean,player,monster
    - 初始化 dir = dir_none
    - while(1)
      - key = GetKey() // 获取按键输入
      - switch(key)
        - p pause
        - q quit
        - w dir_up
        - s dir_down
        - a dir_left
        - d dir_right
        - other dir = last_dir
      - player.PlayerMove(wall,dir) // 玩家根据按键输入移动
        - player.NextMove(dir,nx,ny) //预测下一时刻玩家的坐标
        - 根据nx，ny，判断下一时刻玩家是否撞墙
          - be careful! you must have to ensure nx or ny is at last one is an integer, if not, you can't move according to the input direction
          - ofcourse, you can be a little tolerent, as an example, when the input direction wants to change ny, but nxis not an integer, but , maybe nx is so close to an integer, you can force it converting to an integer, and then change the ny
          - if you obey these rules, you can move the player,
          - player.Move(dir) //调用函数移动玩家
            - clear its image at the old (x,y),
            - then set the new (x,y) according( nx, ny), and draw its image at the new (x,y)
      - if(player.PlayerMove is successful) //如果上一时刻玩家成功移动（没撞墙）
        - player.TryEatBean(bean) // 玩家尝试吃豆子
          - include 2 parts ,first , judge player is or not meet a bean,
          - then, eat the bean and add player's score, and return true
        - if( successful) // 玩家尝试吃豆子成功
          - bean.IsEmpty? && goldBean.IsEmpty?
            - if(true)
              - game over, player win
        - player.TryEatGoldBean(goldBean)// 玩家尝试吃金豆子
          - include 2 parts, just same as above
        - if(successful) // 玩家尝试吃金豆子成功
          - bean.IsEmpty? && goldBean.IsEmpty?
            - if(true)
              - game over, player win
          - change all of monster's status
          - change all of monster's speed
      - monster.TryEatPlayer(player) //怪物尝试吃玩家
        - include 2 part, first, judge monster is or not meet the player,
        - then judge who is win according to monster's status , and (player win)change the player's score or (monster win) game over, player lose
      - monster.MonsterMove(wall,player)
        - according to monster.status, make different choice: chase or escape or waitingBorn
          - dir = chase(wall, player)
            - according to player.GetX & .GetY & wall, choose the move dir
          - dir = escape(wall, player)
            - get contrary dir by chase(wall,player)
          - dir = waitingBorn(wall)
          - monster.Move(dir)
            - clear its image at the old xy, then set the new xy according pre_x, pre_y, and draw its image at the new xy
            - just like what player.Move(dir) has done
          - 恢复地形
  - game.End
