from dis import dis
s = 'for i in range(0, 10): print(i)'

#compile(source, filename, mode[, flags[, dont_inherit]])
'''
@source字符串或者AST（Abstract Syntax Trees）对象。。
@filename代码文件名称，如果不是从文件读取代码则传递一些可辨认的值。
@mode指定编译代码的种类。可以指定为 exec, eval, single。
@flags变量作用域，局部命名空间，如果被提供，可以是任何映射对象。。
@flags和dont_inherit是用来控制编译源码时的标志
'''
co = compile(s, '', 'exec') #编译python字符串或者文件为字节码

print(dir(co))

print(co.co_code)
print(co.co_names)

dis(co.co_code)#有点像反编译的效果