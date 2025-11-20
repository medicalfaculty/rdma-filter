cd D:\study\rdma-filter\

实验室机器
ssh -i "C:\Users\Yuandu\.ssh\id_rsa" -p 10131 root@210.28.134.155
scp -r -i "C:\Users\Yuandu\.ssh\id_rsa" -P 10131 src test CMakeLists.txt build root@210.28.134.155:liuyunchuan/exp01
cmake -DTOGGLE_RDMA=OFF ..

美国机器
ssh -i "C:\Users\Yuandu\.ssh\id_rsa" yunchuan@amd123.utah.cloudlab.us
ssh -i "C:\Users\Yuandu\.ssh\id_rsa" yunchuan@amd112.utah.cloudlab.us
ssh -i "C:\Users\Yuandu\.ssh\id_rsa" yunchuan@amd102.utah.cloudlab.us

scp -r src test CMakeLists.txt build yunchuan@amd123.utah.cloudlab.us:exp01
scp -r src test CMakeLists.txt build yunchuan@amd112.utah.cloudlab.us:exp01
scp -r src test CMakeLists.txt build yunchuan@amd102.utah.cloudlab.us:exp01

sudo apt update
sudo apt install cmake libibverbs-dev rdma-core librdmacm1 librdmacm-dev ibverbs-utils infiniband-diags perftest linux-tools-common linux-tools-generic linux-cloud-tools-generic
mkdir exp01

ssh yunchuan@ms0936.utah.cloudlab.us
ssh yunchuan@ms0914.utah.cloudlab.us
ssh yunchuan@ms0910.utah.cloudlab.us

ssh yunchuan@amd109.utah.cloudlab.us
ssh yunchuan@amd102.utah.cloudlab.us
ssh yunchuan@amd112.utah.cloudlab.us

./test/1_test
./test/2_srv
./test/2_cli

ping 10.10.1.1
ib_send_bw -d mlx5_0
ib_send_bw -d mlx5_0               10.10.1.1
ib_send_bw -d mlx5_0 -D 4 -s 65536 10.10.1.1

scp src\rdma_bf\rdma_bf.h yunchuan@clnode305.clemson.cloudlab.us:exp01\src\rdma_bf
scp test\2_cli.cpp test\2_srv.cpp yunchuan@amd204.utah.cloudlab.us:exp01\test
scp test\2_cli.cpp test\2_srv.cpp yunchuan@clnode294.clemson.cloudlab.us:exp01\test

查看NAT地址
ip a

查看网卡名
ibstat



## CloudLab Node

网卡支持rdma的节点

+ (Powder)
d760p, d760-gpu, d760-hgpu

+ Apt: 1
r320

+ CloudLab Utah: 5
m510, xl170, d6515, c6525, c6620 (d750, d7615, d760, d760-hbm)

+ Wisconsin: 5
c240g5, sm110p, sm220u, d7525, d8545

+ Clemson: 6
ibm8335, r7525, r650, r6525, nvidiagh, r6615

















# DEBUG


报错：`RDMA READ failed: Work Request Flushed Error`
解决：填wq的远端内存地址越界了


报错：服务端listen时，客户端connect函数阻塞了
解决：调试出报错信息：bind failed: Address already in use，原因是上次运行没正常结束，没有释放系统socket
    连续运行两种rdma索引就会卡住，就是因为这个，不知道咋办，就一次只运行一次
    临时解决办法：`sudo lsof -i :18515`查看进程编号，然后`kill`


异常：使用从cuckoo filter库抄来的生成随机数的函数，生成随机数据集，做dram实验时发现fpr比预期高了一倍还多，然后使用固定的数字作数据集，fpr顺利降了下来，因此怀疑生成随机数据集的代码。
更新：使用固定的数字作数据集，fpr也不对，所以跟数据集没关系。把bf代码换成wormhole里的版本，fpr也不对，搁置了，找不出原因。
解决：师兄说是哈希函数的问题，不用在意。


报错：CAS lock failed: transport retry counter exceeded
    RDMA READ failed: transport retry counter exceeded
分析：本端发请求，对端没反应，本端又自动试了几次，一直没反应，重试次数就耗尽了，网卡就写一个失败标志放进cq里。
尝试：回退到之前无锁单客户端版本试一试，对比对比。
更新：在旧版本的基础上，为server添加创建锁列表、连接多client的功能，然后删去cq和qp，client基本没有改变，然后运行时就报了retry exceeded，因此怀疑是删去cq和qp的问题。
更新：再回退，只添加创建锁列表、连接多client的功能，不删去cq和qp，再次运行就正常。因此更确信是删去cq和qp的问题，不过暂不清楚原理。
定位问题：本来正常的代码，仅仅注释掉server里把qp.qp_num传给client的代码时，就出现了retry exceeded。但是还不了解原理。
原理：问题在于rdma连接必须要两边都有qp，即使被单边访问的一端不往qp里进行请求。rdma连接里一个机器的qp对应另一个机器的qp，所以有多个机器就要多个qp。所以server要给每个client创建一个qp，各自连接，不过cq可以不用多个，共用一个就行。已解决。


