# dag-opt

To build
```
$ cmake .
$ make
```
Usage
```                                                                 
usage: ./opt [OPTIONS] algorithm dag e1 e2 ... en
       -t: show timing information
       -m <minimum period>
       -M <maximum period>
       -s <period step>
       -a <alpha>
       -b <beta>
       -u <utilization bound>
       algorithm: [exh|our]
       dag: DAG type [a|b|c|d]
       e1 e2 ... en: execution times for each runnable
```
