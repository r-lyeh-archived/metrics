metrics <a href="https://travis-ci.org/r-lyeh/metrics"><img src="https://api.travis-ci.org/r-lyeh/metrics.svg?branch=master" align="right" /></a>
=====

- Metrics is a pretty table printer in CSV,TSV,ASCII,markdown (C++11).
- Metrics provides handy unit conversions and minimal benchmarking.
- Metrics provides additional accumulation, average, minimum and maximum stats.
- Metrics is ZLIB licensed.

## tl;dr
```c++
#include <iostream>
#include <fstream>
#include "metrics.hpp"

// see also table_ascii, table_csv, table_tsv and table_markdown variants
metrics::table_ascii tbl;

template<typename A, typename B, typename C, typename D, typename E>
void sample( A a, B b, C c, D d, E e ) {
    using namespace metrics;
    tbl << a << b << KiB(c) << KiB(d) << ms(e) << MiB(c)/s(e);
};

int main() {
    using namespace metrics;
    auto taken = bench( [&] { for(unsigned long long i = 0; i < 3276800; ++i); } );

    tbl
        .add_column_left("file")
        .add_column_right("line")
        .add_column_right("raw size (KiB)")
        .add_column_right("compressed size (KiB)")
        .add_column_right("time (ms)")
        .add_column_right("compression speed (MiB/s)")
        .with_horizontal_padding(1)
        .with_header_every_nth_row(10)
        .with_decimal_digits(3)
        //.with_odd_even_shadow(1)
    ;

    sample( __FILE__, __LINE__, GiB(1), KiB(1000), ms(200) );
    sample( __FILE__, __LINE__, MiB(100), KiB(2000), s(taken * 2) );
    sample( __FILE__, __LINE__, MiB(100), KiB(3000), s(taken * 3) );
    sample( __FILE__, __LINE__, GiB(1), KiB(4000), s(taken * 4) );
    sample( __FILE__, __LINE__, KiB(100), KiB(5000), s(taken * 5) );
    sample( __FILE__, __LINE__, KiB(100), KiB(6000), s(taken * 6) );

    tbl.print( std::cout );

    std::ofstream file( "metrics.txt" );
    tbl.print( file );
}
```

## possible output (markdown)
| file      |    line |  raw size (KiB) | compressed size (KiB) |  time (ms) | compression speed (MiB/s) |
|:----------|--------:|----------------:|----------------------:|-----------:|--------------------------:|
| sample.cc |      31 | 1048576.000 KiB |          1000.000 KiB | 200.000 ms |            5120.000 MiB/s |
| sample.cc |      32 |  102400.000 KiB |          2000.000 KiB |  17.400 ms |            5747.037 MiB/s |
| sample.cc |      33 |  102400.000 KiB |          3000.000 KiB |  26.100 ms |            3831.358 MiB/s |
| sample.cc |      34 | 1048576.000 KiB |          4000.000 KiB |  34.801 ms |           29424.829 MiB/s |
| sample.cc |      35 |     100.000 KiB |          5000.000 KiB |  43.501 ms |               2.245 MiB/s |
| sample.cc |      36 |     100.000 KiB |          6000.000 KiB |  52.201 ms |               1.871 MiB/s |
| 0.000     | 201.000 |     2302152.000 |             21000.000 |    374.003 |                 44127.340 |
| 0.000     |  33.500 |      383692.000 |              3500.000 |     62.334 |                  7354.557 |
| 0.000     |  31.000 |         100.000 |              1000.000 |     17.400 |                     1.871 |
| 0.000     |  36.000 |     1048576.000 |              6000.000 |    200.000 |                 29424.829 |

