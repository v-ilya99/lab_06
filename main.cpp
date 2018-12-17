#include <iostream>
#include "picosha2.hpp"
#include <thread>
#include <stdio.h>
#include <boost/log/trivial.hpp>

using namespace std;

void testHash()
{
    string test = "test", test2 = "1234", res;


    picosha2::hash256_one_by_one hash;

    hash.init();
    hash.process(test.begin(), test.end());
    hash.finish();

    res = picosha2::get_hash_hex_string(hash);
    cout << "sha256(" << test << ") = " << res << endl;

    hash.init();
    hash.process(test2.begin(), test2.end());
    hash.finish();

    res = picosha2::get_hash_hex_string(hash);
    cout << "sha256(" << test2 << ") = " << res << endl;
}

bool work = true;

void thrFunc()
{
    string s, res;
    int i;
    int cnt = 0;

    picosha2::hash256_one_by_one hash;

    for( ; work; cnt++)
    {
        s = "";
        for(i = 0; i < 35; i++)
            s += '0' + (rand() % 10);

        hash.init();
        hash.process(s.begin(), s.end());
        hash.finish();

        res = picosha2::get_hash_hex_string(hash);

        bool fl = true;
        for(i = res.length() - 4; i < res.length(); i++)
            if (res[i] != '0')
                fl = false;
        if (fl)
        {
            cout << "sha256(" << s << ") = " << res << "\n";
            //BOOST_LOG_TRIVIAL(info) << "sha256(" + s + ") = " + res + " => Match";
            work = false;
        }
        //BOOST_LOG_TRIVIAL(trace) << "sha256(" + s + ") = " + res; // << (fl ? " => Match" : "" )
    }
    cout << "Exiting. Processed " << cnt << " hashes\n";
}

int main()
{
    testHash();

    int thr_c = thread::hardware_concurrency(), i;
    cout << "threads: " << thr_c << endl;

    thread thr[100];

    // thrFunc();

    // thr_c = 1;

    for(i = 0; i < thr_c; i++)
        thr[i] = thread(thrFunc);

    for(i = 0; i < thr_c; i++)
        thr[i].join();

    return 0;
}
