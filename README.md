# TinyStringBuilder 

### C strings builder - sprintf replacement for embedded use. 


Header only library written in C++11 aimed for embedded usage. Faster than sprintf. 

* No memory allocations & minimal memory foorprint
* C string safe
* **Faster** than sprintf family
* Floating point support (which can be switched off)
* Configurable at build time
* **No std library** dependence

Developed with thought of tiny or no-OS embedded applications, where  dynamic memory allocations are not advisable and C++ std:: library is not appliable. 

## Exameple

```cpp
char buffer[100];

CStringBuilder cb(buffer);    // Create cstring builder, give it the buffer
 
cb.add("hello world! x = ");  // Call add fill the buffer
cb.add(5);                    // add have several useful overloads

puts(cb.cStr());              // Use cStr() to get buffer pointer
if(cb.size()) ...             // Use size() to get actual string size

```


#### Floating point

```cpp
// Set TCSB_USE_FP 1

cb.addf(4.545) 

```
Integer and floating point promotions have the same rank and integer and floating 
point conversions have the same rank ([more here][int_float_ambiguity]). 
So we have to use `addf` for floats


### Future optimisation

First, needless to say that even `strnlen` can be implemented with 
a lot of black magic [to perform faster][strnlen_impl] . 
No doubts this project can be implemented partly in asm and with #ifdefs to particular architectures 
so that it is even faster.  


### 



[strnlen_impl]:http://stackoverflow.com/questions/2372315/how-to-implement-strlen-as-fast-as-possible
[int_float_ambiguity]:http://stackoverflow.com/questions/38772637/overload-ambiguous-int-int64-t-vs-int-double
