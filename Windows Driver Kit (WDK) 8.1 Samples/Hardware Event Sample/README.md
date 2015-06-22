### 对这个例子的一些想法
现在回头在看 Hardware Event Sample 这个例子，它到底有哪些方面值得我们学习？我觉得就是驱动中对一些竞争条件的处理。换个说法讲，如果当我们的驱动面对这样的执行环境时，我们会考虑到这些竞争条件吗？我觉得考虑不到，那为什么考虑不到？说明我们对驱动所处的运行环境的理解还是不够，说白了就是对内核的学习，理解不够。相比较，假如这是一个应用程序，我们是绝对可以考虑的这些竞争条件的。

下面是对这个例子中竞争条件处理的一个总结。

* CustomTimerDPC 和 EventCancelRoutine 之间的可能竞争关系
1. CustomTimerDPC 先获取 SpinLock，但在尚未调用 IoCompleteRequest 完成IRP之前，IRP 被 Cancel，于是 EventCancelRoutine 开始运行并阻塞于 SpinLock
2. 在定时器触发之前，IRP 被 Cancel，于是 EventCancelRoutine 先获取 SpinLock，但在 EventCancelRoutine 取消定时器之前，定时器触发，于是 CustomTimerDPC 运行并阻塞于SpinLock
3. CustomTimer 和 EventCancelRoutine 各自运行期间，对方都没有运行。

* EventCleanup 与 CustomTimerDPC 和 EventCancelRoutine 之间的可能竞争关系
1. EventCleanup 先获取 SpinLock，但在取消定时器之前，定时器触发，于是 CustomTimerDPC 开始运行并阻塞于 SpinLock
2. EventCleanup 先获取 SpinLock，CustomTimerDPC 未运行，但 EventCancelRoutine 已经在运行。
3. EventCleanup 先获取 SpinLock，CustomTimerDPC 未运行，EventCancelRoutine 也没有运行

上面就是这个例子中可能存在的所有竞争关系，然后驱动中对于竞争关系的处理可以这样理解 - 只要上述三者存在竞争关系（即3者同时在处理一个IRP），不管是谁先获得 SpinLock，IRP 最终的处理都交由优先级最高那个去做。三者的优先级关系如下：
    
    EventCleanup < CustomTimerDPC < EventCancelRoutine

OK, 上面这些就是我从这个例子中学到的最主要的东西。另外在搞清楚这些东西的同时，比较有感触的一点是看代码时千万别陷到代码里，代码是用来表现逻辑并处理逻辑的，理清代码背后的逻辑才是王道。

下面我们说下，这个例子中涉及到的一些小的技术点。

### [Remove Locks][1]
首先说说对这个东西的理解，可以说它就是一个带计数功能的 KEVENT 对象，这一点我们可以中下面它的结构定义中看到。

    typedef struct _IO_REMOVE_LOCK_COMMON_BLOCK {
        BOOLEAN     Removed;
        BOOLEAN     Reserved [3];
        __volatile LONG        IoCount;
        KEVENT      RemoveEvent;    // TODO(H): Here
    } IO_REMOVE_LOCK_COMMON_BLOCK;

    typedef struct _IO_REMOVE_LOCK_DBG_BLOCK {
        LONG        Signature;
        ULONG       HighWatermark;
        LONGLONG    MaxLockedTicks;
        LONG        AllocateTag;
        LIST_ENTRY  LockList;
        KSPIN_LOCK  Spin;
        __volatile LONG        LowMemoryCount;
        ULONG       Reserved1[4];
        PVOID       Reserved2;
        PIO_REMOVE_LOCK_TRACKING_BLOCK Blocks;
    } IO_REMOVE_LOCK_DBG_BLOCK;

    typedef struct _IO_REMOVE_LOCK {
        IO_REMOVE_LOCK_COMMON_BLOCK Common;
    #if DBG
        IO_REMOVE_LOCK_DBG_BLOCK Dbg;
    #endif
    } IO_REMOVE_LOCK, *PIO_REMOVE_LOCK;

可以看到，它的核心确实是一个 KEVENT 对象，只不过额外加上了计数功能。另外 Microsoft 的官方文档中也提到，这是 Microsoft 提供的一个用于跟踪在一个设备对象上尚未完成的IO操作的数目，从而进一步用于确定什么时间安全的删除相应的设备对象的官方解决方案。就是说，大多数驱动开发人员都会碰到类似的这种需求，而这个需求是可以有一个统一的解决方案的，于是 Microsoft 就给了这样一个东西，这样就不用每一个程序员去为这个一个需求开发各自的解决方案了。

然后，我更想说的是，Remve Locks的使用环境，在什么情况下，我们需要这个东西，我觉得这个才是更重要的，这个需要我们在思考下。

### [FILE_OBJECT.FsContext][2]
下面是 MSDN 上对这个成员的解释：

> A pointer to whatever optional state a driver maintains about the file object; otherwise, NULL. For file system drivers, this member must point to a FSRTL_ADVANCED_FCB_HEADER header structure that is contained within a file-system-specific structure; otherwise system instability can result. Usually, this header structure is embedded in a file control block (FCB). However, on some file systems that support multiple data streams, such as NTFS, this header structure is a stream control block (SCB).
>
> Note  In a WDM device stack, only the functional device object (FDO) can use the two context pointers. File system drivers share this member across multiple opens to the same data stream.
> 
> Drivers can use the FsContext and FsContext2 members to maintain driver-determined state about an open file object. A driver cannot use these members unless the file object is accessible in the driver's I/O stack location of an IRP.


这里我的理解是该成员是留给驱动作者用于给特定 FILE_OBJECT 对象关联一段自己特定的数据用的。上面提到的 File System Drivers 对这个成员的使用，对我们来说，正是一个参考。

OK，就这么多吧，其他零碎的东西就先不说了，先告一段落，不然在拖下去，耐心就没了.

[1]:    https://msdn.microsoft.com/zh-cn/library/ff565504(v=vs.85).aspx    "Using Remove Locks"
[2]:    https://msdn.microsoft.com/en-us/library/windows/hardware/ff545834(v=vs.85).aspx    "FILE_OBJECT structure"