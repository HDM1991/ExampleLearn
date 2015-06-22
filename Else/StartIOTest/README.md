### 关于对 StartIo  Routine 的学习和思考
首先说下这篇文章的大致思路，文章的思路就是以一个使用者在使用 StartIo Routine 时的疑问为主线，然后在结合具体的源码和例子，讲自己的一些认识和看法。不过在这片文章中我不会直接引用相关的源码，但会给出这些源码和例子的地址，大家需要的可以自己研究。OK，下面就让我们从我看到这东西的第一个疑问说起。

#### 疑问一 StartIo Routine 由谁调用的，运行在什么样的上下文中？
通过查看[相关源码][1]发现，IoStartPacket 和 IoStartNextPacket 中都有对 StartIo Routine 的调用，而 IoStartPacket 是由 Dispatch Function 调用的，IoStartNextPacket 是由 StartIo Routine 或者 Cancel Routine 调用的，综合上述调用关系我们可以看到，StartIo Routine 一般运行在同 Dispatch Function 和 Cancel Routine 一样的上下文中。很奇怪是不是？最初对 StartIo Routine 思考时，我觉得应该会有一个单独线程去处理这个 IRP 队列，应该说这是比较符合正常人的思路的。但事实却不是这样，没有单独的线程，StartIo Routine 通常情况下就运行在同 Dispath Function 一样的上下文中，没有什么单独的线程，这也许可以称之为巧妙，但也可以说让人理解其处理流程会很吃力。具体细节大家可以看 IoStartPacket 和 IoStartNextPacket 的实现代码。

#### 疑问二 StartIo Routine 的目的是帮助我们串行化异步 IRP ，那它到底在那些方面帮助了我们？我们还需要做什么？
从源码来看，我觉得 StartIo Routine 给我们最大的帮助就是对 IRP 队列的管理，不需要我们自己去维护一个 IRP 队列。但我觉得这也是它唯一做的事，可能有人会说它在 IRP 的正常完成过程和 Cancel Routine 之间的竞争关系上对我们也有帮助。但在这一点上，我并没有什么大的感觉。因为 StartIo Routine 和 Cancel Routine 之间还是需要手动去处理这种竞争关系，而且每种竞争条件下该做什么处理会让一个没看过 StartIo 相关的源码的人感到很困惑，这一点大家可以从[示例代码][3]上感受下。具体我就不多说了

#### 思考
OK，最后在说说其他一点想法。那就是 StartIo Routine 到底是不是一个好的封装，好的处理方式？从个人的角度来讲，如果不考虑它的处理方式，我会认为它的封装很差劲。但这种封装方式真的和它内部的处理方式没有关系吗？换个说法说，是不是就是因为它的这种实现方式导致了现在这种封装，呈献给我们一个让人困惑重重的接口。个人觉的，要不 Microsoft 就不要提供 StartIo Routine，要不就提供一种更容易让人理解的接口或者提供关于这种机制处理流程，处理思想的更多资源。

另外，假如我们不使用 StartIo Routine，而自己去完成关于 IRP 串行处理的相关东西，难吗？我觉得不难。核心处理应该就下面两点：

1. 链表操作的同步处理
2. 与 Cancel Routine 之间的同步处理

关于 1 无需多言。那 2 呢？真的很难处理吗？我觉的不难，特别是在看过 eventsample 之后。因为 eventsample 已经做了这样的处理，而且它处理了更复杂的同步问题。所以，我觉得应该可以自己开发出比 StartIo 更好的处理方式
 
[1]: https://github.com/HDM1991/ExampleLearn/blob/master/WindowsResearchKernel-WRK/WRK-v1.2/base/ntos/io/iomgr/iosubs.c "IoStartPacket 和 IoStartNextPacket 相关源码"
[2]: https://github.com/HDM1991/ExampleLearn/blob/master/WindowsResearchKernel-WRK/WRK-v1.2/base/ntos/ke/devquobj.c "IoStartPacket 和 IoStartNextPacket 调用的对 IRP 队列进行操作的相关函数(如 KeInsertDeviceQueue)"
[3]: https://github.com/HDM1991/ExampleLearn/tree/master/Else/StartIOTest/NT_Driver/Driver.cpp "示例代码"
[4]: https://github.com/HDM1991/ExampleLearn/tree/master/Windows%20Driver%20Kit%20(WDK)%208.1%20Samples/Hardware%20Event%20Sample "Hardware Event Sample"