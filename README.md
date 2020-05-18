[TOC]

# cpplib_base
C++公用基础库,仿照chromium-base,提供一些基本的功能实现,可以在项目中方便导入,提高开发效率.

# 编译说明
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug/Release
make
```

# 开发计划
类或函数 | 文件 | 说明 | 完成情况
-- | -- | -- | --
MessageLoop | base/thread/message_loop.h | 消息循环队列 | 已完成
Thread | base/thread/thread.h | 基于消息循环的线程 | 已完成
LOG | base/log/logging.h | 日志 | 已完成
Singleton | base/singleton.h | 单例模板类 | 已完成
AtExitManager | base/at_exit.h | 注册退出函数   | 已完成
Base64Encode/Base64Decode | base/encode/base64.h | base64加密和解密 | 已完成
ElapsedTimer | base/timer/elapsed_timer.h | 计时器 | 待开发
Timer | base/timer/timer.h  | 定时器 | 待开发

# 常用类说明
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
BindToCurrentThread | 作用是使在调用`RunLoop`之前就能接收其他线程抛出的任务,在调用`RunLoop`之后会处理这些任务 | 如果你不需要关心`RunLoop`之前是否有任务需要处理,那么你可以不用调用`BindToCurrentThread`接口
PostTask | 抛任务,任务执行完不会回调 | 可在任意线程上调用
PostTaskAndReply | 抛任务,带回调,抛出的任务执行完后,**回调函数会被抛回源线程执行** | 传递的回调函数不能传递参数,带参数版本的借口待开发

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

