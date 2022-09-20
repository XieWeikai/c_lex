# c语言词法分析器

## 注释

两种注释

```c
// 行注释
/*  块注释 */ 
```

## 关键字

```c
for
while
continue
break
if
else
switch
case
default
do
struct
union
enum
static
extern
typedef
return
const

char
short
int 
float
double
unsigned
void
```

上面列出了c语言的关键字（非全部）。

## 标识符

```c
letter [A-Z]
num [0-9]
id ({letter}|_)(letter|num|_)*
```

## 数字

```c
123 // integer  ({num}{-}[0])num*
123.456 // float integer.num+
0x3ef   // hex    0x[0-9a-f]+
0174    // oct    0[0-7]+
1.34e3  // scientific notation  ...
```

## 运算符
### 关系运算符
```c
< > <= >= == !=
```
### 逻辑运算符

```c
&& || !
```

### 位运算

```c
& | ~ ^ << >>
```

### 算数运算

```c
+ - * / ++ -- %
```

### 赋值

```c
= += -= *= /=
```

## 标点符号

```c
? : , ; . ( ) { }
```

## 空白符

```c
' ' '\n' '\r'
```

## 字符串

```c
"jdifn"  // 暂时只匹配这种形式的字符串
```

## 字符

```c
'a'  // 暂时就这种形式吧，不识别'\033'这种
```

## 非法符号

```c
@ $
```



## 额外功能

- 统计各类单词个数、行数、字符数，最后输出统计结果。
- 跳过注释。（注意注释内的东西也要正常统计）
- 检查出词法错误并能恢复，一遍输出全部此法错误
  - 如出现非法字符:$ @「￥之类的
  - 如0x12i3 为非法数字