## possible output (csv)
```
  file      ,    line ,  raw size (KiB) , compressed size (KiB) ,  time (ms) , compression speed (MiB/s)
  sample.cc ,      31 , 1048576.000 KiB ,          1000.000 KiB , 200.000 ms ,            5120.000 MiB/s
  sample.cc ,      32 ,  102400.000 KiB ,          2000.000 KiB ,  25.643 ms ,            3899.732 MiB/s
  sample.cc ,      33 ,  102400.000 KiB ,          3000.000 KiB ,  38.464 ms ,            2599.821 MiB/s
  sample.cc ,      34 , 1048576.000 KiB ,          4000.000 KiB ,  51.286 ms ,           19966.626 MiB/s
  sample.cc ,      35 ,     100.000 KiB ,          5000.000 KiB ,  64.107 ms ,               1.523 MiB/s
  sample.cc ,      36 ,     100.000 KiB ,          6000.000 KiB ,  76.928 ms ,               1.269 MiB/s
  0.000     , 201.000 ,     2302152.000 ,             21000.000 ,    456.428 ,                 31588.971
  0.000     ,  33.500 ,      383692.000 ,              3500.000 ,     76.071 ,                  5264.829
  0.000     ,  31.000 ,         100.000 ,              1000.000 ,     25.643 ,                     1.269
  0.000     ,  36.000 ,     1048576.000 ,              6000.000 ,    200.000 ,                 19966.626
```

## possible output (tsv)
```
  file              line          raw size (KiB)         compressed size (KiB)    time (ms)      compression speed (MiB/s)
  sample.cc           31         1048576.000 KiB                  1000.000 KiB   200.000 ms                 5120.000 MiB/s
  sample.cc           32          102400.000 KiB                  2000.000 KiB    12.359 ms                 8091.500 MiB/s
  sample.cc           33          102400.000 KiB                  3000.000 KiB    18.538 ms                 5394.333 MiB/s
  sample.cc           34         1048576.000 KiB                  4000.000 KiB    24.717 ms                41428.481 MiB/s
  sample.cc           35             100.000 KiB                  5000.000 KiB    30.897 ms                    3.161 MiB/s
  sample.cc           36             100.000 KiB                  6000.000 KiB    37.076 ms                    2.634 MiB/s
  0.000          201.000             2302152.000                     21000.000      323.587                      60040.109
  0.000           33.500              383692.000                      3500.000       53.931                      10006.685
  0.000           31.000                 100.000                      1000.000       12.359                          2.634
  0.000           36.000             1048576.000                      6000.000      200.000                      41428.481
```

## possible output (ascii)
```lisp
+--------+
| metrics \----------------------------------------------------------------------------------------------+
| file      |    line |  raw size (KiB) | compressed size (KiB) |  time (ms) | compression speed (MiB/s) |
+--------------------------------------------------------------------------------------------------------+
| sample.cc |      31 | 1048576.000 KiB |          1000.000 KiB | 200.000 ms |            5120.000 MiB/s |
| sample.cc |      32 |  102400.000 KiB |          2000.000 KiB |  13.505 ms |            7404.609 MiB/s |
| sample.cc |      33 |  102400.000 KiB |          3000.000 KiB |  20.258 ms |            4936.406 MiB/s |
| sample.cc |      34 | 1048576.000 KiB |          4000.000 KiB |  27.010 ms |           37911.597 MiB/s |
| sample.cc |      35 |     100.000 KiB |          5000.000 KiB |  33.763 ms |               2.892 MiB/s |
| sample.cc |      36 |     100.000 KiB |          6000.000 KiB |  40.515 ms |               2.410 MiB/s |
+--------------------------------------------------------------------------------------------------------+
+----------+
| stats-acc \--------------------------------------------------------------------------------------------+
| 0.000     | 201.000 |     2302152.000 |             21000.000 |    335.051 |                 55377.914 |
+----------+
| stats-avg \--------------------------------------------------------------------------------------------+
| 0.000     |  33.500 |      383692.000 |              3500.000 |     55.842 |                  9229.652 |
+----------+
| stats-min \--------------------------------------------------------------------------------------------+
| 0.000     |  31.000 |         100.000 |              1000.000 |     13.505 |                     2.410 |
+----------+
| stats-max \--------------------------------------------------------------------------------------------+
| 0.000     |  36.000 |     1048576.000 |              6000.000 |    200.000 |                 37911.597 |
+--------------------------------------------------------------------------------------------------------+
```

## todo
- [ ] htmls/charts/svg output

## licenses
- [metrics](https://github.com/r-lyeh/metrics), ZLIB licensed.
- [auto_table](http://ideone.com/k7Jcfy) by Dmitry Ledentsov, public domain (?).
- [units](https://github.com/r-lyeh/units) by Calum Grant, BOOST licensed.

## changelog
- v1.0.0 (2015/05/08)
  - initial commit
