# Faulty-cache-simulator

A trace driven cache simulator equipped with the functionality of a well-known cache fault tolerant scheme is provided through this project. This project can be used for simulating the basic cache statistics for a given cache configuration and for a specific memory stream. The current implementation supports cache sub-block disabling of various block granularities. 
The current implementation was used to provide the trace-based results of the following published work: 
“F. Filippou, G. Keramidas, M. Mavropoulos, and D. Nikolos. A Novel Fault Tolerant Cache Architecture Based on Orthogonal Latin Squares Theory. Proc. of Intl. Conference on Design, Automation, and Test in Europe, 2018.,”. [ieee-explore link](https://ieeexplore.ieee.org/document/8342236).

**The supported schemes are the following:**
 -	*Clean cache*
 -	*Faulty cache – Block disabling scheme*
 -	*Faulty cache – Subblock disabling scheme (2/4/8 subblocks)*

The simulating scheme can be easily selected through a configuration header file. 


**Directories:**
- *run_scripts*: You can find two sample bash scripts, which can be used to configure the simulator appropriately for each specific case.
- *polybench_traces*: A subset of [polybench benchmarks](https://web.cse.ohio-state.edu/~pouchet.2/software/polybench/) used to extract the first level data cache (DL1 cache) traces. Binaries built for the SMALL_DATASET input size and traces have been created using gem5/x86 simulator. Traces provide information of the requested command, with the following order: the requested cache address, the PC of the requested instruction, the type of the command (read/write) and the timestamp. Traces of the whole Polybench benchmark suite, can be downlowded [here](http://students.ceid.upatras.gr/~mavropoulo/polybench_traces.tar.gz).  
- *fault_maps*: A small number of fault maps and five fault probabilities provided under this directory. Use different fault maps and various probability to simulate the desired fault scenario. 

The implementation of this work was part of my phd, under the supervision of *prof. Dimitris Nikolos* and *prof. Georgios Keramidas*. 

*Michail Mavropoulos*
- [mavropoulo@ceid.upatras.gr](mailto:mavropoulo@ceid.upatras.gr)
- [http://students.ceid.upatras.gr/~mavropoulo/](http://students.ceid.upatras.gr/~mavropoulo/)
