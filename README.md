# 3_rd-sem
My homework from 3-rd semester in MIPT.

## lsn1

* ```mye.c``` - my own soft version of ```echo``` utility. The key ```-n``` is realised.
* ```hello.c``` - print "Hello world" to console by using ```write()``` syscall.

## lsn2

* ```myid.c``` - my own soft version of ``` id ``` utility. Realised searching by uid and username.

## lsn3

* ```mycat.c``` - my own soft version of ```cat``` utility. Realised writing from stdout or from some files. 
* ```mycp.c``` - my own soft version of ```cp``` utility. Realised keys ```-f```, ```-v```, ```-i``` and long analogs. Works in 2 modes:
```
mycp [OPTIONS] source_file destination_file
```
and
```
mycp [OPTIONS] file1 file2 file3 ... destination_folder
```

## lsn4

* ```myfork.c``` - program which creates N streams in program and print it's ```PID``` and ```PPID``` in terminal.
* ```sleepsort.c``` - sorting numbers by sleeping in parallel streams (time depends of number) and printing numbers.
* ```mytime.c``` - utility which measure executing tume of any program by using ```exec``` functions family.
* ```myls.c``` - my own soft version of ```ls``` utility. (in process)
