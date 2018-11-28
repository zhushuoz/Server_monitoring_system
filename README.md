#### Master端

1. 利用tcp连接实现master与client的通信，保证数据的可靠传输。运用多线程，实现高并发处理信息，可以保证数据的共享。
2. 用链表实现任务队列，链表节点中存放client端的IP信息，链表采用虚拟头结点实现，方便对链表进行插入和删除操作。
3. 使用配置文件的方式记录master端和client端的地址信息和端口信息等，便于维护。
4. 在程序开始时，先从配置文件中读取信息，再进行初始化，将配置文件中的IP段插入任务队列。保证master端上线可以先遍历任务队列中的IP。
5. master创建一个线程监听client的心跳连接，当有连接时，判断当前连接的client端IP是否存在在队列中，若不存在则插入队列中。在插入前先寻找节点最少的任务队列，再进行插入以平均每条线程的负载。
6. master创建一个线程接收client端的警报信息，保证client端有警报信息后可以第一时间告知master端。master端收到警告信息后，将警报信息和client端的IP写入Warning文件中。
7. 程序处理信息的并发度为五，创建五个线程分别对应五个任务队列，每个队列中循环遍历当前的链表。通过节点中的IP信息，依次创建套接字连接client端，若连接成功进行收文件等操作，失败将此节点从任务队列中删除。
8. 用长短连接的方式进行文件传输，保证数据不会混乱。长链接接收标示码，短连接进行传输文件。master端收到正确的标示码后，连接client端创建短连接，接收文件。接收成功后关闭短连接，继续接接收标示码。
9. 运用线程锁对链表加锁，线程锁可以解决多个线程同时访问共享数据时，数据不同步的问题。创建五个线程锁，对五个链表进行加锁，在每条链表中保证同一时刻只能进行删除或者插入操作。

#### Client端

1. client端创建六个线程分别运行六个脚本，检测服务器的CPU，Disk等信息，并将信息写入日志文件中，实现对服务器的健康检测。
2. 创建线程，作为心跳连接，循环连接master端。
3. 创建线程来等待master端的连接。若有连接，则进行文件传输，传输结束后将日志文件清空。
4. 运用线程锁对六个日志文件进行加锁，保证同一时刻只能写入或读出。

#### 上线功能

​	将程序设置成守护进程，添加到系统服务中。

```makefile
install:
	@mkdir /etc/pihealth_zhushuo/
	@cp ./master /etc/pihealth_zhushuo/
	@cp ./con_info.log /etc/pihealth_zhushuo/
	@cp ./pihealth_zhushuo.service /lib/systemd/system/
	@chmod 755 /lib/systemd/system/pihealth_zhushuo.service
	@cp ./pihealth_zhushuo.start /etc/pihealth_zhushuo/
	@chmod 755 /etc/pihealth_zhushuo/pihealth_zhushuo.start
	@cp ./pihealth_zhushuo.stop /etc/pihealth_zhushuo/
	@chmod 755 /etc/pihealth_zhushuo/pihealth_zhushuo.stop
	@touch /etc/pihealth_zhushuo/pihealth_zhushuo.pid
	@systemctl daemon-reload
	@systemctl enable pihealth_zhushuo.service
	#设置开机自启动
	@systemctl start pihealth_zhushuo.service
	#开启服务
uninstall:
	#删除服务
	@rm -rf /etc/pihealth_zhushuo/ 
	@systemctl stop pihealth_zhushuo.service
	@rm -rf /lib/systemd/system/pihealth_zhushuo.service
```

#### 脚本信息

1. ##### CPU信息获取

   时间 负载1（1分钟） 负载2（5分钟） 负载3（15分钟） 占用率 （时间间隔0.5） 当前温度 警告（normal，note（50-70），warning（70~））

2. ##### 磁盘信息获取DISK

   时间 标识（0为整个磁盘，1为分区） 磁盘还是分区（disk| /boot , /） 磁盘/分区总量 磁盘/分区剩余量   占用比例

3. ##### 恶意进程检测

   时间 进程名 PID 用户 CPU占用 内存占用

   1. 在5s内如果某一进程的CPU或者内存占用超过50%，视为恶意进程
   2. 第一次检测时，获取所有可能的恶意进程
   3. 如果第一次检测时有潜在恶意进程，等待5s继续检测
   4. 如果潜在恶意进程仍然占超过阈值的资源，视为恶意进

4. ##### MEM内存信息

   时间　总量　剩余量　当前占用（％）　占用百分比动态平均值

5. ##### 系统运行概况获取

   时间 主机名 OS版本 内核版本 运行时间 平均负载 磁盘总量 磁盘已用% 内存大小 内存已用% CPU温度 磁盘报警级别 内存报警级别 CPU报警级别

   若警报级别达到`warning`视为危险警报

6. ##### 用户信息获取

   时间 用户总数（非系统用户）近期活跃用户（3个） 具有ROOT权限用户 当前在线用户_登录IP_TTY