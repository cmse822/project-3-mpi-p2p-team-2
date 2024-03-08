# Project 3: MPI Ping-Pong and Ring Shift



## Part 1: Ping-Pong test

In the first part of the project, we implemented the Ping-Pong test using both blocking and non-blocking communication. In this regard, the following points are noted:

* For the blocking scheme, the `MPI_Send()` and `MPI_Recv()` functions were used, while for the `MPI_Isend()` and `MPI_Irecv()` used for the non-blocking scheme. 

* The C++ codes can be found under './src' directory, titled: `PingPongBlocking.cpp` and `PingPongNonBlocking.cpp`. In these codes, the message size, number of iterations, and the path to the result csv are inputs.

* The runtime for one complete communication between two ranks were timed using the `MPI_Wtime()` function. 

* 100 ping-pong iteration was used to overcome the noise effect on the timing data. Still, the middle 80 datapoints used for averaging the communication time. 

* A wide range of message sizes were used for this project from 2 bytes to 16 MB. This way, we made sure to collect data for evaluating both latency and bandwidth of the model.

* Both blocking and non-blocking ping-pong test models were run on either same node (on node) and different nodes (off nodes). This process was controlled by `SLURM`. 

    * A constraint of using similar nodes has been used in job file to make sure all runs using similar resources: `#SBATCH --constraint=amr`

    * For the on node analysis, only one node and 2 task per each node was requested, while for the off node analysis, two nodes and one task per node was used.

* During the Ping-Pong process, for a given message size `s`, the amount of data transfering between the first and second processess is $2 \times s$, reason being that the data is send by the first process to the second one, and then the send by the second and recieved by first. But, the timing is for the whole back-forth set. This ponit has been implemented in the calculation of the bandwidth.


The Blocking and Non-blocking ping-pong has been run on HPCC `amr` type of nodes using both single node (on node) and two different physical nodes (off nodes). The average communication time as a function of the message size has been shown below:

![plot_pingpong](Analysis/Fig01_PingPong.png)

As shown in the above figure, the curve for the "off node" results were significantly higher that that of "on node" with a vertical shift. This clearly verifies the application of the off node communication where the messages should travel furhter distances between the actual physical nodes the processess located. 

In more technical ponit of view, this figure shows that the communication run time of the ping-pong test was somehow stabilized up to a certain message size, which can be used to calculate the _latency_. However, after that point, the slope of curve can be used for calculation of the _bandwidth_. In this regard, a linear model has been used to fit to the data as follows:

$T = \alpha + \beta \times x$

where $T$ is the runtime communication, $x$ is the message size (bytes), $\alpha$ is the latency (s), and $\beta$ is used for calculation of the bandwidth using the following equation:

$B = \frac{2}{\beta} \times 10^{-9}$

where $B$ is the bandwidth (GB/s), and we have $2$ at the numinator as the communication time is corresponds to $2 \times s$. 

The _latency_ and _bandwidth_ results from the previous figure has been summarized in the following table. As this table shows, the _latency_ of the system on the `amr` nodes were about $2.36 \mu s$. On the other hand, the _latency_ of the system when two processess were located on two different physical nodes were found to be abount $35.7 \mu s$, which was about 15 times higher than the situation where two processess are located on the same node. 

Senario | _Latency_ ($\mu s$) | _Bandwidth_ ($GB/s$)
--|--|--
On node / Blocking    | 2.47   | 9.65
Off node/ Blocking    | 34.11  | 10.23
On node / NonBlocking | 2.25   | 10.18
Off node/ NonBlocking | 37.42  | 11.21

Moreover, the results showed that the _bandwidth_ for all scenarios (includeing on/off nodes and blocking/nonblocking) were almost similar and about $10 GB/s$. Considering implementation of the high-speed network in HPCC and similar facilities, the _bandwidth_ results verified this fact, where the _bandwidth_ of the on/off node runs were almost identical. However, it should be mentioned that the _bandwidth_ is an indicator of the rate of data transfer, while for ping-pong runs on off nodes, although high _bandwidth_ available through the high speed networks, the runtimes still higher because of the physical distances and communications needed. 



## Part 2: Ring Shift test

1. Implement the MPI ring shift in C or C++ for an arbitrary number of processes in the ring and arbitrary message size (i.e., number of elements per process). In your implementation, use `MPI_Sendrecv()` instead of separate `MPI_Send()` and `MPI_Recv()` calls.
2. As in Parts 1 and 2, vary the message size from 2 bytes to 4 kb, in powers of 2. Also vary the number of processes used from 2 to `N`, in powers of 2, where `N` is sufficiently large that rank 0 and rank `N-1` are guaranteed to reside on separate nodes (`N` will depend on which cluster you are using on HPCC).
3. Compute the bandwidth and latency, as above. Plot the bandwidth as a function of message size. Include separate lines for each number of processes used. 
4. Analyze and discuss your results. Explain the behavior of the resulting curves.

## Part 4: Non-blocking MPI Ring Shift

Repeat Part 3 but using non-blocking communication via `MPI_Isendrecv()`. Compare the results to the blocking case.

## What to turn-in

To your git project repo, commit your final working code for the above exercises and a concise write-up including all plots, and detailed responses to the questions posed concerning your results. 
