# c-g
practice task of Bingyan Studio

## 3.11
在百度 / b 站无用搜索了 2h+，查到一堆零碎的知识。
终于开始阅读《编译原理》，阅读到 P 55。

## 3.12
开始写词法分析器，参考
https://blog.csdn.net/weixin_42529594/article/details/105622166
《编译原理》附录

## 3.15
参考《编译原理》完成简易版 文法。
根据 文法 ，采取最朴素的 预测分析法 ，完成最朴素的 文法分析器，构造 AST

## 3.16
基本完成绘制流程图

## 3.17
完成 break/contiue/return 语句跳转。

## 感想/题外话
最开始接到这个任务时，我十分茫然。这是一个熟悉又陌生的领域：熟悉是因为我使用 c++ 很久了，顺带也比较熟悉 c 语言；但陌生是因为我完全没有关注语言的内在逻辑。换句话说，就是会说“汉语”，但完全不知道“汉语”为什么这么说。所以我花了一个下午的时间在百度/b站搜索 “词法”、“AST”、“流程图” 等关键字，可是始终没有一个回答/教程能把三者完整地串在一起。然后我只能选择 《编译原理》。一翻开书，我十分激动，因为这本书系统又详细地介绍编译的原理，我相信我需要的它都有；但我也很恐惧，这几百页的砖头不是一会能啃下来的。说实话，发现这本书时我心里顿时轻松了，只要肯下功夫读，就一定有收获。

于是我开始读。一开始我是像先完整读一遍，至少读完语法分析。事实证明，我错了。在我读完 50+ 时，我发现基本看不懂了，而且之前看过的也忘光了。合上书无法复述，至少无法理清思路。所以我直接按照自己的理解开始写 词法分析器，虽然之后不一定能无缝衔接，但也能加深对词法/文法的理解。

写完后，我决定再读一遍。这次我十分仔细，每读到新的部分就停下来思考能不能用到程序里。当我读完预测分析法后我就知道，我已经能开始写了。所以我立即停止阅读，直接开始上手。

这次我吸取教训，先花了快一整天（的空闲时间）编写文法，因为预测分析无法处理 First(a) 一样的情况，也无法处理左递归，所以我针对性地查阅 《编译原理》，消除左递归。

之后就是重复性劳动，用最朴素的方法，敲最多的代码，画最丑的图（哭

但当 graphviz 真的画出流程图时，我心中的喜悦激动难以言表。虽然不美观，线乱七八糟，还有一堆不规范的地方，但至少能跑，很是欣慰。

这段时间我收获很大。首先是清楚地认识到我到底有多少空闲时间，看来人必须得逼自己一把（。其次算是 《编译原理》 开了个头，也产生继续阅读的动力。最后就是踏出了零到一的一步，我已经从只会写短代码的菜鸡，进化成屎山+bug生成器了！

我的任务完成了，之后就交给命运了。

一切都是 steins gate 的选择！
