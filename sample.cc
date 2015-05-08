#include <iostream>
#include <fstream>
#include "metrics.hpp"

// see also table_ascii, table_csv, table_tsv and table_markdown variants
metrics::table_markdown tbl; 

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
