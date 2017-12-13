# TinyStringBuilder 

### C strings builder - sprintf replacement for embedded use. 

Header only library written in C++11 aimed for embedded usage. Faster than sprintf. 

* **No dynamic memory allocations** & minimal memory foorprint
* C string safe
* **Faster** than sprintf family
* Floating point support (which can be switched off)
* Control over features to build (make it tiny for tiny devices)
* Follow fixed size stdint type family (uint8_t, int16_t, ...)
* **No std library** dependence

Developed with thought of tiny or no-OS embedded applications, where  dynamic memory allocations are not advisable and C++ std:: library is not appliable. 

## Exameple

```cpp
char buffer[100];

CStringBuilder cb(buffer);    // Create cstring builder, give it the buffer
 
cb << "hello world! x = " << 5; // Use << operator overloads
cb.add(5);                      // or do it by hand

puts(cb.cstr());              // Use cstr() to get buffer pointer
if(cb.size()) ...             // Use size() to get actual string size
if(cb.buffer_size()) ...      // Get initial buffer size

```


#### Floating point

```cpp
// Set TCSB_USE_FP 1

cb.addf(4.545) ;
// or
cb << 4.545;
```
Integer and floating point promotions have the same rank and integer and floating 
point conversions have the same rank ([more here][int_float_ambiguity]). 
So we have to use `addf` for floats


### Future optimisation

First, needless to say that even `strnlen` can be implemented with 
a lot of black magic [to perform faster][strnlen_impl].
No doubts this project can be implemented partly in asm + #ifdefs to particular architectures 
so that it is even faster.  


### 



[strnlen_impl]:http://stackoverflow.com/questions/2372315/how-to-implement-strlen-as-fast-as-possible
[int_float_ambiguity]:http://stackoverflow.com/questions/38772637/overload-ambiguous-int-int64-t-vs-int-double
