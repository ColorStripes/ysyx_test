# PA3：穿越时空的旅行

## PA3.1：

### 批处理系统

- 整个内容描述的是nanos-lite，讲述特权等级制度，各个ISA架构都有自己规定的，不能混用。

### 穿越时空的旅行

### 应急响应

- 实现ecall指令：isa_raise_intr()：

- 里面也有etrace---->调用的printf经行跟踪

  ![image-20220707125943105](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707125943105.png)

- 实现的关键在于知道ecall到底是怎样调用的：

  1. 发生自陷函数yield（）调用产生自陷

  2. 自陷函数调用存放中断号码到a7（-1），ecall指令执行![image-20220707130934726](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707130934726.png)

  3. ecall调用如下函数，将中断号和epc存储，返回异常处理入口地址，给了dnpc，使得能让pc跳到异常处理函数![image-20220707131505392](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707131505392.png)

     - (8.12日修改)：
     - ![image-20220812162746599](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220812162746599.png)将正确的mcause号传入，可以开启difftest，在用R（17)分发，共分发两次
  
  4. 而对于异常入口地址是怎么来的呢？
  
     其实，早在HAS_CET的那条初始化函数，就已经将异常处理的汇编程序的首条指令地址给了mtvec：
  
     main.c的
  
     ![image-20220707132253375](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707132253375.png)
  
     ![image-20220707132317167](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707132317167.png)
  
     调用cte_init![image-20220707132434670](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707132434670.png)

![image-20220707132529946](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707132529946.png)

将汇编地址读入，完成异常入口地址的初始化

5. 而要运行difftes则需要mstatus初始化为`0xa00001800`，但有个问题是我们返回的a7中断号的-1，但标准的中断号应该是11,。是因为拿到的自陷等级不够，但是sqike的等级一直是机器级，所以可以拿到11。这里我们就不在管difftest，进行下边的操作。

   ![image-20221003165536547](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221003165536547.png)
   
   改：
   
   ![image-20220930002542513](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220930002542513.png)

### 保存上下文

- 上下文的保存主要通过异常处理函数执行。（也就是那段汇编指令）

- ![image-20220707133532452](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707133532452.png)

  压栈就保存了上下文（压入了除$0，sp寄存器的所有gpr，和3个特殊的csr）（Sp寄存器用来指示汇编程序的压栈位置，不用保存，$0一直为0，和地址空间共用位置，也不用保存。）

- ![image-20220707153422280](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707153422280.png)

这里这个必答题有意思，作为学C语言单片机的我来说，这压根不知道啥东西。然而我斜对面坐着位北大的学姐（露姐yyds），却给我讲通了这部分内容，然后惊讶的发现我为什么不是学汇编单片机的学生。下面我记录一下这个必答题的过程：

1. 地址空间信息：就是把Context结构体成员的顺序弄对，否则P4将难以进行（PA3未用到）。

2. 有没有发现刚刚压完栈，就跳到了_am_irq_handle，但我们打开am_irq_handle函数去看，![image-20220707153911159](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707153911159.png)这里好像有个参数吧？但是我函数调用直接跳转了，哪里有参数传入？

   其实参数的传入用汇编写好了，![image-20220707154129168](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707154129168.png)也就是这条mv指令。因为架构规定，sp是栈指针，a0-a7保存参数，但是只有一个指针c传入，所以只用到a0寄存器。这样就完成了函数调用。a0保存的是栈顶位置，也就是（负CONTEXT_SIZE)的位置。

