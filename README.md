# Welcome to Plot!

Super-simple plotting library, firmer part of [Ironic](https://github.com/Meta-chan/ironic_library). All you need to know is:

```c++
Plot p;
p.xfunc = [](void*, unsigned int i) { return i * 2 * M_PI / 1000; };
p.yfunc = [](void*, unsigned int i) { return sin(i * 2 * M_PI / 1000); };
p.n = 1000;
p.color = 0xFF0000;
Plot::plot(1, p);
```

Have fun!

###### P.S. My code is not dirty, it is alternatively clean.