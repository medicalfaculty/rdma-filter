### client初始化rdma连接

1. `malloc`开辟空间

2. 初始化`dev`：调用`rdma_dev`构建函数

   2-1. 获取`dev`和`ctx`

   2-2. 查询`dev_attr`和`port_attr`（好像没用）

   2-3. 分配`pd`

   2-4. 注册`mr`，初始化`ibv_mr.addr`（没懂为啥）

3. 初始化向量

4. 对每个线程：

   4-1. 创建`cq`

   4-2. 其他：初始化协程池 & 临时内存 & task和handle
   
   4-3. 创建`TCP`连接，`connect`函数
   
   ​	4-3-1. 初始化`qp_init_attr`
   
   ​	4-3-2. `create_qp`
   
   ​	4-3-3. 切换`qp`状态，`init` & `rtr` & `rts`
   
   4-4. 对每个协程：
   
   ​	4-4-1. 创建`mr`
   
   ​	4-4-2. `query_remomte_mr`
   
   ​	4-4-3. `run`
   
   ​	4-4-4. `sync_dir`
   
5. 加载


