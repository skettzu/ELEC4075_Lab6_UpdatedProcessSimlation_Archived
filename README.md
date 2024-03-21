Lab Instructions:

Step 1
Make the simulator more interactive by allowing the user to choose the following at the beginning of the simulation:

1. The percentage of Processes in Blocked state at which Processes are swapped out – 80%, 90%, or 100%.
o In the past lab exercise, we had events in the text file that triggered a swap such as P15 is swapped
out; but in this Lab the threshold triggers the swap.

o For example, if the user chooses 80, then as soon as 80% of the processes are in the Blocked state,
swap out a process.

o Therefore, you will need to keep track of the total number of active processes and the number of
processes that are Blocked so that when it meets the threshold it will trigger the swap out.

o Processes are swapped in when other Processes terminate, therefore, there will be no event such as
P11 is swapped in; in your input text file. Instead, a process terminating and moving to the Exit state triggers a swap into memory.

2. The number of Processes swapped out at that point – one Process or two Processes – and, therefore, the number of Processes brought in. We will assume for this exercise that the processes are the same size
therefore if you swap out two processes you have enough space in memory to swap in two processes.

If there are several Blocked processes in memory you will decide which you want to swap out – you can use the
process that has been blocked the longest, assign priorities of your choice and use the priorities, choose a random
process, etc. – any method that your group decides.

For Step 1, you can collaborate with anyone in the class and with as many classmates as you want – there is no
restriction on group size for this Step. This Step is just so that you have a proper simulator to use to design the
experiment in Step 2.

Step 2
For Step 2 you may work in a group of up to three students.
Design an experiment that fully tests your simulator. The experiment must test all six possible combinations by varying
the percentage (80%, 90%, or 100%) as well as the number of swapped processes (one or two). The experiment must
also choose the best combination based on the least overall overhead latency.
To do this you will also need to design one or more input text files like those in Lab Exercise 2. You should have
at least 20 Processes in the system – the order of events is up to you, use whatever you think you need to test the
swapping in and out. For a proper experiment, the events in the input files should trigger the swapping in and out
several times in one simulation.
Consider the following conditions:
• There is a fixed penalty or latency involved whenever Processes are swapped in or out. Each time the OS
detects the percentage, a fixed penalty or latency must be added to the overall overhead latency. When
Processes terminate and the OS needs to swap in Processes, a fixed penalty or latency must be added to the
overall overhead latency. Therefore, the frequency of swapping affects the overall overhead latency.
• The bus is wide enough to transfer one process at a time between main memory and the disk. Therefore,
swapping in or out two Processes increases the overall overhead latency more than swapping in or out a single
process