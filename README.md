
# CN_HW
此仓库用于研究生计算机网络课程大作业，C++实现HTTP服务器,运行文件见makefile,软件报告如下

# C++实现HTTP服务器
①　能接收用户浏览器端基于HTTP的GET和POST请求（已实现）
②　能够读取服务器中存储的文件，并返回给请求客户端（已实现）
③　服务器端支持错误处理，如要访问的URL不存在时回复404错误等（已实现）
④　支持文件在服务器上的上传和下载（已实现）
⑤　服务器可以支持压缩，比如gzip。服务器端可以增加压缩支持（已实现）
⑥　支持多线程，来提高处理请求的并发度（已实现）
⑦　实现服务器文件的分块传输（已实现）
⑧　实现HTTP的持久连接keep alive （已实现）
⑨　实现管道化连接，同时并行发送多个请求（未实现）
⑩　使用openssl库，提高服务器安全性（未实现）
⑪　使用libevent支持多路并发（未实现）



# **实验五：****HTTP服务器实验**

小组成员：李航程（74）、郭子漾（175）、李若凡（174）

目录

[**HTTP服务器实验**	](#_Toc15542 )

[一． 实验任务	](#_Toc22565 )

[1.实验目的	](#_Toc10931 )

[2.预期实现的功能	](#_Toc29422 )

[3.具体要求和帮助	](#_Toc3900 )

[二． 实验过程	](#_Toc32068 )

[1.试验环境	](#_Toc17545 )

[2.实验说明	](#_Toc27664 )

[3.程序运行截图	](#_Toc10532 )

[三． 实验源码	](#_Toc5081 )

[四． 实验总结	](#_Toc30488 )

[五． 参考资料	](#_Toc1969 )





## 一． **实验任务**

### **1.实验目的**

<img src=".\readme图片资源\图1.jpg" alt="img" style="zoom: 80%;" /> 

### **2.****预期实现的功能**

①　能接收用户浏览器端基于HTTP的GET和POST请求（已实现）

②　能够读取服务器中存储的文件，并返回给请求客户端（已实现）

③　服务器端支持错误处理，如要访问的URL不存在时回复404错误等（已实现）

④　支持文件在服务器上的上传和下载（已实现）

⑤　服务器可以支持压缩，比如gzip。服务器端可以增加压缩支持（已实现）

⑥　支持多线程，来提高处理请求的并发度（已实现）

⑦　实现服务器文件的分块传输（已实现）

⑧　实现HTTP的持久连接keep alive （已实现）

⑨　实现管道化连接，同时并行发送多个请求（未实现）

⑩　使用openssl库，提高服务器安全性（未实现）

⑪　使用libevent支持多路并发（未实现）

 

 

### **3.具体要求和帮助**

#### **（1）浏览器访问完整版流程**

<img src=".\readme图片资源\图2.jpg" alt="img" style="zoom: 80%;" /> 

在浏览器地址栏键入URL，按下回车之后会经历以下流程：
\1. 浏览器向DNS 服务器请求解析该URL中的域名所对应的IP地址;
\2. 解析出IP地址后，根据该IP地址和默认端口80，和服务器建立TCP连接;
\3. 浏览器发出读取文件(URL中域名后面部分对应的文件)的HTTP请求，该请求报文作为TCP三次握手的第三个报文的数据发送给服务器;
\4. 服务器对浏览器请求作出响应，并把对应的web资源如html、css、js文件等发送给浏览器;
\5. 释放TCP连接;
\6. 浏览器将渲染该页面;

#### **（2）HTTP协议基础知识**

​	参考网址：https://blog.csdn.net/weixin_38087538/article/details/82838762

​	简化处理，但我们至少要支持GET方式和POST方式的请求。

​	请求报文构成：

<img src=".\readme图片资源\图3.jpg" alt="img"  /> 

我们访问www.baidu.com的请求如下（通过Wireshark抓包）：

GET / HTTP/1.1Host: www.baidu.comConnection: keep-aliveUser-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.92 Safari/537.1 LBBROWSERAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8xxxxxxxxxxxxxxx: gzip,deflate,sdchAccept-Language: zh-CN,zh;q=0.8Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3Cookie: BAIDUID=E21378CBAB746D5B00E45348BBD091FB:FG=1; ......

响应报文如下：

HTTP/1.1 200 OKDate: Wed, 17 Nov 2021 05:39:06 GMTServer: BWS/1.0Content-Length: 7083Content-Type: text/html;charset=utf-8Cache-Control: privateContent-Encoding: gzipVary: Accept-EncodingExpires: Sat, 15 Nov 2031 05:39:06 GMTSet-Cookie: H_PS_PSSID=2121_1431_1944_1788; path=/; domain=.baidu.comConnection: Keep-Alive[html网页数据]

百度支持gzip，我们可以不压缩数据。使用标准浏览器来验证我们的web服务器是否正确。可以通过访问广域网上的网站如ping www.baidu.com获得的ip来验证我们自制浏览器的正确性。

 

（3）Socket编程基本知识

Linux socket参考网站：http://blog.csdn.net/fengyv/article/details/659980

socket在linux g++、windows g++、VS中的使用方法略有不同，资料自己查找。推荐使用Linux g++。

 

（4）服务器端的并发处理

服务器为了应对同时多个请求到达的情况，必须支持并发，可选基本模式有三种：多进程、多线程、多路复用IO接口。在Linux下的代表机制分别为：fork、pthread、epoll/select/poll。著名的Apache HTTP Server支持多进程和多线程，而Nginx在Linux下默认使用的是epoll。下面是参考网站：

​	fork  http://blog.csdn.net/lingdxuyan/article/details/4993883

​	pthread  http://blog.csdn.net/handyhuang/article/details/8301058

​	epoll  http://blog.csdn.net/ljx0305/article/details/4065058

​	select  http://www.douban.com/note/265840990/?type=like

​	使用select实现的echo （linux版）http://blog.csdn.net/god2469/article/details/8761346

​	使用select的TCP示例（linux+windows）：http://www.zeali.net/entry/13

## 二． **实验****过程**

### **1.试验环境**

Ubuntu18.04（VMware中） vscode  g++

### **2.实验说明**

​	仔细阅读实验要求后，为了便于开发和节省时间，所以决定：

（1） 不做图形界面而使用控制台模式，从控制台显示服务器关键信息，代码也会比较简洁。

（2） 浏览器端并发处理的技术上选择的是多线程方式，因为不是很熟悉libevent这个库。

（3） 整个实验并没有考虑高并发和安全性，意在实现基础功能。

 

### **3.程序运行截图**

（1）启动服务器，开启监听，监听端口为：http:0.0.0.0:9090

<img src=".\readme图片资源\图4.jpg" alt="img" style="zoom: 80%;" /> 

（2）访问http:0.0.0.0:9090/test.html（一个很简单的html页面）：

<img src=".\readme图片资源\图5.jpg" alt="img" style="zoom:80%;" /> 

终端内容打印：客户端发送GET请求去请求”/”中的test.html资源

<img src=".\readme图片资源\图6.jpg" alt="img" style="zoom:80%;" /> 

服务器响应请求发送test.html中的内容，在终端输出：

<img src=".\readme图片资源\图7.jpg" alt="img" style="zoom:80%;" /> 

（3）POST方法演示：

监听端口为：http:localhost:8080

输入URL：http:localhost:8080/post.html

使用form表单发送post请求，请求参数为bc=1，http报文体中可见到bc参数

<img src=".\readme图片资源\图8.jpg" alt="img" style="zoom: 80%;" /> 

（4）删掉post.html,再请求访问post.html资源，无法访问显示

![img](.\readme图片资源\图9.jpg) 

服务器响应请求，终端打印404，not found

<img src=".\readme图片资源\图10.jpg" alt="img" style="zoom:80%;" /> 

（4） 文件的上传和下载都序列化二进制流进行传输：

<img src="D:\本科文件汇总\计算机网络\CN_HW\readme图片资源\图11.jpg" alt="img" style="zoom:80%;" /> 

通过浏览器发出post请求给服务器上传文件bg.jpg

<img src=".\readme图片资源\图12.jpg" alt="img" style="zoom:80%;" /> 

bg.jpg文件转化为二进制流进行传输，终端打印出二进制乱码

<img src=".\readme图片资源\图13.jpg" alt="图13" style="zoom:80%;" /> 

（5） 多个浏览器同时输入相同URL不同资源，进行访问，实现少数线程并发

<img src=".\readme图片资源\图14.jpg" alt="img" style="zoom:80%;" /> 

（6） 分割传输 在传输大容量数据时，通过把数据分割成多块，能够让浏览器逐步显示页面。这种把实体主体分块的功能称为分块传输编码（Chunked Transfer Coding）。

进入网页渲染时，浏览器向服务器请求bg.jpg资源

<img src=".\readme图片资源\图15.jpg" alt="img" style="zoom:80%;" /> 

服务器响应请求成功，分块发送图片资源bg.jpg，块大小设置为4096byte

<img src=".\readme图片资源\图16.jpg" alt="img" style="zoom:80%;" /> 

<img src=".\readme图片资源\图17.jpg" alt="img" style="zoom:80%;" /> 

（7） 持久连接keep alive：在最早的 http 协议中，每进行一次 http 通信，就需要做一次 tcp 的连接。在http持久连接下，服务器发出响应后让TCP连接继续打开着。同一对客户/服务器之间的后续请求和响应可以通过这个连接发送。

设置如果在20秒内与服务器没有数据传输，则每隔3秒发送一次探测报文，发送3次后如果任然没有响应，则关闭TCP连接

<img src=".\readme图片资源\图18.jpg" alt="img" style="zoom:80%;" /> 

## 三． **实验源码**

  https://github.com/2017403603/CN_HW.git

## 四． **实验总结**

通过查找学习资料，使用C++实现了一个残缺版的http服务器，完成了一些基础功能，并没有考虑服务器的并发和安全性。

（1） 该试验在Ubuntu环境下用C/C++语言完成，学到了linux下c/c++语言的一些类库和编译方法。

（2） 从底层原理上理解了GET和POST方法的本质区别。

（3） 用socket实现浏览器和服务器之间的通讯，初步了解socket中的一些方法的使用注意事项。

（4） 通信过程使用http协议，详细学习了http协议报文格式及其报文内容所表达的意思，以及socket通信时如何编写http协议。

（5） http服务器要为多个客户端服务，故其势必要支持并发处理，这就要求服务器端要实现多线程，进一步熟悉多线程编程。

（6） 学习到了了http协议中文件的分块传输机制和keep alive机制

 

## 五． **参考资料**

（1） 《C语言程序设计》 孙鸿飞 刘国成 主编

（2） C语言SOCKET编程指南_fenglovel_新浪博客

http://blog.sina.com.cn/s/blog_79b01f66010163q3.html

（3） java程序员菜鸟进阶(六)《HTTP权威指南》之HTTP相关概念详解

http://www.360doc.com/content/13/0217/11/9318309_266094744.shtml

（4） pthread编程基础-fireaxe-ChinaUnix博客

http://blog.chinaunix.net/uid-20528014-id-333508.html
