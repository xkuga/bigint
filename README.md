BigInt
======

update: 2014.06.09 by xkuga

support operation: +, -, *, /, ^, %, ^%

the program is implement by binary

the BigInt use complement to store and calculate

multiplication: implement by using booth algorithm

division: implement by using binary search

pow and powmod: implement by using binary way

maybe the program is not very fast, but it works anyway ><

what's more, it's easy to learn and understand how the computer works

if you find any mistakes, you can contact me on weibo(http://weibo.com/cpl86)

thank you!



大整型
------

update: 2014.06.09 by xkuga

程序实现了这几种大数运算：+, -, *, /, ^, %, ^%

采用的是二进制的方式实现, BigInt的存储与运算均采用补码

乘法：采用的是Booth一位乘

除法：用二分法去做减法

幂运算与模幂运算：都是采用二进制的方式实现的，并非一个一个乘

具体实现原理可看微博@C程序设计语言：http://www.weibo.com/cpl86

虽然可能不是很快，不过还是能用><

而且程序比较易懂，对于理解计算机如何运算有一定的帮助

如果有任何错误或改进可在微博中@C程序设计语言，谢谢!