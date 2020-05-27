[TOC]

# cpplib_base
C++公用基础库,仿照chromium-base,提供一些基本的功能实现,可以在项目中方便导入,提高开发效率.

# 编译说明
```
git submodule update --init --recursive

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug/Release
make
```

# 开发计划
先列着,慢慢来吧~~~  
类或函数 | 文件 | 说明 | 完成情况
-- | -- | -- | --
TaskQueue | base/thread/task_queue.h | 任务队列 | 已完成
MessageLoop | base/thread/message_loop.h | 消息循环队列 | 已完成
Thread | base/thread/thread.h | 基于消息循环的线程 | 已完成
LOG | base/log/logging.h | 日志 | 已完成
Singleton | base/singleton.h | 单例模板类 | 已完成
AtExitManager | base/at_exit.h | 注册退出函数   | 已完成
Base64Encode/Base64Decode | base/encode/base64.h | base64加密和解密 | 已完成
Md5 | base/encode/md5.h | md5散列 | 已完成
Hash  | base/hash/hash.h | hash散列 | 已完成
ThreadPool | base/thread/thread_pool.h | 基于消息循环线程的线程池 | 已完成
ElapsedTimer | base/timer/elapsed_timer.h | 计时器 | 已完成
DelayTimer | base/timer/delay_timer.h  | 定时器 | 已完成
Json | 请直接使用rapidjson,位于`src/base/third_party/rapidjson` | Json库封装 | 放弃,rapidjson已非常好用,完全封装的话非常耗时
TreeArray | base/array/tree_array.h | 树型数组 | 模板类,只支持数学计算的类型,如int/float/double等; 数组索引从1开始
File | base/file/file.h | 文件跨平台封装 | 待开发
IPCHandler | base/ipc/ipc_handler.h | 封装跨平台进程间通信 | 待开发
Process | base/process/process.h | 封装跨平台进程创建和运行 | 待开发
TCPServer | base/net/tcp_server.h | 封装跨平台tcp server(两种网络模型) | 待开发
TCPClient | base/net/tcp_client.h | 封装跨平台tcp client | 待开发
UDPServer | base/net/udp_server.h |封装跨平台udp server |待开发
UDPClient | base/net/udp_client.h| 封装跨平台udp client|待开发
Metrics | base/metrics.h | 封装数据统计接口 | 待开发


# 常用类说明

## TaskQueue
任务队列,提供任务入队和出队操作,提供锁保护
### 接口
函数 | 说明 | 注意事项
-- | --| --
PushTask | 任务入队 | 自带锁保护
PopTask | 任务出队 | 自带锁保护
Empty | 任务队列是否为空 |
Clear | 清空队列 | 自带锁保护

## MessageLoop
消息循环队列,使用它可以将任何当前线程改造成消息循环线程.

### 示例
```
#include "base/thread/message_loop.h"

base::MessageLoop loop;
loop.BindToCurrentThread();
......
loop.RunLoop();
```

### 接口
函数 | 说明 | 注意事项
-- | --| --
RunLoop | 调用之后,当前线程会进入循环,可以处理其他线程抛过来的任务 | 除非其他线程有调用它的`Stop()`函数, 它才能退出循环
PostTask | 抛任务,任务执行完不会回调 | 可在任意线程上调用
PostTaskAndReply | 抛任务,带回调,抛出的任务执行完后,**回调函数会被抛回源线程执行** | 传递的回调函数不能传递参数,带参数版本的借口待开发
PostDelayTask/PostDelayTaskAndReply | 延时执行任务的版本 | 
Stop | 任务都执行完后,停止消息循环 | 调用后就不会再接收新的任务
StopSoon | 清空任务队列,立即停止消息循环 | 

### 优化方向
* 提供可传递参数的PostTask回调: `PostTaskAndReplyWithResult`;

## Thread
基于消息循环的线程类

### 示例
```
#include "base/thread/thread.h"

base::Thread thread_io;
thread_io.Start();

thread_io.PostTaskAndReply(...);
```