![img](file:///D:\QQ\1401724065\Image\C2C\BDED6B2353B45170BAD0A22D45F16DE6.jpg)

3.所以，最后Context* c做为参数，也就是a0，（a0保存的是sp）sp就是栈顶地址，所以Context* c就指向sp所指空间，完成c的赋值。

4.Context结构体的顺序就是如图所示顺序（压栈顺序，压栈的时候sp没有变）

![image-20220707160432059](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707160432059.png)

### 事件分发

事件分发就非常好弄了吧（注意有两次打包事件，一次mcause打包，一次R（17）打包事件，然后进行分发）

![image-20220812163206176](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220812163206176.png)

![image-20220707160728688](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707160728688.png)

### 恢复上下文

![image-20220707160855082](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707160855082.png)

将存储的东西依次拿出来，然后sp恢复到开栈之前的位置，执行mret函数，记得mret要pc+4哦，因为mepc保存的是ecall地址，如果不加四，则一直执行ecall->trap.s->ecall->trap.s死循环。

### Etrace

因为硬件一定执行，所以此处的硬件也一定会模拟完毕，在这里调用的printf将会不受异常事件限制。

![image-20220707161327098](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707161327098.png)



## PA3.2：

### 加载第一个用户程序

- 这个主要就是加载将镜像文件的所有”段“进行判断。具体怎么索引到所有的段，请看
- http://www.peckerwood.top/post/elf0x02/，
- https://paper.seebug.org/papers/Archive/refs/elf/Understanding_ELF.pdf

我的方法：![image-20220707163538664](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707163538664.png)

注意：![image-20220707163102895](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707163102895.png)这里的“魔术”，读取文件的魔术师反的，因为小端问题

### 操作系统的运行时环境

- 系统调用

  系统调用的trace开关在这里![image-20220707213803670](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707213803670.png)因为这个调用的trace的太底层了，在nemu无法实现，所以在os中实现

  系统调用参数寄存器（riscv64）：

  ![image-20220707214257988](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220707214257988.png)

  

### 操作系统之上的TRM

- 在理解了systemcall的调用后，从apps的_write中调用系统调用端口，传入合适的参数，然后重新编译hello.c.重新移植到os中。![image-20220708103408730](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708103408730.png)

- 然后再do_systemcall中调用write（）（自己写）然后打出一句话，还有一堆HHHHHHH（这个我还不知道为啥，但是大佬们说不用管，后面堆区管理就可以解决（希望吧））

- ![image-20220708104027506](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708104027506.png)

- 

  ![image-20220708152059151](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708152059151.png)

  这里面有寄存器的分配规定，为什么用a0-a6进行传参

  

  #### 堆区管理

  - 堆区管理主要在![image-20220708150259561](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708150259561.png)

  实现完成就好，实现之后printf就可以打出我们想要的字符串了

## PA3.3

### 简易文件系统

- 写在最前面：![image-20220708215017647](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708215017647.png)
- 首先在Finfo结构体中添加新的成员，用来指向打开文件后的指向偏移![image-20220708215254317](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708215254317.png)
- 由于并不是所有文件都用名字，我们希望可以打开没名字的文件，用文件标识符fd表示文件，所以对文件操作都是对fd指向的文件的操作

- Finof结构体数组，用来加载文件描述表，数组的前三项是已规定的，不能更改，后面的下标号可以自行分配![image-20220708215524571](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708215524571.png)

- 实现所需函数并不难，和系统调用相关，因为文件操作需要系统来协调

  ![image-20220708215833211](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708215833211.png)

- 函数的实现和write，yeild系统调用一样，关键是看手册读懂每个函数究竟是在干什么，以及返回什么样的值。

- 更改loader，向native_uload（NULL,filename）函数，传入要载入文件的地址

  修改loader，（/////////////////////为之前的），loader的实现用了lseek进行移位![image-20220708220431119](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708220431119.png)

- 修改makefile参数![image-20220708220822948](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708220822948.png)添加用户程序

- strace将GPR1变量直接返回为字符串，打印打开的文件信息![image-20220708221007361](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220708221007361.png)

  

#### 一切皆文件



#### 虚拟文件系统

- 修改fs_write，         修改文件描述表的函数指针，为serial_write

  ![image-20220709112912586](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220709112912586.png)

##### IOE

- 时钟

  - 在timer-test测试的makefile中加入参数LIBS=libndl让其链接ndl库。

  - 测试主函数加入NDL.h头文件。

  - 记得使用NDL库首先初始化，获取系统启动时间。

  - 用NDL封装的gettimofday

    ![image-20220714100034077](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714100034077.png)

  - gettimeofday内部，需要定义#include <sys/time.h>获取结构体信息

    ![image-20220714100328311](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714100328311.png)

- NDL_PollEvent：![image-20220714100939999](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714100939999.png)

- 与之对应的event_read：![image-20220714101128474](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714101128474.png)

- ![image-20220714101237147](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714101237147.png)

  [(35条消息) fopen与open的区别_$&.Dream Flying��的博客-CSDN博客_fopen open](https://blog.csdn.net/weixin_47731862/article/details/112273651)

  一个是系统调用，一个是c语言封装的调用

- 键盘直接io_read读一次取所有信息，如果分开读取会发生只up不down的信息错误

  ![image-20220709222456049](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220709222456049.png)

  

- VGA

  - 画布是贴在屏幕上的，画布贴在哪，由NDL_DrawRect中的write决定，write写入dev/fb的位置就是画布贴在屏幕的位置
  - 写入屏幕大小![image-20220714101833634](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714101833634.png)
  - 测试问题：
    - 读readme，把N改为PPT张数
    - 报错1：![image-20220714104836245](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714104836245.png)安装concert库
    - 报错2：linux转化安全等级改变，点击最后的网址，按提示注释文件，改不了文件请777[(35条消息) chmod777到底是什么意思_唐宋xy的博客-CSDN博客_chmod777](https://blog.csdn.net/weixin_42054155/article/details/113408572)![image-20220714105321024](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714105321024.png)
    - 
  
  居中操作：（7.11日的第一版有误，当前测试通过，SDL测试未通过，居中请看后文）
  
  ![image-20220711180714496](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220711180714496.png)
  
  

#### 精彩纷呈的应用程序

##### 定点算术

![image-20220714102638958](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714102638958.png)

![image-20220714102702937](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714102702937.png)

![image-20220714102718561](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714102718561.png)

##### Navy中的应用程序

- 应用程序需加入到169行的APP编译选项![image-20220714103300021](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714103300021.png)
- 具体SDL库怎么封装的，可以看手册描述（WIKI或者[SDL_Color (libsdl.org)](http://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlcolor.html)）
- 这里的居中信息更改，改为依据画布信息居中，以前是更具更新面积居中![image-20220714103910880](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714103910880.png)

- 其他SDL封装信息github中详查

###### PAL

- 如果你不是南大学生如何获得仙剑？那我们可以。。。。。直接访问南大PA页面，嘿嘿嘿http://jyywiki.cn/ICS/2021/labs/PA3

- apt-fill

- 颜色获得方式改变，通过索引拼接方式，拼出32位颜色信息![image-20220714110929373](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714110929373.png)

- 上面的index索引bug计算卡了我很久，一旦计算不对，能正常运行，但文字的黑色的

- 申请的malloc临时像素空间，一定要free，不然运行一半就会地址越界。

- 出现flbu指令，浮点指令很可能是有函数未实现，比如这个![image-20220714111523531](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714111523531.png)

  未实现就不能运行仙剑

- i值判断有误（一按键盘就报错），因为没写入‘\n’![image-20220714111642008](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714111642008.png)

  ![image-20220714111730422](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714111730422.png)

  调用反汇编riscv64-linux-gnu-objdump -d pal-riscv64 > a.txt，一条一条trace找到bug

- 最后就是居中问题，居中使得地址直接越界，已修改居中，已画布为参数居中

- 最后关闭所有trce，运行仙剑![image-20220714112157735](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714112157735.png)



##### 批处理系统

- 啥也不多说了直接看man吧，只要理解好这些函数就可以写出来[exec函数 (360doc.com)](http://www.360doc.com/content/11/0118/18/5531585_87429340.shtml)

![image-20220714185101040](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20220714185101040.png)

