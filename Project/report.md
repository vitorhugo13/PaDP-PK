# PaDP project - Vítor Gonçalves

## Exercise 2
**Files required:** ```ex2.c```, ```tmeas.c``` and ```tmeas.h``` (these last two files were reused from laboratory classes to calculate the program's execution time.)

**Program execution:** ```gcc ex2.c -o ex2 -lpthread -lm tmeas.c``` followed by ```./ex2 <number of subintervals of integration> <number of threads>```

**Example:** ```./ex2 1000 10```

![](https://i.imgur.com/zq2w16Q.png)

**Comparing runtimes using different numbers of threads**
* To study how the number of threads varies the execution time of programs, the program was executed using the same number of integration subintervals (1000), varying only the number of threads.
* The result can be seen in the chart below. 

![](https://i.imgur.com/GqSMfXv.png)
Where the x_axis is the number of threads used, and the execution time, in seconds, is present in the y_axis.

## Exercise 3
**Files required:** ```ex3.c```, ```tmeas.c``` and ```tmeas.h``` (these last two files were reused from laboratory classes to calculate the program's execution time.)

**Program execution:** ```gcc ex3.c -o ex3 -lpthread -lm tmeas.c``` followed by ```./ex3 <x_value> <n_value> <number of threads>```

**Example:** ```./ex3 4 100 10```

![](https://i.imgur.com/uWBoHob.png)


**Comparing runtimes using different numbers of threads**
* To study how the number of threads varies the execution time of programs, the program was executed using the same value for x and n, varying only the number of threads.
* The result can be seen in the chart below. 

![](https://i.imgur.com/JQZgFF8.png)

* x_value used: 4
* n_value used: 200
* Where the x_axis is the number of threads used, and the execution time, in seconds, is present in the y_axis.

## Exercise 4
**Files required:** ```ex4.c```

**Program execution:** ```gcc ex4.c -o ex4 -lpthread -lm``` followed by ```./ex4 <number of elements of the sum>```

**Example:** ```./ex4 1000``` while P = 5

![](https://i.imgur.com/FCkdPB7.png)

## Exercise 5
**Files required:** ```ex5.c```

**Program execution:** ```gcc ex5.c -o ex5 -fopenmp``` followed by ```./ex5 <number of threads>```

**Example:** ```./ex5 10```

![](https://i.imgur.com/U8qspUw.png)

**Note:** To be able to verify that the program solves systems of equations of other dimensions and with other values, it is necessary to change the value of the variables ```dimension```, ```matrix``` and ```b``` in the ex5.c file.


## Exercise 6
**Files required:** ```ex6.c```

**Program execution:** ```gcc ex6.c -o ex6 -lm -fopenmp``` followed by ```./ex6 <upper limit> <number of threads>```

**Example:** ```./ex6 100 10``` 

![](https://i.imgur.com/BXi1Eqy.png)
