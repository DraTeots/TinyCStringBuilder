# TinyStringBuilder 

### C strings builder - sprintf replacement for embedded use. 


Header only library written in C++11 for embedded usage. Faster than sprintf.

* No memory allocations & minimal memory foorprint
* C string safe
* Faster then sprintf family


### Future optimisation

First, needless to say that even `strnlen` can be implemented with 
a lot of black magic [to perform faster][strnlen_impl] to be faster. 
Without doubts this project can be implemented partly in asm and with #ifdefs to particular architectures 
so that it is even faster.  


### 



[strnlen_impl]:http://stackoverflow.com/questions/2372315/how-to-implement-strlen-as-fast-as-possible
