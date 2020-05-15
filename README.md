[TOC]

# cpplib_base
C++公用基础库,仿照chromium-base,提供一些基本的功能实现,可以在项目中方便导入,提高开发效率.

# 开发计划
类或函数 | 文件 | 说明 | 完成情况
-- | -- | -- | --
MessageLoop | src/base/thread/message_loop.h | 消息循环队列 | 已完成
Thread | src/base/thread/thread.h | 基于消息循环的线程 | 已完成
LOG | src/base/logging/logging.h | 日志 | 待开发

# 常用类说明
## MessageLoop
消息循环队列,使用它可以将任何当前线程改造成消息循环线程.

### 示例
```
#include "src/base/thread/message_loop.h"

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
PostTaskAndReplyWithResult(待开发) |  |

## Thread
基于消息循环的线程类

### 示例
```
#include "src/base/thread/thread.h"

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



