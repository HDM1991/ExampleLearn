#### 概述
obcallback 这个 example 比较简单，粗糙，基本就是向我们展示了两种回调函数的用法。下面我们一方面说说这个 example 的总体思路；另一方面说说关于这两种回调函数文档中描述不太清楚，结合具体代码后才弄明白的地方。

#### 总体思路
这个 example 实现了两个功能，如下：

    * 保护特定名称的进程不被关闭
    * 禁止特定名称的进程启动

不过这两个功能同时只能使用其中一个，而且就使用的这个功能来讲，也只能针对一个进程，毕竟是一个 example。

对于保护这个功能来讲，它是需要两种类型的回调函数配合完成的。PsSetCreateProcessNotifyRoutineEx 注册的回调函数负责在进程启动时保存要保护的进程的相关信息，这些信息用于 ObRegisterCallbacks 注册的回调函数，保护的实质性工作就在该函数注册的回调函数中进行。

对于禁止这个功能，只需要 PsSetCreateProcessNotifyRoutineEx 注册的回调函数。

#### 文档中描述的不太清楚的地方
首先是 *OB_PRE_OPERATION_INFORMATION* structure 的 CallContext 成员，这个成员是用来和当前 ObjectPreCallback callback routine 配对的 ObjectPostCallback callback routine 之间传递信息的。当设置这个成员后，与当前 ObjectPreCallback callback routine 配对的 ObjectPostCallback callback routine 可以通过它的 OB_POST_OPERATION_INFORMATION structure 的参数的 CallContext 成员获取之前设置的值。

然后是 ObjectPreCallback callback routine 和 ObjectPostCallback callback routine 被调用的上下文。下面是文档中的说法 

> This routine is called at PASSIVE_LEVEL in an arbitrary thread context with normal kernel APCs disabled。

更确切一点应该是运行在执行该操作的进程的任意线程上下文中。