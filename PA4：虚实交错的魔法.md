# PA4：虚实交错的魔法：分时多任务

## 多道程序

### 上下文切换

- 这里需要知道几个概念：

  - **sp**：栈指针，指示上下文中的内容
  - **context pointer（cp）**：这个是进程的指针
  - **__am_irq_handle()**：进入中断处理的trap.S的汇编程序，保存上下文，进入这个事件打包程序。所传入的上下文不同，则直接决定了之后处理的程序内容。所以说它是上下文切换的入口。

- PA3遗留问题：Context的由来，也就是压栈的顺序。[栈指针&& 帧指针详解 - Hugh_Sun - 博客园 (cnblogs.com)](https://www.cnblogs.com/samo/articles/3092895.html)

  完成这个kcontext这个函数，需要了解如何指向一个Context的开头，内核线程的创建所创建的**context_make**上下文结构模拟的是保存完*上下文内容*的*上下文结构*

  ![image-20221003230810166](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003230810166.png)
  
  ![image-20221004210809558](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221004210809558.png)

#### 线程/进程调度

- **schedule()**的内容：仅仅是返回当前PCB掌控的context pointer，也就是所切换到的上下文Context的开头

```c
// save the context pointer
current->cp = prev;

// always select pcb[0] as the new process
current = &pcb[0];

// then return the new context
return current->cp;
```

![image-20221003235030465](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003235030465.png)

#### 内核线程

- kcontext的实现：

  - 因为模拟的是开栈后的结构，所以局部变量的Context指针指向的是开栈后的位置，也就是Context这个结构体的开头

  ![image-20221003231407870](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003231407870.png)

- context_kload的实现：
  - 因为和**naive_uload**函数同级，所以放在了loader.c下面。
  - **PCB结构体**：![image-20221003232647796](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003232647796.png)

- **AddrSpace结构体：**（我们需要用到里面的Area，因为<u>kstack</u>参数）

  ![image-20221003232830815](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003232830815.png)

- PCB里有一个**stack**数组，数量是32K个，一个是1B，所以大小是32KB![image-20221003233650055](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003233650055.png)

- **context_kload(PCB *pcb, void (*entry)(void *), void *arg)**函数：

  - 参数1：进程控制结构体
  - 参数2：所要进入的内核线程函数
  - 函数传递arg参数

  实现：创建人工上下文，保存到上下文指针

  ![image-20221003234519206](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003234519206.png)

![image-20221003234612304](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221003234612304.png)



##### 现在让我们整理一下流程：

​	主函数运行过程：

![image-20221004211318980](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004211318980.png)

​	1.内核线程创建过程：

​	![image-20221004212434862](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004212434862.png)

![image-20221004212959719](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004212959719.png)

![image-20221004213518208](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004213518208.png)

![image-20221004213807222](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004213807222.png)

2. 内核自陷，进程调度过程：

   - 触发内核自陷

     ![image-20221004214049803](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004214049803.png)

   - 进行PA3的一些列系统调用过程：

     **（ **  ecall触发  **--->** 根据异常处理入口trap.S  **--->**  压栈保存上下文后，跳入 __am_irq_handle  **--->** 

     ​      事件打包为EVENT_YIELD事件 **--->** do_event事件处理 **--->** 识别为EVENT_YIELD事件 **--->** 进行进程调度 **--->**

     ​	  返回pcb选中的进程的Context指针 **--->**  **）**

   所以，这个返回的Context结构体在哪里？

   ![image-20221004223423625](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004223423625.png)

   **所以我们来看调用约定：**

   ![image-20221004224015914](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004224015914.png)

   **关键一步：**

   ![image-20221004224549869](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004224549869.png)

#### 内核线程参数

- 这个问题其实在PA3就遇到过，通过调用约定，将参数传入

  ![image-20221004000054602](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004000054602.png)

![image-20221004225140413](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221004225140413.png)

- 所以往上下文机构中a0寄存器传入参数

  ![image-20221004114952791](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221004114952791.png)



#### 用户进程

- 完成用户进程创建函数：

  - 开展，设置内核栈范围，用于保存上下文
  - loader（）加载用户程序，找到入口
  - 设置栈顶指针位置为 `heap.end`

  ![image-20221005200708739](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221005200708739.png)

- 用户进程创建内部函数（目前我们忽略 *内存范围as参数* ）

  ![image-20221005201314986](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221005201314986.png)

- 通过 crt0中的 **_strat** ，和约定的 **GPRx寄存器** 将栈顶指针 返回给 **SP** 寄存器

  这里是用户程序的开始，**_start** 是用户程序的开始

  ![image-20221005201708290](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221005201708290.png)



#### 用户进程参数

- 用户栈结构：

  ![image-20221006013824443](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221006013824443.png)

- 按照这个结构创建用户栈

  **分为四步**：

  1.  开栈
  2.  加载用户进程参数
      - 按照上图的结构加载参数。这些参数分成两部分, 一部分是字符串区域(string area), 另一部分是`argv/envp`这两个字符串指针数组, 数组中的每一个元素是一个字符串指针, 而这些字符串指针都会指向字符串区域中的某个字符串。
      - 最后将**栈顶的位置**指向 **argc** 的位置
  3.  加载用户程序
      - 跟之前一样，只不过需要把栈底的位置设为**argc** 的位置，**Context** 结构在 **argc** 之后
  4.  最后将**cp->GPRx**指向 **当前栈顶** 的位置

  ```c
  void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[])
  {
    printf("IN uload: load: %s\n", filename);
  
    //// 1.open the kernel stack
    pcb->as.area.start = (void *)pcb->stack;
    pcb->as.area.end = pcb->as.area.start + STACK_SIZE;
  
  
    //// 2. args loading
    uintptr_t argc = 0, envc = 0;
    while (argv && argv[argc])
      argc++;                                           // the number of argc
    while (envp && envp[envc])
      envc++;                                           // the number of envp
  
    void *current_sp = pcb->as.area.end - 1;            // user stack end
    //push the stack
    // a. Unspecified
    int Unspecified_1 = sizeof(uintptr_t);
    current_sp -= Unspecified_1;
    // b. string area
    uintptr_t arg_pointer[argc];
    for (int i = 0; i < argc; i++)
    {
      current_sp -= (strlen(argv[i]) + 1);           //+1 for '\0'
      arg_pointer[i] = (uintptr_t)current_sp;
      strcpy(current_sp, argv[i]);                   // strcpy is +sp, so first sp is current_sp - strlen
    }
    uintptr_t env_pointer[envc];
    for (int j = 0; j < envc; j++)
    {
      current_sp -= (strlen(envp[j]) + 1);
      env_pointer[j] = (uintptr_t)current_sp;
      strcpy(current_sp, envp[j]);                   // strcpy is +sp, so first sp is current_sp - strlen
    }
    // c. Unspecified
    int Unspecified_2 = sizeof(uintptr_t);
    current_sp -= Unspecified_2;
    // d. NULL
    current_sp -= 1;
    // e. pointer of the envp                                      //opposite order
    for (int j = envc - 1; j >= 0; j--)
    {
      current_sp -= sizeof(uintptr_t *);
      *(uintptr_t *)current_sp = env_pointer[j];
    }
    // f. NULL
    current_sp -= 1;
    // g. pointer of the argv                                      //opposite order
    for (int i = argc - 1; i >= 0; i--)
    {
      current_sp -= sizeof(uintptr_t *);
      *(uintptr_t *)current_sp = arg_pointer[i];
    }
    // h. argc
    current_sp -= sizeof(uintptr_t);
    *(uintptr_t *)current_sp = argc;
  
  
    //// 3.load the user program
    pcb->as.area.end = current_sp;                              //stack end can't cover the args' stack
    uintptr_t entry = loader(pcb, filename);
    pcb->cp = ucontext(&pcb->as, pcb->as.area, (void *)entry);
  
  
    //// 4.set the stack top with argc's pointer
    pcb->cp->GPRx = (uintptr_t)current_sp;
  
  }
  ```



- 修改`cell_main`进行测试：

  ![image-20221006015301800](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221006015301800.png)

- `context_kload`传参参数设置

  ![image-20221006214053479](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221006214053479.png)

- 修改PAL代码

  ![image-20221006020132072](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221006020132072.png)



##### 实现带参数的execve

- syscall.c中的修改

  **context_uload()**中的PCB传入的是当前进程的PCB，加载B的参数时用的也是A的用户栈，所以以A的进程创建B的用户程序。

  在**switch_boot_pcb()**之后结束A的执行流，yield强制切换到B的进程。

  ![image-20221006221301039](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221006221301039.png)![image-20221006220954453](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221006220954453.png)

- 用户栈的最大栈顶指针位置，不应与A的用户栈内容重合，所以在堆区为B开辟空间，作为B的用户栈

  ![image-20221006221621070](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221006221621070.png)

- 在**mm.c**中实现堆区开辟

  ![image-20221006222025641](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221006222025641.png)

- 测试：程序自己调用自己

  ![image-20221006222158560](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221006222158560.png)

##### 运行Busybox

- 配置busybox，在busybox文件夹下

  ```shell
  make ISA=riscv64 menuconfig
  ```

- Busybox通过软连接将各个shell指令工具对应的ELF连接，执行对应的elf相当于执行Busybox

- 检查给定路径文件是否能执行

  ![image-20221007102209880](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221007102209880.png)

- 修改nterm中的内建shell

  ![image-20221007102434434](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221007102434434.png)

​		`excevp（）`将会通过`setenv`设置的PATH进行遍历，查找可执行文件：[execvp - 搜狗百科 (sogou.com)](https://baike.sogou.com/v11039289.htm?fromTitle=execvp)

​		`execl（）`只会执行所给绝对路径的文件，不会遍历PATH



## 超越容量的界限

### SV39分页机制

- 参考连接：https://zhuanlan.zhihu.com/p/263372436

  ​					http://rcore-os.cn/rCore-Tutorial-Book-v3/chapter4/3sv39-implementation-1.html

  ​					https://learningos.github.io/rust-based-os-comp2022/chapter4/3sv39-implementation-1.html

**SATP寄存器**：

![image-20221008205838656](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221008205838656.png)

![image-20221008210002164](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221008210002164.png)

**MODE域**：![image-20221008211138256](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221008211138256.png)

![image-20221008210040555](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221008210040555.png)

**页表项**：

![image-20221008210122438](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221008210122438.png)

![image-20221008211100370](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221008211100370.png)

![image-20221008213029535](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221008213029535.png)





### 将虚存管理抽象成VME

#### 在分页机制上运行Nanos-lite

- 核心函数 `map（）`：
  - **va,** **pa**都是按页对齐的地址，及低12位为0

```c
void map(AddrSpace *as, void *va, void *pa, int prot) {
  assert(as->ptr);

  uint64_t virtual_add = (uint64_t)va;                   //39 bits virtual address
  uint32_t first_vindx = (virtual_add >> 30) & 0x1ff;    //virtual_add[39 : 30]
  uint64_t * first_vpage = (uint64_t*)as->ptr;           //the start virtual page content(64 bits)
  

  if(!(first_vpage[first_vindx] & 0x1)){                 //invalid in talbe item
    void * new_ppage = pgalloc_usr(PGSIZE);              //apply the new physical page
    //the [55 : 11] of physical page address + 10 bits flags | 1 bits valid
    first_vpage[first_vindx] =  (((uint64_t)new_ppage & 0xfffffffffff000) >> 2) | VALID_MASK;       
  }

  uint32_t second_vindx = (virtual_add >> 21) & 0x1ff;    //virtual_add[29 : 21]
  uint64_t * second_vpage = (uint64_t*)((first_vpage[first_vindx] & PGTABLE_MASK) << 2);            //the address of the second virtual page content(64 bits)
  
  if(!(second_vpage[second_vindx] & 0x1)){                 //invalid in talbe item
    void * new_ppage = pgalloc_usr(PGSIZE);                //apply the new physical page
    second_vpage[second_vindx] = (((uint64_t)new_ppage & 0xfffffffffff000) >> 2) | VALID_MASK; 
  }

  uint32_t third_vindx = (virtual_add >> 12) & 0x1ff;     //virtual_add[20 : 12]
  uint64_t * third_vpage = (uint64_t*)((second_vpage[second_vindx] << 2) & PGTABLE_MASK);           //the address of the second virtual page content(64 bits)
  
  if(!(third_vpage[third_vindx] & 0x1)){                  //invalid in talbe item
    //void * new_ppage = pgalloc_usr(PGSIZE);             //apply the new physical page
    third_vpage[third_vindx] = (((uint64_t)pa & 0xfffffffffff000) >> 2) | VALID_MASK;               //the address of the third virtual page is pa
  }

}
```

- `map`的主要功能是将传入的虚拟地址与物理地址对应：

  - 由**va**虚拟地址确定一级页表索引，as确定页表地址，表项中填充由 **pgalloc_usr(PGSIZE)** 申请来的物理页表地址

    ![image-20221008215110944](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221008215110944.png)

  - 一级物理页表表项中，填充下一级物理页表地址，给标志位为0，表示指向下一页表

  - 第三级页表表项，填充传来的**pa**真实物理地址 也就是 **va** 对应的物理地址

- **0xfffffffffff000**是取第13到56位

- **PGTABLE_MASK** = 0x3ffffffffffc00 是取第11到第54位

- 每个物理页地址都是56位的，并且是**低12位为0的对齐**地址（页表对齐）

- 最后对应的物理地址，低12位填充offset

  ![../_images/sv39-full.png](http://rcore-os.cn/rCore-Tutorial-Book-v3/_images/sv39-full.png)

在nemu中增加对分页机制的支持：

- 如何判断CPU当前是否处于分页模式?          ----->    **satp**寄存器  

  添加新的CSR寄存器（S级），SATP寄存器让NEMU知道开启了分页机制

  ![image-20221010162859236](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221010162859236.png)

- 分页地址转换的具体过程应该如何实现?	   ---->     两个抽象的 mmu **API** 

  - 根据当前的系统状态判断一次虚拟地址的访问应该如何进行![image-20221010163006817](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010163006817.png)

  - 虚拟地址转化函数

    - 基本思想与**map**类似，但是读取页表项的时候只能用**paddr_read()**,因为NEMU中读取物理地址内容也需要相应规则转化，不能直接使用root_page[item]来获取页表项，这样会发生段错误。
    - 函数最后返回，三级页表项对应的物理地址

  ![image-20221010163937826](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010163937826.png)

  

- 对于虚拟地址读写函数的实现

  通过check检查是否需要地址转换，如果需要则转换地址，然后再进行物理地址读取

  ![image-20221008215640978](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221008215640978.png)





#### 在分页机制上运行用户进程

- 引入虚拟地址空间的概念，每个开辟的虚拟地址都对应物理地址。  虚拟地址空间（0x40000000 ----- 0x80000000）

- 在make update的时候加入**VME=1**的参数，使应用连接到**0x40000000**的虚拟地址空间，避免与内核进程（**0x83000000**附近）重叠

  

- `context_uload()`中的修改

  ![image-20221010170748913](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010170748913.png)

​		使用用户栈物理地址压入参数

![image-20221010170942590](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010170942590.png)

​		返回用户栈顶指针的物理地址

​							![image-20221010171259931](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010171259931.png)	



- `loader（）`函数的修改

  - 注意这里是按页加载，由于map实现的时候va，pa都只能传入页对齐地址，所以对**vaddr**和**paddr**做如下处理

  - ```c
    #define PGSIZE 4096
    #define PG_OFFSET 0xfff
    #define PG_BEGIN(addr) ((void*)((uintptr_t)addr & (~PG_OFFSET)))   //返回addr对应的页对齐页首地址
    #define PG_END(addr) ((void*)(PG_BEGIN(addr) + PGSIZE))			   //返回addr对应的页对齐页末地址
    //返回由虚拟地址获得的偏移量，得到的一物理页中的页表项地址位置
    #define PADDR_FROM_VADDR(paddr,vaddr) ((void*)((uintptr_t)paddr | ((uintptr_t)(vaddr) & PG_OFFSET)))
    #define min(a,b) ((uintptr_t)(a) < (uintptr_t)(b)? (a):(b))		   //返回最小一项
    ```

    

![image-20221010172019514](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010172019514.png)

- 说明：

  - 如果首个vaddr不是页对齐的，从偏移量开始填充，一页的上半部分是空的

    ![image-20221010173802159](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010173802159.png)

  - 如果，一页填充的结束不是页对齐地址

    ![image-20221010174421343](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010174421343.png)

  - 如果，一页清空结束位置不是整页对齐，则

    ![image-20221010174752398](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010174752398.png)



- `ucontext（）`中的修改

  加入地址空间描述符，每个进程维护自己的地址空间

  ![image-20221010013931468](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010013931468.png)

  ![image-20221010175003056](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010175003056.png)

  

- `__am_irq_handle()`中的修改

  - 在`__am_irq_handle()`的开头调用`__am_get_cur_as()` (在`abstract-machine/am/src/$ISA/nemu/vme.c`中定义), 来将当前的地址空间描述符指针保存到上下文中
  - 在`__am_irq_handle()`返回前调用`__am_switch()` (在`abstract-machine/am/src/$ISA/nemu/vme.c`中定义)来切换地址空间, 将被调度进程的地址空间落实到MMU中

![image-20221010175328725](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010175328725.png)



#### 在分页机上运行仙剑奇侠传

- 让开辟的堆区空间映射到物理地址中，所以实现`mm_brk()`函数，实现堆区映射

  - 这是系统调用函数，**SYS_brk**系统调用

    ![image-20221010180508165](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221010180508165.png)

  - **max_brk**标记着下一物理地址页的页首位置

    ![image-20221010180836099](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010180836099.png)

  - 如果**brk**的位置大于了新页首位置，则重新开辟一个新的物理页，并映射该地址空间到新开辟的物理页

  ![image-20221010180009514](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010180009514.png)

  

##### 我快被自己蠢哭了 —— 致命BUG

<u>BUG描述：</u>

在实现完用户进程之后，可以顺利启动**dummy**程序，所以在分页机制用户进程实现的函数都没有问题，但是在实现**mm_brk**后，仙剑奇侠传怎么都无法启动，**isa_mmu_translate**总是报缺页错误，而且缺页位置很是奇怪，有时候在**malloc**函数中，有时候在**__am_irq_handle**中并不固定。

<u>排除BUG：</u>

- 由于是缺页bug，所以我一度怀疑是 虚拟地址转化函数 有问题，但是经过几次尝试，发现用户程序可以正常运行一段时间，其中也经过了地址转化函数，所以排除了这个地方。

- 然后我打印了我创建了开辟的 用户栈的虚拟用户地址空间，发现每次越界位置里离用户栈不远，而且这里出错位置都是压栈和开栈的问题，明显是程序使用的栈越界，但是讲义上不是说32KB的用户栈是足够使用的嘛？而且之前运行仙剑是够用的，所以我怀疑是创建用户进程的时候，返回的栈顶指针位置不对，也曾经认为用户栈的压栈用的是虚拟地址，但根本不可能，直接bound了，看来只有用户进程才能使用虚拟地址。改了一下返回的用户栈顶指针为虚拟地址**pcb->as.area.end**，结果发现缺页位置在**pcb->as.area.end**附近（我* * * ）。

  ![image-20221010182851839](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010182851839.png)

- 既然是用户栈越界，那用户程序开辟地址空间只有**mm_brk**才能做到，所以返回到**mm_brk**函数发现了**system_call handler**的备注，发现自己根本没有调用这个函数（找到，漏洞1），但是修复了**SYS_brk**后依然是缺页（我？？？？）。What？难道还有问题？

- 难道是Context有问题，打印了**__am_irq_handle**发现返回次数不止一次，而且每次c也是不一样的，说明上下文正常切换了。既然创建用户进程没问题，切换用户进程也没问题，但切换后用户栈改变了，只可能在系统调用中出现了问题，所以我打印了系统调用号，发现在某次系统调用号为9（正好是SYS_brk）后，直接越界。

  ![image-20221010190238668](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010190238668.png)

- 所以返回**_sbrk**函数观察，好像没有返回值，并且我之前设置的打印堆顶指针位置的**printf**也没有打印。

  ![image-20221010023601304](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010023601304.png)

- 等等。。。。**printf**？？？，为什么会出现**printf**，我好像知道为什么会用户栈越界了。。。。所以把**printf**打印内容增多后，反发现更少的系统调用嵌套后，地址缺页。（get bug），去掉**printf**后pal正常运行。

  ![image-20221010191150681](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010191150681.png)

- 为什么会这样？？？ 在多次系统调用嵌套，保存上下文需要消耗用户栈，用户栈快满的时候，调用**SYS_brk**进行开栈，结果还没等9号系统调用返回（开栈未成功，空间还没开辟），在9号系统调用中插入**printf**也会消耗用户栈保存要打印的数据，同时也会调用系统调用，又要保存上下文，还没开出用户栈空间，就消耗用户栈，这样用户栈就出界了。这样是在系统调用中不能调用**printf**的原因

  ![image-20221010191959961](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221010191959961.png)



### 支持虚存管理的多道程序

AM创建的所有虚拟地址空间都会包含内核映射, 无论在切换之前是位于哪一个虚拟地址空间, 内核线程都可以在这个虚拟地址空间上正确运行. 因此我们只要在`kcontext()`中将上下文的地址空间描述符指针设置为`NULL`, 来进行特殊的标记。

![image-20221011115203121](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221011115203121.png)



## 分时多任务

- 在cpu结构体中添加一个`bool`成员`INTR`.

  ![image-20221012232318608](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012232318608.png)

- 在`dev_raise_intr()`中将INTR引脚设置为高电平.

  ![image-20221012232203629](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012232203629.png)

- 在`cpu_exec()`中for循环的末尾添加轮询INTR引脚的代码, 每次执行完一条指令就查看是否有硬件中断到来:

  ![image-20221012233128720](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012233128720.png)

- 添加中断事件识别与处理

  - 千万不要pc+4，因为这里是中断，中断的那条指令还没执行

  ![image-20221012233258840](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012233258840.png)

  ![image-20221012233501093](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012233501093.png)

- 更改用户进程，内核线程创建的上下文

  ![image-20221012233721271](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012233721271.png)

![image-20221012233802417](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012233802417.png)



- 奇葩的bug：

![image-20221012011832123](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221012011832123.png)

原因：返回的是之前的上下文，导致**hello_fun**也会进行**system call**

![image-20221012231806105](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221012231806105.png)





## 内核栈和用户栈

- trap.S

![image-20221013171123163](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221013171123163.png)

![image-20221013012220046](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20221013012220046.png)

```c
__am_asm_trap:

  csrrw sp, mscratch, sp   # (1) atomically exchange sp and mscratch
  bnez sp, np_user         # (2) take the branch if we trapped from user
  csrr sp, mscratch        # (3) if we trapped from kernel, restore the original sp
  
  # now sp is pointing to the kernel stack
  # save the context...

  #save GPR
  addi sp, sp, -CONTEXT_SIZE
  MAP(REGS, PUSH)
  li t3, 0
  j save_context


np_user:

  #save GPR
  addi sp, sp, -CONTEXT_SIZE
  MAP(REGS, PUSH)
  li t3, 1
  j save_context



save_context:

  #c->sp = $sp
  csrr t0, mscratch
  STORE t0, OFFSET_SP(sp)

  # ksp = 0
  csrwi mscratch, 0
  
  csrr t0, mcause
  csrr t1, mstatus
  csrr t2, mepc

  STORE t0, OFFSET_CAUSE(sp)
  STORE t1, OFFSET_STATUS(sp)
  STORE t2, OFFSET_EPC(sp)
  STORE t3, OFFSET_NP(sp)


  # set mstatus.MPRV to pass difftest
  li a0, (1 << 17)
  or t1, t1, a0
  csrw mstatus, t1

  mv a0, sp
  jal __am_irq_handle

  mv sp, a0                       #set the new context's stack top

  LOAD t1, OFFSET_STATUS(sp)
  LOAD t2, OFFSET_EPC(sp)
  LOAD t3, OFFSET_NP(sp)
  csrw mstatus, t1
  csrw mepc, t2

  bnez t3, recover_user

  MAP(REGS, POP)
  addi sp, sp, CONTEXT_SIZE    

  mret


recover_user:

  MAP(REGS, POP)

  addi sp, sp, CONTEXT_SIZE
  csrw mscratch, sp

  addi sp, sp, -CONTEXT_SIZE
  LOAD sp, OFFSET_SP(sp)

  mret

```

- 因为内核进程创建的时候没有设置sp指针，所以没有最后返回用户态那样  LOAD sp, OFFSET_SP(sp) ，而是以恢复上下文时传入的a[0]寄存器所指示的位置为准。a[0]寄存器所指示的Context c，因为Context和内核线程参数arg开在内核栈，所以通过a[0]寄存器就能找到内核栈开Context结构体之前和之后的位置。
- 而用户态之所以要LOAD sp, OFFSET_SP(sp) ，是因为用户态的Context c是开在内核栈上，a[0]依然是指向Context c的位置，也就是内核栈的位置，但恢复上下文之后，栈指针要指向用户栈。而用户进程让上下文中的sp一直指向堆区位置也就是我们所认为的用户栈，所以通过LOAD能恢复栈指针指向用户栈。

- 在上下文添加 np 变量，指示从上下文返回的 状态（内核态或用户态）

  并且在ucontext和kcontext设置正确的返回态

![image-20221014201655332](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221014201655332.png)

- 分离内核栈与用户栈

  uload压入参数时，消耗的是用new_page()开辟的用户栈，而在调用**ucontext**时调用的是**pcb**的内核栈，保证上下文保存在内核栈，而用户参数保存在用户栈

  ![image-20221014204757230](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221014204757230.png)

- 添加全局变量寄存器mscratch

  ![image-20221014204913731](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221014204913731.png)

  ![image-20221014204857156](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221014204857156.png)



## 编写不朽的传奇

#### 传奇1:展示你的计算机系统

有想过让程序分时运行，并且保持后台状态也能运行，但是会出现频闪，所以最后采用选中哪个进程就运行哪个，其他进程不动。

- 在键盘读取函数添加pcb偏移函数

  ![image-20221015101341268](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221015101341268.png)

- 在调度函数总，运行所选中的进程

  ![image-20221015101426644](https://raw.githubusercontent.com/ColorStripes/Typora_Picture/master/picture/image-20221015101426644.png)



#### 传奇2：运行ONScripter模拟器

```text
git commit --allow-empty -am "before starting pa5"
git checkout master
git merge pa4
git checkout -b pa5
```

```
sudo apt-get install libsdl-image1.2-dev
sudo apt-get install libsdl2.0-ttf-dev
sudo apt install libsdl1.2-dev libsdl-mixer1.2-dev
sudo apt-get install libbz2-dev
```

