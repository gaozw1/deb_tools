1. checkdebs:
check how many packages are in the crp.Implement functions through `wget`

2. compare version:
compare deb packages version by `dpkg --compare-versions`

命令行参考https://mike632t.wordpress.com/2017/03/02/compairing-version-numbers-using-dpkg/

版本定义： 翻译自https://www.debian.org/doc/debian-policy/ch-controlfields.html#s-f-Version

包的版本号，格式为：[epoch:]upstream_version[-debian_revision]。

这三个组成部分：

epoch

这是一个 个位数的（通常很小）无符号整数。可以省略，省略时就假设为零。

当上游版本号方案发生更改时，epoch可以提供帮助，但必须小心使用。即使是在测试阶段，如果没有提前与debian-devel（盲猜debian包的开发者）达成共识，你也不应该改变epoch。

upstream_version（上游版本）

这是版本号的主要部分。如果适用，它通常是已经制作好的.deb文件的原始（“上游”）软件包的版本号。通常，其格式与上游制造商规定的格式相同；然而，它可能需要重新格式化以适应包管理系统的格式和版本比较方案。

包管理系统中关于upstream_version的比较说明如下所述。版本号的upstream_version部分是必需的。

upstream_version只能包含字母数字（只能是A-Za-z0-9）和字符.+-~（句号 、加号、连字符、波浪号）并应以数字开头。如果没有debian_revision，则不允许使用连字符。

debian_revision

此部分根据上游版本指定Debian包的版本。它必须只包含字母数字和字符+.~ （加号、句号、波浪号）并以与upstream_version相同的方式进行比较。

按照惯例，每次upstream_version增加时，debian_revision都会重新从1开始。

软件包管理系统将在字符串的最后一个连字符处（如果有）将版本号分开，以确定upstream_version和debian_revision。没有debian_revision相当于debian_revision为0。

debian_revision部分存在表明此包是on-native package （请参阅Source packages）。不存在就表示该包是native package。

比较两个版本号时，首先比较每个版本号的epoch，如果epoch相等，则比较upstream_version，如果upstream_version相等，则比较debian_revision。epoch会以数值方式进行比较，upstream_version和debian_revision部分则会通过软件包管理系统使用以下算法进行比较：

从左到右比较字符串。

（以下三段流程笔者仍然不是很理解）
首先，确定完全由非数字字符组成的每个字符串的初始部分。两个版本号的此部分（其中一个版本号此部分可能为空）在词汇上进行比较。如果发现差异，则返回。词汇比较是修改后的ASCII值的比较，因此，这部分的号码中带字母的早于非字母的，并且带波浪线的早于任何内容，甚至波浪线在末尾的也会早于任何内容。例如，以下部分版本按从低到高的顺序排列~~,~~a,~,the empty part,a。（~的一个常见用途是用于上游预发布。例如，1.0~beta1~svn1245版本低于1.0~beta1，而1.0~beta1版本低于1.0。）

然后确定由数字字符组成的每个字符串剩余部分的初始部分。比较这两部分的数值，发现的任何差异都将作为比较结果返回。出于这些目的，空字符串（只能出现在正在比较的一个或两个版本字符串的末尾）计为零。

重复这两个步骤（比较和删除初始非数字字符串和初始数字字符串），直到发现差异或两个字符串都用完。