### 接口
函数 | 说明 | 注意事项
-- | -- | --
SetName/GetName | 给线程取名 |
Start/Stop  | 启动/停止线程 | Start保证任务队列启动成功
PostTask/PostTaskAndReply | 抛任务 | 对MessageLoop借口的封装
PostDelayTask/PostDelayTaskAndReply | 延时执行的任务 | 对MessageLoop借口的封装
Stop | 执行完任务后,关闭线程|
StopSoon | 清空尚未执行的任务,关闭线程

## LOG
简单的日志系统,可选择打印到控制台或者写入到日志文件中.
![image](https://github.com/243286065/pictures_markdown/blob/master/media/h264/5d42c4769e6b7f95f865cb562fee1ab3.png?raw=true)

日志格式: `[进程号:线程号:日期/时间.微秒:日志级别:文件(行号)] 日志内容`

使用示例:
```
#include "base/log/logging.h"

// 初始化日志配置
logging::InitLogging("test.log", logging::LOG_TO_BOTH_FILE_AND_SYSTEM_DEBUG_LOG, logging::APPEND_TO_OLD_LOG_FILE);


LOG(INFO) << "------------------------------END------------------------------------------" << 1;

```

### 接口
函数或接口 | 说明 | 注意事项
-- | -- | --
SetLogMessageHandler | 自定义日志处理函数,可用通过设置接口由程序自己控制日志处理. 函数原型`bool(int severity, const char* file, const int line, const char* message)>` | 当设置的日志回调函数返回`true`时表示不再由日志系统对日志进行处理,返回`false`时,日志系统将继续处理这条日志.
GetLogMessageHandler | 获取当前自定义日志处理函数 | 默认为`nullptr`
SetMinLogLevel/GetMinLogLevel | 设置或获取会被处理日志的最低级别.日志供分为`LOG_DEBUG`/`LOG_INFO`/`LOG_WARNING`/`LOG_ERROR`/`LOG_FATAL`,其值分别标识`-1~3`. | 默认debug模式下最低级别为`LOG_DEBUG`,release模式下为`LOG_WARNING`
InitLogging | 对默认日志配置进行修改,`LoggingDestination`参数表示日志是要输出到文件还是控制台; `OldFileDeletionState`参数决定当日志文件重名时,要不要删除旧文件重写 | 如果不调用这个接口,日志系统会以默认参数运行(日志只打印到控制台)
LOG | 创建日志,可以使用`LOG(DEBUG)/LOG(INFO)/LOG(WARNING)/LOG(ERROR)/LOG(FATAL)`来创建对应级别的日志. | 日志级别小于所设置的最低处理级别时,日志不会被处理.
DLOG | DEBUG模式下才会处理的日志 |
CHECK | 检查条件是否成立,类似`assert`的作用 | 
DCHECK | DEBUG模式下才会生效的CHECK|


### 优化方向
* 日志文件大小限制,包括单个文件大小和总的日志文件大小;
* 合并写入日志文件,减少写文件操作.

## Singleton
一个简单的单例类模板,使用了锁保证创建单例对象的线程安全,但是不保证其他对象逻辑线程安全.

使用示例:
```
#include "base/singleton.h"

class A {...};
typedef Singleton<A> ASingleton;

ASingleton::GetInstance()->...
```

只为了在项目中创建单例类方便,很简单,没什么好讲的.


## AtExitManager
其作用类似atexit(),即在声明周期结束后执行所注册的回调。在该对象的生命周期内可注册多个回调函数，在对象销毁时所有的注册函数按照出栈的形式来调用。

示例:
```
#include "base/at_exit.h"
#include "base/log/logging.h"

void printResult(int sum) {
    LOG(INFO) << "-------sum-------" << sum;
}

int main() {
    base::AtExitManager exit_manager;

    base::AtExitManager::RegisterCallback(std::bind(printResult, 100));
    base::AtExitManager::RegisterCallback(std::bind(printResult, 101));
    base::AtExitManager::RegisterCallback(std::bind(printResult, 102));

    {
        base::AtExitManager exit_manager_2;
        base::AtExitManager::RegisterCallback(std::bind(printResult, 103));
        base::AtExitManager::RegisterCallback(std::bind(printResult, 104));
        base::AtExitManager::RegisterCallback(std::bind(printResult, 105));
    }

    base::AtExitManager exit_manager_3;
    base::AtExitManager::RegisterCallback(std::bind(printResult, 107));
    base::AtExitManager::RegisterCallback(std::bind(printResult, 108));
    base::AtExitManager::RegisterCallback(std::bind(printResult, 109));

    return 0;
}
```

最后运行结果:
```
[24643:24643:0518/133342.891162:INFO:thread_test.cc(35)] -------sum-------105
[24643:24643:0518/133342.891221:INFO:thread_test.cc(35)] -------sum-------104
[24643:24643:0518/133342.891242:INFO:thread_test.cc(35)] -------sum-------103
[24643:24643:0518/133342.891265:INFO:thread_test.cc(35)] -------sum-------109
[24643:24643:0518/133342.891290:INFO:thread_test.cc(35)] -------sum-------108
[24643:24643:0518/133342.891309:INFO:thread_test.cc(35)] -------sum-------107
[24643:24643:0518/133342.891330:INFO:thread_test.cc(35)] -------sum-------102
[24643:24643:0518/133342.891350:INFO:thread_test.cc(35)] -------sum-------101
[24643:24643:0518/133342.891369:INFO:thread_test.cc(35)] -------sum-------100
```

### 接口
函数或接口 | 说明 | 注意事项
-- | -- | --
RegisterCallback | 静态函数,注册退出时回调 | 调用顺序和注册顺序相反
RunCallbacksNow | 静态函数,立即执行当前`AtExitManager`对象中注册的回调 |

## Base64
提供简单的base64加密解解密功能,主要copy自chromium中的实现.

示例:
```
#include "base/log/logging.h"
#include "base/encode/base64.h"


std::string input = "121321321asdadadsadsad";
std::string result = base::Base64Encode(input);
std::string output;
bool valid = base::Base64Decode(result, &output);
LOG(WARNING) << input;
LOG(WARNING) << result;
LOG(WARNING) << valid << "-----" << output;
```

输出结果:
```
[22337:22337:0518/192643.265964:WARNING:thread_test.cc(104)] 121321321asdadadsadsad
[22337:22337:0518/192643.266039:WARNING:thread_test.cc(105)] MTIxMzIxMzIxYXNkYWRhZHNhZHNhZA==
[22337:22337:0518/192643.266075:WARNING:thread_test.cc(106)] 1-----121321321asdadadsadsad
```

### 接口
函数或接口 | 说明 | 注意事项
-- | -- | --
Base64Encode | base64加密函数 | 
Base64Decode | base64解密函数 | 输出注意是string指针


## ThreadPool
简单的线程池,基于`base/thread/thread.h`,非阻塞. 注意在构造函数时就已经创建了线程,但是都还未运行,需要等到调用Start,才能开始接收任务.

示例:
```
#include "base/thread/thread_pool.h"
#include "base/log/logging.h"
#include "base/utils.h"


base::ThreadPool pool(10);
pool.Start();

auto timestamp = base::GetMillSecondsTimestamp();
for(int i= 0 ; i< 10000; i++) {
pool.PostTask([=]() {
    LOG(WARNING) << "----------------------------" << i;
    // std::this_thread::sleep_for(std::chrono::seconds(3));
});
}
pool.Stop();
LOG(WARNING) << "Use time: " << base::GetMillSecondsTimestamp() - timestamp << " ms";
```

### 接口
函数或接口 | 说明 | 注意事项
-- | -- | --
Start | 开始运行线程池 | 只有调用Start后才会开始接收任务,且此操作为非阻塞操作
Stop | 停止线程池 | 此接口需要得到已有任务完成才会完全退出.
PostTask | 传递任务 |

改进方向:  
* 提供接收回调任务接口;
* 提供延时任务接口.

## Hash
简单的Hash散列计算函数. 基于cityhash项目.

示例:
```
#include "base/log/logging.h"
#include "base/hash/hash.h"

std::string test = "1232423中国1231312";
LOG(WARNING) << "--------------" << std::hex << base::Hash(test) << "---" << base::Hash64(test) << "----" << base::Hash128(test).first << base::Hash128(test).second;
std::wstring test1 = L"1232423中国1231312";
LOG(WARNING) << "-----2---------" << std::hex << base::Hash(test1) << "---" <<base::Hash64(test1) << "----" << base::Hash128(test1).first << base::Hash128(test1).second;
LOG(WARNING) << "-----md5------" << base::FastMD5(test);
```
运行结果:
```
[14202:14202:0519/210024.549851:WARNING:thread_test.cc(130)] --------------948f8e9c---c3ac8f11bcf5bfd9----7e9ce8a954094c9b1a6ac0c513b6bf4f
[14202:14202:0519/210024.549861:WARNING:thread_test.cc(132)] -----2---------948f8e9c---c3ac8f11bcf5bfd9----7e9ce8a954094c9b1a6ac0c513b6bf4f
[14202:14202:0519/210024.549981:WARNING:thread_test.cc(133)] -----md5------d482a1faf595dc83ecbc5378b25f90af
```

宽字符串也是先转换成char再进行计算hash值的.

### 接口
函数或接口 | 说明 | 注意事项
-- | -- | --
Hash | 得到32位整数值结果的hash | 注意存放结果的buff需要自己分配足够内存
Hash64 | 得到64位整数的hash | 注意存放结果的buff需要自己分配足够内存
Hash128 | 得到128位整数的hash | 结果以一个pair<64,64>保存; 注意存放结果的buff需要自己分配足够内存
FastMD5 | md5计算 | 

## ElapsedTimer
一个简单的计时器. **创建时就开始计时**.

示例:
```C++
#include "base/timer/elapsed_timer.h"


base::ElapsedTimer timer;
......

base::TimeDelta delta = timer.Elapsed();
LOG(WARNING) << delta.InMicroseconds();
LOG(WARNING) << timer.Begin() << "----" << timer.Elapsed();

```

### 接口
函数或接口 | 说明 | 注意事项
-- | -- | --
Elapsed | 获取到目前位置时间长度 | 返回`base::TimeDelta`对象
Begin | 返回开始计时的时间戳(微秒) |

## DelayTimer
延时定时器,可以触发任务. 非阻塞操作依赖`base::Thread`实现.

示例:
```
#include "base/time/time.h"
#include "base/timer/delay_timer.h"
#include "base/thread/thread.h"

void print_timestap(int index) {
  LOG(WARNING) << index << "---------" << base::Now();
}

base::MessageLoop loop;
  print_timestap(0);

base::Thread io;
io.Start();
io.PostDelayTask(base::TimeDelta::FromSeconds(3), std::bind(print_timestap, 1));
io.PostDelayTask(base::TimeDelta::FromSeconds(1), std::bind(print_timestap, 2));
io.PostTask(std::bind(print_timestap, 3));
io.PostTask(std::bind(print_timestap, 4));
io.PostTask(std::bind(print_timestap, 5));

io.PostTaskAndReply(nullptr, std::bind(print_timestap, 6));

base::DelayTimer delay_timer(base::TimeDelta::FromSeconds(5));
delay_timer.SyncBlockWait(std::bind(print_timestap, 6));
print_timestap(7);
delay_timer.SyncNoBlockWait(std::bind(print_timestap, 8));

delay_timer.AsyncWait(std::bind(print_timestap, 9));
delay_timer.AsyncWait(std::bind(print_timestap, 10));
delay_timer.Cancle();
print_timestap(11);

base::DelayTimer delay_timer2(base::TimeDelta::FromSeconds(3));
delay_timer2.AsyncWait(std::bind(print_timestap, 12));

loop.RunLoop();
```

输出:
```
[12847:12847:0523/105000.088835:WARNING:thread_test.cc(148)] 0---------1590202200088869
[12847:12848:0523/105000.089256:WARNING:thread_test.cc(148)] 3---------1590202200089288
[12847:12848:0523/105000.089332:WARNING:thread_test.cc(148)] 4---------1590202200089342
[12847:12848:0523/105000.089370:WARNING:thread_test.cc(148)] 5---------1590202200089377
[12847:12848:0523/105001.089233:WARNING:thread_test.cc(148)] 2---------1590202201089240
[12847:12848:0523/105003.089212:WARNING:thread_test.cc(148)] 1---------1590202203089219
[12847:12847:0523/105005.089382:WARNING:thread_test.cc(148)] 6---------1590202205089403
[12847:12847:0523/105005.089440:WARNING:thread_test.cc(148)] 7---------1590202205089446
[12847:12847:0523/105005.090000:WARNING:thread_test.cc(148)] 11---------1590202205090015
[12847:12847:0523/105005.090305:WARNING:thread_test.cc(148)] 6---------1590202205090323
[12847:12851:0523/105008.090304:WARNING:thread_test.cc(148)] 12---------1590202208090313
```

### 接口
函数或接口 | 说明 | 注意事项
-- | -- | --
SyncBlockWait | 同步阻塞等待,即需要定时器超时才能继续执行 | 不依赖MessageLoop;调用它定时器才会计时
SyncNoBlockWait | 同步非阻塞等待,即定时器超时后回调到源线程执行 | 依赖MessageLoop, 即原线程需要是消息循环线程,且不会被长期阻塞在某个任务上
AsyncWait | 异步等待,定时器超时后任务会在其他线程上执行 | 原线程不依赖于MessageLoop
Cancle | 取消定时器 | 会立刻取消所有还未触发的任务

> 注意:本定时器,一个定时器可以多次wait,每次都是重新开始计时.

## TreeArray
树型数组:就是用数组来模拟树状结构,常用在频繁执行数组前缀和计算的场景中. 有关介绍可以参考: https://www.cnblogs.com/xl2432/p/12974749.html . 

示例:
```
#include "base/array/tree_array.h"

base::TreeArray<int> tree(100);
for(int i = 1; i <= 100; i++) {
    tree.set(i, i);
}

LOG(WARNING) << "-----0-------" << tree.get(50);
LOG(WARNING) << "-----1-------" << tree.get(100);
LOG(WARNING) << "-----2-------" << tree.sum(50);
LOG(WARNING) << "-----3-------" << tree.sum(100);
tree.add(50, 1);
LOG(WARNING) << "-----0-------" << tree.get(50);
LOG(WARNING) << "-----4-------" << tree.sum(50);
LOG(WARNING) << "-----5-------" << tree.sum(100);
tree.set(50, 50);
LOG(WARNING) << "-----0-------" << tree.get(50);
LOG(WARNING) << "-----6-------" << tree.sum(50);
LOG(WARNING) << "-----7-------" << tree.sum(100);
```
输出:
```
[1150:1150:0527/170551.372410:WARNING:thread_test.cc(201)] -----0-------50
[1150:1150:0527/170551.372463:WARNING:thread_test.cc(202)] -----1-------100
[1150:1150:0527/170551.372481:WARNING:thread_test.cc(203)] -----2-------1275
[1150:1150:0527/170551.372501:WARNING:thread_test.cc(204)] -----3-------5050
[1150:1150:0527/170551.372519:WARNING:thread_test.cc(206)] -----0-------51
[1150:1150:0527/170551.372537:WARNING:thread_test.cc(207)] -----4-------1276
[1150:1150:0527/170551.372556:WARNING:thread_test.cc(208)] -----5-------5051
[1150:1150:0527/170551.372573:WARNING:thread_test.cc(210)] -----0-------50
[1150:1150:0527/170551.372591:WARNING:thread_test.cc(211)] -----6-------1275
[1150:1150:0527/170551.372608:WARNING:thread_test.cc(212)] -----7-------5050
```
