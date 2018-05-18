## Cloud-Benchmarking
A C based project to benchmark memories and disks of virtual machines on [Chameleon](https://www.chameleoncloud.org/).

#### Memory benchmarking objectives

* Measure the memory speed of your host.
* Your parameter space should include read+write operations, sequential write access, random write access, varying block sizes (8B, 8KB, 8MB, 80MB), and varying concurrency (1 thread, 2 threads, 4 threads, and 8 threads); you should allocate a large piece of memory, and perform read+write or write operations on either sequential or random access within memory. For the latency experiments, you can limit the number of 1B~8B operations to a small number, ensuring that the experiment runs for at least 10s of seconds.
* The metrics you should be measuring are throughput (MB/sec) and latency (microseconds). Vary the number of threads and find the optimal number of concurrency to get the best performance.
* Compute the theoretical memory bandwidth of your memory, based on the type of memory and the speed
* Run the [Stream benchmark](http://www.cs.virginia.edu/stream/) and report the best performance achieved; what efficiency do you achieve compared to the theoretical performance?

#### Disk benchmarking objectives
* Measure the disk speed.
* Your parameter space should include read+write operations, sequential read access, random read access, varying block sizes (8B, 8KB, 8MB, 80MB), and varying the concurrency (1 thread, 2 threads, 4 threads, 8 threads).
* The metrics you should be measuring are throughput (MB/sec) and latency (ms). You should allocate a large piece file (e.g. 10GB), and perform read+write or read operations on either sequential or random access within this 10GB file. For the latency experiments, you can limit the number of 1B~8B operations to a small number, ensuring that the experiment runs for at least 10s of seconds.
* Identify the optimal number of concurrency to get the best performance; explain your findings, putting in perspective the hardware you are testing; can you tell if the disk you are evaluating is a spinning hard drive (HDD) or a solid state memory (SSD) disk?
* Run the [IOZone benchmark](http://www.iozone.org/) and report the best performance achieved; what efficiency do you achieve compared to the theoretical performance? 


**For detailed performance results, refer the report.**
