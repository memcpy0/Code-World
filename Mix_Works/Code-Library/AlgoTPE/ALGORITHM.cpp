   1 #include "algorithm.hpp"
   2 #include <algorithm>
   3 #include <iostream>
   4 #include <vector>
   5 #include <cctype>
   6 #include <array>
   7 #include <ctime>
   8 #include <cstdlib>
   9 #include <string>
  10 #include <random>
  11 #include <chrono>
  12  
  13 // reference: http://www.cplusplus.com/reference/algorithm/
  14  
  15 namespace algorithm_ {
  16  
  17 ///////////////////////////////////////
  18 static bool myfunction(int i, int j) { return (i == j); }
  19 static bool comp_case_insensitive(char c1, char c2) { return (std::tolower(c1) == std::tolower(c2)); }
  20 static bool IsOdd(int i) { return ((i % 2) == 1); }
  21  
  22 int test_algorithm_find()
  23 {
  24 {
  25     int myints[] = { 5, 20, 5, 30, 30, 20, 10, 10, 20 };
  26     std::vector<int> myvector(myints, myints + 8);
  27     std::vector<int>::iterator it;
  28  
  29     // using default comparison:
  30     it = std::adjacent_find(myvector.begin(), myvector.end());
  31  
  32     if (it != myvector.end())
  33         std::cout << "the first pair of repeated elements are: " << *it << '\n'; // 30
  34  
  35     //using predicate comparison:
  36     it = std::adjacent_find(++it, myvector.end(), myfunction);
  37  
  38     if (it != myvector.end())
  39         std::cout << "the second pair of repeated elements are: " << *it << '\n'; // 10
  40 }
  41  
  42 {
  43     // using std::find with array and pointer:
  44     int myints[] = { 10, 20, 30, 40 };
  45     int * p;
  46  
  47     p = std::find(myints, myints + 4, 30);
  48     if (p != myints + 4)
  49         std::cout << "Element found in myints: " << *p << '\n'; // 30
  50     else
  51         std::cout << "Element not found in myints\n";
  52  
  53     // using std::find with vector and iterator:
  54     std::vector<int> myvector(myints, myints + 4);
  55     std::vector<int>::iterator it;
  56  
  57     it = std::find(myvector.begin(), myvector.end(), 30);
  58     if (it != myvector.end())
  59         std::cout << "Element found in myvector: " << *it << '\n'; // 30
  60     else
  61         std::cout << "Element not found in myvector\n";
  62 }
  63  
  64 {
  65     int myints[] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };
  66     std::vector<int> haystack(myints, myints + 10);
  67  
  68     int needle1[] = { 1, 2, 3 };
  69  
  70     // using default comparison:
  71     std::vector<int>::iterator it;
  72     it = std::find_end(haystack.begin(), haystack.end(), needle1, needle1 + 3);
  73  
  74     if (it != haystack.end())
  75         std::cout << "needle1 last found at position " << (it - haystack.begin()) << '\n'; // 5
  76  
  77     int needle2[] = { 4, 5, 1 };
  78  
  79     // using predicate comparison:
  80     it = std::find_end(haystack.begin(), haystack.end(), needle2, needle2 + 3, myfunction);
  81  
  82     if (it != haystack.end())
  83         std::cout << "needle2 last found at position " << (it - haystack.begin()) << '\n'; // 3
  84 }
  85  
  86 {
  87     int mychars[] = { 'a', 'b', 'c', 'A', 'B', 'C' };
  88     std::vector<char> haystack(mychars, mychars + 6);
  89     std::vector<char>::iterator it;
  90  
  91     int needle[] = { 'A', 'B', 'C' };
  92  
  93     // using default comparison:
  94     it = find_first_of(haystack.begin(), haystack.end(), needle, needle + 3);
  95  
  96     if (it != haystack.end())
  97         std::cout << "The first match is: " << *it << '\n'; // A
  98  
  99     // using predicate comparison:
 100     it = find_first_of(haystack.begin(), haystack.end(), needle, needle + 3, comp_case_insensitive);
 101  
 102     if (it != haystack.end())
 103         std::cout << "The first match is: " << *it << '\n'; // a
 104 }
 105  
 106 {
 107     std::vector<int> myvector;
 108  
 109     myvector.push_back(10);
 110     myvector.push_back(25);
 111     myvector.push_back(40);
 112     myvector.push_back(55);
 113  
 114     std::vector<int>::iterator it = std::find_if(myvector.begin(), myvector.end(), IsOdd);
 115     std::cout << "The first odd value is " << *it << '\n'; // 25
 116 }
 117  
 118 {
 119     std::array<int, 5> foo = { 1, 2, 3, 4, 5 };
 120  
 121     std::array<int, 5>::iterator it = std::find_if_not(foo.begin(), foo.end(), [](int i){return i % 2; });
 122     std::cout << "The first even value is " << *it << '\n'; // 2
 123 }
 124  
 125     return 0;
 126 }
 127  
 128 ////////////////////////////////////////////
 129 int test_algorithm_all_of()
 130 {
 131 {
 132     std::array<int, 8> foo = { 3, 5, 7, 11, 13, 17, 19, 23 };
 133  
 134     if (std::all_of(foo.begin(), foo.end(), [](int i){return i % 2; }))
 135         std::cout << "All the elements are odd numbers.\n"; // All the elements are odd numbers
 136 }
 137  
 138 {
 139     std::array<int, 7> foo = { 0, 1, -1, 3, -3, 5, -5 };
 140  
 141     if (std::any_of(foo.begin(), foo.end(), [](int i){return i<0; }))
 142         std::cout << "There are negative elements in the range.\n"; // There are negative elements in the range
 143 }
 144  
 145 {
 146     std::array<int, 8> foo = { 1, 2, 4, 8, 16, 32, 64, 128 };
 147  
 148     if (std::none_of(foo.begin(), foo.end(), [](int i){return i<0; }))
 149         std::cout << "There are no negative elements in the range.\n"; // There are no negative elements in the range
 150 }
 151  
 152     return 0;
 153 }
 154  
 155 ////////////////////////////////////////////////
 156 static bool myfunction2(int i, int j) { return (i<j); }
 157 static bool mypredicate(int i, int j) { return (i == j); }
 158  
 159 int test_algorithm_search()
 160 {
 161 {
 162     int myints[] = { 1, 2, 3, 4, 5, 4, 3, 2, 1 };
 163     std::vector<int> v(myints, myints + 9);
 164  
 165     // using default comparison:
 166     std::sort(v.begin(), v.end());
 167  
 168     std::cout << "looking for a 3... ";
 169     if (std::binary_search(v.begin(), v.end(), 3)) std::cout << "found!\n"; // found!
 170     else std::cout << "not found.\n";
 171  
 172     // using myfunction as comp:
 173     std::sort(v.begin(), v.end(), myfunction2);
 174  
 175     std::cout << "looking for a 6... ";
 176     if (std::binary_search(v.begin(), v.end(), 6, myfunction2)) std::cout << "found!\n";
 177     else std::cout << "not found.\n"; // not found.
 178 }
 179  
 180 {
 181     std::vector<int> haystack;
 182  
 183     // set some values:        haystack: 10 20 30 40 50 60 70 80 90
 184     for (int i = 1; i<10; i++) haystack.push_back(i * 10);
 185  
 186     // using default comparison:
 187     int needle1[] = { 40, 50, 60, 70 };
 188     std::vector<int>::iterator it;
 189     it = std::search(haystack.begin(), haystack.end(), needle1, needle1 + 4);
 190  
 191     if (it != haystack.end())
 192         std::cout << "needle1 found at position " << (it - haystack.begin()) << '\n'; // 3
 193     else
 194         std::cout << "needle1 not found\n";
 195  
 196     // using predicate comparison:
 197     int needle2[] = { 20, 30, 50 };
 198     it = std::search(haystack.begin(), haystack.end(), needle2, needle2 + 3, mypredicate);
 199  
 200     if (it != haystack.end())
 201         std::cout << "needle2 found at position " << (it - haystack.begin()) << '\n';
 202     else
 203         std::cout << "needle2 not found\n"; // needle2 not found
 204 }
 205  
 206 {
 207     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };
 208     std::vector<int> myvector(myints, myints + 8);
 209  
 210     std::vector<int>::iterator it;
 211  
 212     // using default comparison:
 213     it = std::search_n(myvector.begin(), myvector.end(), 2, 30);
 214  
 215     if (it != myvector.end())
 216         std::cout << "two 30s found at position " << (it - myvector.begin()) << '\n'; // 2
 217     else
 218         std::cout << "match not found\n";
 219  
 220     // using predicate comparison:
 221     it = std::search_n(myvector.begin(), myvector.end(), 2, 10, mypredicate);
 222  
 223     if (it != myvector.end())
 224         std::cout << "two 10s found at position " << int(it - myvector.begin()) << '\n'; // 5
 225     else
 226         std::cout << "match not found\n";
 227 }
 228  
 229     return 0;
 230 }
 231  
 232 //////////////////////////////////////////////
 233 int test_algorithm_copy()
 234 {
 235 {
 236     int myints[] = { 10, 20, 30, 40, 50, 60, 70 };
 237     std::vector<int> myvector(7);
 238  
 239     std::copy(myints, myints + 7, myvector.begin());
 240  
 241     std::cout << "myvector contains:";
 242     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 243         std::cout << ' ' << *it; // 10 20 30 40 50 60 70
 244  
 245     std::cout << '\n';
 246 }
 247  
 248 {
 249     std::vector<int> myvector;
 250  
 251     // set some values:
 252     for (int i = 1; i <= 5; i++)
 253         myvector.push_back(i * 10);          // myvector: 10 20 30 40 50
 254  
 255     myvector.resize(myvector.size() + 3);  // allocate space for 3 more elements
 256  
 257     std::copy_backward(myvector.begin(), myvector.begin() + 5, myvector.end());
 258  
 259     std::cout << "myvector contains:";
 260     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 261         std::cout << ' ' << *it; // 10 20 30 10 20 30 40 50
 262     std::cout << '\n';
 263 }
 264  
 265 {
 266     std::vector<int> foo = { 25, 15, 5, -5, -15 };
 267     std::vector<int> bar(foo.size());
 268  
 269     // copy only positive numbers:
 270     auto it = std::copy_if(foo.begin(), foo.end(), bar.begin(), [](int i){return !(i<0); });
 271     bar.resize(std::distance(bar.begin(), it));  // shrink container to new size
 272  
 273     std::cout << "bar contains:";
 274     for (int& x : bar) std::cout << ' ' << x; // 25 15 5
 275     std::cout << '\n';
 276 }
 277  
 278 {
 279     int myints[] = { 10, 20, 30, 40, 50, 60, 70 };
 280     std::vector<int> myvector;
 281  
 282     myvector.resize(7);   // allocate space for 7 elements
 283  
 284     std::copy_n(myints, 7, myvector.begin());
 285  
 286     std::cout << "myvector contains:";
 287     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 288         std::cout << ' ' << *it; // 10 20 30 40 50 60 70
 289  
 290     std::cout << '\n';
 291 }
 292  
 293     return 0;
 294 }
 295  
 296 ///////////////////////////////////////////////
 297 int test_algorithm_count()
 298 {
 299 {
 300     // counting elements in array:
 301     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };   // 8 elements
 302     int mycount = std::count(myints, myints + 8, 10);
 303     std::cout << "10 appears " << mycount << " times.\n"; // 3
 304  
 305     // counting elements in container:
 306     std::vector<int> myvector(myints, myints + 8);
 307     mycount = std::count(myvector.begin(), myvector.end(), 20);
 308     std::cout << "20 appears " << mycount << " times.\n"; // 3
 309 }
 310  
 311 {
 312     std::vector<int> myvector;
 313     for (int i = 1; i<10; i++) myvector.push_back(i); // myvector: 1 2 3 4 5 6 7 8 9
 314  
 315     int mycount = count_if(myvector.begin(), myvector.end(), IsOdd);
 316     std::cout << "myvector contains " << mycount << " odd values.\n"; // 5
 317 }
 318  
 319     return 0;
 320 }
 321  
 322 //////////////////////////////////////////
 323 static bool mygreater(int i, int j) { return (i>j); }
 324  
 325 int test_algorithm_equal()
 326 {
 327 {
 328     int myints[] = { 20, 40, 60, 80, 100 };               // myints: 20 40 60 80 100
 329     std::vector<int>myvector(myints, myints + 5);       // myvector: 20 40 60 80 100
 330  
 331     // using default comparison:
 332     if (std::equal(myvector.begin(), myvector.end(), myints))
 333         std::cout << "The contents of both sequences are equal.\n"; // equal
 334     else
 335         std::cout << "The contents of both sequences differ.\n";
 336  
 337     myvector[3] = 81;                                 // myvector: 20 40 60 81 100
 338  
 339     // using predicate comparison:
 340     if (std::equal(myvector.begin(), myvector.end(), myints, mypredicate))
 341         std::cout << "The contents of both sequences are equal.\n";
 342     else
 343         std::cout << "The contents of both sequences differ.\n"; // differ
 344 }
 345  
 346 {
 347     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };
 348     std::vector<int> v(myints, myints + 8);                         // 10 20 30 30 20 10 10 20
 349     std::pair<std::vector<int>::iterator, std::vector<int>::iterator> bounds;
 350  
 351     // using default comparison:
 352     std::sort(v.begin(), v.end());                              // 10 10 10 20 20 20 30 30
 353     bounds = std::equal_range(v.begin(), v.end(), 20);          //          ^        ^
 354  
 355     std::cout << "bounds at positions " << (bounds.first - v.begin()); // 3
 356     std::cout << " and " << (bounds.second - v.begin()) << '\n'; // 6
 357  
 358     // using "mygreater" as comp:
 359     std::sort(v.begin(), v.end(), mygreater);                     // 30 30 20 20 20 10 10 10
 360     bounds = std::equal_range(v.begin(), v.end(), 20, mygreater); //       ^        ^
 361  
 362     std::cout << "bounds at positions " << (bounds.first - v.begin()); // 2
 363     std::cout << " and " << (bounds.second - v.begin()) << '\n'; // 5
 364 }
 365  
 366 {
 367     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };
 368     std::vector<int> v(myints, myints + 8);       // 10 20 30 30 20 10 10 20
 369  
 370     std::sort(v.begin(), v.end());                // 10 10 10 20 20 20 30 30
 371  
 372     std::vector<int>::iterator low, up;
 373     low = std::lower_bound(v.begin(), v.end(), 20);
 374     up = std::upper_bound(v.begin(), v.end(), 20);
 375  
 376     std::cout << "lower_bound at position " << (low - v.begin()) << '\n'; // 3
 377     std::cout << "upper_bound at position " << (up - v.begin()) << '\n'; // 6
 378 }
 379  
 380     return 0;
 381 }
 382  
 383 //////////////////////////////////////////
 384 int test_algorithm_fill()
 385 {
 386 {
 387     std::vector<int> myvector(8);                       // myvector: 0 0 0 0 0 0 0 0
 388  
 389     std::fill(myvector.begin(), myvector.begin() + 4, 5);   // myvector: 5 5 5 5 0 0 0 0
 390     std::fill(myvector.begin() + 3, myvector.end() - 2, 8);   // myvector: 5 5 5 8 8 8 0 0
 391  
 392     std::cout << "myvector contains:";
 393     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 394         std::cout << ' ' << *it; // 5 5 5 8 8 8 0 0
 395     std::cout << '\n';
 396 }
 397  
 398 {
 399     std::vector<int> myvector(8, 10);        // myvector: 10 10 10 10 10 10 10 10
 400  
 401     std::fill_n(myvector.begin(), 4, 20);     // myvector: 20 20 20 20 10 10 10 10
 402     std::fill_n(myvector.begin() + 3, 3, 33);   // myvector: 20 20 20 33 33 33 10 10
 403  
 404     std::cout << "myvector contains:";
 405     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 406         std::cout << ' ' << *it; // 20 20 20 33 33 33 10 10
 407     std::cout << '\n';
 408 }
 409  
 410     return 0;
 411 }
 412  
 413 ///////////////////////////////////////////
 414 void myfunction3(int i) {  // function:
 415     std::cout << ' ' << i;
 416 }
 417  
 418 struct myclass {           // function object type:
 419     void operator() (int i) { std::cout << ' ' << i; }
 420 } myobject;
 421  
 422 int test_algorithm_for_each()
 423 {
 424     std::vector<int> myvector;
 425     myvector.push_back(10);
 426     myvector.push_back(20);
 427     myvector.push_back(30);
 428  
 429     std::cout << "myvector contains:";
 430     for_each(myvector.begin(), myvector.end(), myfunction3); // 10 20 30
 431     std::cout << '\n';
 432  
 433     // or:
 434     std::cout << "myvector contains:";
 435     for_each(myvector.begin(), myvector.end(), myobject); // 10 20 30
 436     std::cout << '\n';
 437  
 438     return 0;
 439 }
 440  
 441 ////////////////////////////////////////////////
 442 // function generator:
 443 int RandomNumber() { return (std::rand() % 100); }
 444  
 445 // class generator:
 446 struct c_unique {
 447     int current;
 448     c_unique() { current = 0; }
 449     int operator()() { return ++current; }
 450 } UniqueNumber;
 451  
 452 int current = 0;
 453 int UniqueNumber2() { return ++current; }
 454  
 455 int test_algorithm_generate()
 456 {
 457 {
 458     std::srand(unsigned(std::time(0)));
 459  
 460     std::vector<int> myvector(8);
 461  
 462     std::generate(myvector.begin(), myvector.end(), RandomNumber);
 463  
 464     std::cout << "myvector contains:";
 465     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 466         std::cout << ' ' << *it;
 467     std::cout << '\n';
 468  
 469     std::generate(myvector.begin(), myvector.end(), UniqueNumber);
 470  
 471     std::cout << "myvector contains:";
 472     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 473         std::cout << ' ' << *it; // 1 2 3 4 5 6 7 8
 474     std::cout << '\n';
 475 }
 476  
 477 {
 478     int myarray[9];
 479  
 480     std::generate_n(myarray, 9, UniqueNumber2);
 481  
 482     std::cout << "myarray contains:";
 483     for (int i = 0; i<9; ++i)
 484         std::cout << ' ' << myarray[i]; // 1 2 3 4 5 6 7 8 9
 485     std::cout << '\n';
 486 }
 487  
 488     return 0;
 489 }
 490  
 491 ////////////////////////////////////////////////
 492 int test_algorithm_includes()
 493 {
 494     int container[] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
 495     int continent[] = { 40, 30, 20, 10 };
 496  
 497     std::sort(container, container + 10);
 498     std::sort(continent, continent + 4);
 499  
 500     // using default comparison:
 501     if (std::includes(container, container + 10, continent, continent + 4))
 502         std::cout << "container includes continent!\n"; // container includes continent
 503  
 504     // using myfunction as comp:
 505     if (std::includes(container, container + 10, continent, continent + 4, myfunction2))
 506         std::cout << "container includes continent!\n"; // container includes continent
 507  
 508     return 0;
 509 }
 510  
 511 ///////////////////////////////////////////////////////////
 512 int test_algorithm_merge()
 513 {
 514 {
 515     int first[] = { 5, 10, 15, 20, 25 };
 516     int second[] = { 50, 40, 30, 20, 10 };
 517     std::vector<int> v(10);
 518     std::vector<int>::iterator it;
 519  
 520     std::sort(first, first + 5);
 521     std::sort(second, second + 5);
 522  
 523     it = std::copy(first, first + 5, v.begin());
 524     std::copy(second, second + 5, it);
 525  
 526     std::inplace_merge(v.begin(), v.begin() + 5, v.end());
 527  
 528     std::cout << "The resulting vector contains:";
 529     for (it = v.begin(); it != v.end(); ++it)
 530         std::cout << ' ' << *it; // 5 10 10 15 20 20 25 30 40 50
 531     std::cout << '\n';
 532 }
 533  
 534 {
 535     int first[] = { 5, 10, 15, 20, 25 };
 536     int second[] = { 50, 40, 30, 20, 10 };
 537     std::vector<int> v(10);
 538  
 539     std::sort(first, first + 5);
 540     std::sort(second, second + 5);
 541     std::merge(first, first + 5, second, second + 5, v.begin());
 542  
 543     std::cout << "The resulting vector contains:";
 544     for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it)
 545         std::cout << ' ' << *it; // 5 10 10 15 20 20 25 30 40 50
 546     std::cout << '\n';
 547 }
 548  
 549     return 0;
 550 }
 551  
 552 ////////////////////////////////////////
 553 int test_algorithm_heap()
 554 {
 555 {
 556     std::vector<int> foo{ 9, 5, 2, 6, 4, 1, 3, 8, 7 };
 557  
 558     if (!std::is_heap(foo.begin(), foo.end()))
 559         std::make_heap(foo.begin(), foo.end());
 560  
 561     std::cout << "Popping out elements:";
 562     while (!foo.empty()) {
 563         std::pop_heap(foo.begin(), foo.end());   // moves largest element to back
 564         std::cout << ' ' << foo.back();         // prints back // 9 8 7 6 5 4 3 2 1
 565         foo.pop_back();                         // pops element out of container
 566     }
 567     std::cout << '\n';
 568 }
 569  
 570 {
 571     std::vector<int> foo{ 2, 6, 9, 3, 8, 4, 5, 1, 7 };
 572  
 573     std::sort(foo.begin(), foo.end());
 574     std::reverse(foo.begin(), foo.end());
 575  
 576     auto last = std::is_heap_until(foo.begin(), foo.end());
 577  
 578     std::cout << "The " << (last - foo.begin()) << " first elements are a valid heap:"; // 9
 579     for (auto it = foo.begin(); it != last; ++it)
 580         std::cout << ' ' << *it; // 9 8 7 6 5 4 3 2 1
 581     std::cout << '\n';
 582 }
 583  
 584 {
 585     int myints[] = { 10, 20, 30, 5, 15 };
 586     std::vector<int> v(myints, myints + 5);
 587  
 588     std::make_heap(v.begin(), v.end());
 589     std::cout << "initial max heap   : " << v.front() << '\n'; // 30
 590  
 591     std::pop_heap(v.begin(), v.end()); v.pop_back();
 592     std::cout << "max heap after pop : " << v.front() << '\n'; // 20
 593  
 594     v.push_back(99); std::push_heap(v.begin(), v.end());
 595     std::cout << "max heap after push: " << v.front() << '\n'; // 99
 596  
 597     std::sort_heap(v.begin(), v.end());
 598  
 599     std::cout << "final sorted range :";
 600     for (unsigned i = 0; i<v.size(); i++)
 601         std::cout << ' ' << v[i]; // 5 10 15 20 99
 602  
 603     std::cout << '\n';
 604 }
 605  
 606     return 0;
 607 }
 608  
 609 ////////////////////////////////////////////
 610 int test_algorithm_partition()
 611 {
 612 {
 613     std::array<int, 7> foo{ 1, 2, 3, 4, 5, 6, 7 };
 614  
 615     // print contents:
 616     std::cout << "foo:"; for (int& x : foo) std::cout << ' ' << x;
 617     if (std::is_partitioned(foo.begin(), foo.end(), IsOdd))
 618         std::cout << " (partitioned)\n";
 619     else
 620         std::cout << " (not partitioned)\n"; // not partitioned
 621  
 622     // partition array:
 623     std::partition(foo.begin(), foo.end(), IsOdd);
 624  
 625     // print contents again:
 626     std::cout << "foo:"; for (int& x : foo) std::cout << ' ' << x; // 1 7 3 5 4 6 2
 627     if (std::is_partitioned(foo.begin(), foo.end(), IsOdd))
 628         std::cout << " (partitioned)\n"; // partitioned
 629     else
 630         std::cout << " (not partitioned)\n";
 631 }
 632  
 633 {
 634     std::vector<int> myvector;
 635  
 636     // set some values:
 637     for (int i = 1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9
 638  
 639     std::vector<int>::iterator bound;
 640     bound = std::partition(myvector.begin(), myvector.end(), IsOdd);
 641  
 642     // print out content:
 643     std::cout << "odd elements:";
 644     for (std::vector<int>::iterator it = myvector.begin(); it != bound; ++it)
 645         std::cout << ' ' << *it; // 1 9 3 7 5
 646     std::cout << '\n';
 647  
 648     std::cout << "even elements:";
 649     for (std::vector<int>::iterator it = bound; it != myvector.end(); ++it)
 650         std::cout << ' ' << *it; // 6 4 8 2
 651     std::cout << '\n';
 652 }
 653  
 654 {
 655     std::vector<int> foo{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
 656     std::vector<int> odd, even;
 657  
 658     // resize vectors to proper size:
 659     unsigned n = std::count_if(foo.begin(), foo.end(), IsOdd);
 660     odd.resize(n); even.resize(foo.size() - n);
 661  
 662     // partition:
 663     std::partition_copy(foo.begin(), foo.end(), odd.begin(), even.begin(), IsOdd);
 664  
 665     // print contents:
 666     std::cout << "odd: ";  for (int& x : odd)  std::cout << ' ' << x; std::cout << '\n'; // 1 3 5 7 9
 667     std::cout << "even: "; for (int& x : even) std::cout << ' ' << x; std::cout << '\n'; // 2 4 6 8
 668 }
 669  
 670 {
 671     std::vector<int> foo{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
 672     std::vector<int> odd;
 673  
 674     std::partition(foo.begin(), foo.end(), IsOdd);
 675  
 676     auto it = std::partition_point(foo.begin(), foo.end(), IsOdd);
 677     odd.assign(foo.begin(), it);
 678  
 679     // print contents of odd:
 680     std::cout << "odd:";
 681     for (int& x : odd) std::cout << ' ' << x; // 1 9 3 7 5
 682     std::cout << '\n';
 683 }
 684  
 685 {
 686     std::vector<int> myvector;
 687  
 688     // set some values:
 689     for (int i = 1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9
 690  
 691     std::vector<int>::iterator bound;
 692     bound = std::stable_partition(myvector.begin(), myvector.end(), IsOdd);
 693  
 694     // print out content:
 695     std::cout << "odd elements:";
 696     for (std::vector<int>::iterator it = myvector.begin(); it != bound; ++it)
 697         std::cout << ' ' << *it; // 1 3 5 7 9
 698     std::cout << '\n';
 699  
 700     std::cout << "even elements:";
 701     for (std::vector<int>::iterator it = bound; it != myvector.end(); ++it)
 702         std::cout << ' ' << *it; // 2 4 6 8
 703     std::cout << '\n';
 704 }
 705  
 706     return 0;
 707 }
 708  
 709 //////////////////////////////////////
 710 int test_algorithm_permutation()
 711 {
 712 {
 713     std::array<int, 5> foo = { 1, 2, 3, 4, 5 };
 714     std::array<int, 5> bar = { 3, 1, 4, 5, 2 };
 715  
 716     if (std::is_permutation(foo.begin(), foo.end(), bar.begin()))
 717         std::cout << "foo and bar contain the same elements.\n"; // foo and bar contain the same elements
 718 }
 719  
 720 {
 721     int myints[] = { 1, 2, 3 };
 722  
 723     std::sort(myints, myints + 3);
 724  
 725     std::cout << "The 3! possible permutations with 3 elements:\n";
 726     do {
 727         std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
 728     } while (std::next_permutation(myints, myints + 3));
 729  
 730     std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n'; // 1 2 3
 731 }
 732  
 733 {
 734     int myints[] = { 1, 2, 3 };
 735  
 736     std::sort(myints, myints + 3);
 737     std::reverse(myints, myints + 3);
 738  
 739     std::cout << "The 3! possible permutations with 3 elements:\n";
 740     do {
 741         std::cout << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n';
 742     } while (std::prev_permutation(myints, myints + 3));
 743  
 744     std::cout << "After loop: " << myints[0] << ' ' << myints[1] << ' ' << myints[2] << '\n'; // 3 2 1
 745 }
 746  
 747     return 0;
 748 }
 749  
 750 /////////////////////////////////////////////
 751 struct myclass2 {
 752     bool operator() (int i, int j) { return (i<j); }
 753 } myobject2;
 754  
 755 bool compare_as_ints(double i, double j) { return (int(i)<int(j)); }
 756  
 757 int test_algorithm_sort()
 758 {
 759 {
 760     std::array<int, 4> foo{ 2, 4, 1, 3 };
 761  
 762     do {
 763         // try a new permutation:
 764         std::prev_permutation(foo.begin(), foo.end());
 765  
 766         // print range:
 767         std::cout << "foo:";
 768         for (int& x : foo) std::cout << ' ' << x;
 769         std::cout << '\n';
 770  
 771     } while (!std::is_sorted(foo.begin(), foo.end()));
 772  
 773     std::cout << "the range is sorted!\n";
 774 }
 775  
 776 {
 777     std::array<int, 4> foo{ 2, 4, 1, 3 };
 778     std::array<int, 4>::iterator it;
 779  
 780     do {
 781         // try a new permutation:
 782         std::prev_permutation(foo.begin(), foo.end());
 783  
 784         // print range:
 785         std::cout << "foo:";
 786         for (int& x : foo) std::cout << ' ' << x;
 787         it = std::is_sorted_until(foo.begin(), foo.end());
 788         std::cout << " (" << (it - foo.begin()) << " elements sorted)\n";
 789  
 790     } while (it != foo.end());
 791  
 792     std::cout << "the range is sorted!\n";
 793 }
 794  
 795 {
 796     std::vector<int> myvector;
 797  
 798     // set some values:
 799     for (int i = 1; i<10; i++) myvector.push_back(i);   // 1 2 3 4 5 6 7 8 9
 800  
 801     std::random_shuffle(myvector.begin(), myvector.end());
 802  
 803     // using default comparison (operator <):
 804     std::nth_element(myvector.begin(), myvector.begin() + 5, myvector.end());
 805  
 806     // using function as comp
 807     std::nth_element(myvector.begin(), myvector.begin() + 5, myvector.end(), myfunction2);
 808  
 809     // print out content:
 810     std::cout << "myvector contains:";
 811     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 812         std::cout << ' ' << *it; // 1 2 3 4 5 6 7 8 9
 813     std::cout << '\n';
 814 }
 815  
 816 {
 817     int myints[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
 818     std::vector<int> myvector(myints, myints + 9);
 819  
 820     // using default comparison (operator <):
 821     std::partial_sort(myvector.begin(), myvector.begin() + 5, myvector.end());
 822  
 823     // using function as comp
 824     std::partial_sort(myvector.begin(), myvector.begin() + 5, myvector.end(), myfunction2);
 825  
 826     // print out content:
 827     std::cout << "myvector contains:";
 828     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 829         std::cout << ' ' << *it; // 1 2 3 4 5 9 8 7 6
 830     std::cout << '\n';
 831 }
 832  
 833 {
 834     int myints[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };
 835     std::vector<int> myvector(5);
 836  
 837     // using default comparison (operator <):
 838     std::partial_sort_copy(myints, myints + 9, myvector.begin(), myvector.end());
 839  
 840     // using function as comp
 841     std::partial_sort_copy(myints, myints + 9, myvector.begin(), myvector.end(), myfunction2);
 842  
 843     // print out content:
 844     std::cout << "myvector contains:";
 845     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 846         std::cout << ' ' << *it; // 1 2 3 4 5
 847     std::cout << '\n';
 848 }
 849  
 850 {
 851     int myints[] = { 32, 71, 12, 45, 26, 80, 53, 33 };
 852     std::vector<int> myvector(myints, myints + 8);               // 32 71 12 45 26 80 53 33
 853  
 854     // using default comparison (operator <):
 855     std::sort(myvector.begin(), myvector.begin() + 4);           //(12 32 45 71)26 80 53 33
 856  
 857     // using function as comp
 858     std::sort(myvector.begin() + 4, myvector.end(), myfunction2); // 12 32 45 71(26 33 53 80)
 859  
 860     // using object as comp
 861     std::sort(myvector.begin(), myvector.end(), myobject2);     //(12 26 32 33 45 53 71 80)
 862  
 863     // print out content:
 864     std::cout << "myvector contains:";
 865     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 866         std::cout << ' ' << *it; // 12 26 32 33 45 53 71 80
 867     std::cout << '\n';
 868 }
 869  
 870 {
 871     double mydoubles[] = { 3.14, 1.41, 2.72, 4.67, 1.73, 1.32, 1.62, 2.58 };
 872  
 873     std::vector<double> myvector;
 874  
 875     myvector.assign(mydoubles, mydoubles + 8);
 876  
 877     std::cout << "using default comparison:";
 878     std::stable_sort(myvector.begin(), myvector.end());
 879     for (std::vector<double>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 880         std::cout << ' ' << *it; // 1.32 1.41 1.62 1.73 2.58 2.72 3.14 4.67
 881     std::cout << '\n';
 882  
 883     myvector.assign(mydoubles, mydoubles + 8);
 884  
 885     std::cout << "using 'compare_as_ints' :";
 886     std::stable_sort(myvector.begin(), myvector.end(), compare_as_ints);
 887     for (std::vector<double>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 888         std::cout << ' ' << *it; // 1.41 1.73 1.32 1.62 2.72 2.58 3.14 4.67
 889     std::cout << '\n';
 890 }
 891  
 892     return 0;
 893 }
 894  
 895 ////////////////////////////////////////////////////
 896 int test_algorithm_swap()
 897 {
 898 {
 899     int myints[] = { 10, 20, 30, 40, 50 };              //   myints:  10  20  30  40  50
 900     std::vector<int> myvector(4, 99);                   // myvector:  99  99  99  99
 901  
 902     std::iter_swap(myints, myvector.begin());     //   myints: [99] 20  30  40  50
 903                               // myvector: [10] 99  99  99
 904  
 905     std::iter_swap(myints + 3, myvector.begin() + 2); //   myints:  99  20  30 [99] 50
 906                                   // myvector:  10  99 [40] 99
 907  
 908     std::cout << "myvector contains:";
 909     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
 910         std::cout << ' ' << *it; // 10 99 40 99
 911     std::cout << '\n';
 912 }
 913  
 914 {
 915     int x = 10, y = 20;                              // x:10 y:20
 916     std::swap(x, y);                                 // x:20 y:10
 917  
 918     std::vector<int> foo(4, x), bar(6, y);       // foo:4x20 bar:6x10
 919     std::swap(foo, bar);                         // foo:6x10 bar:4x20
 920  
 921     std::cout << "foo contains:";
 922     for (std::vector<int>::iterator it = foo.begin(); it != foo.end(); ++it)
 923         std::cout << ' ' << *it; // 10 10 10 10 10 10
 924     std::cout << '\n';
 925 }
 926  
 927 {
 928     std::vector<int> foo(5, 10);        // foo: 10 10 10 10 10
 929     std::vector<int> bar(5, 33);        // bar: 33 33 33 33 33
 930  
 931     std::swap_ranges(foo.begin() + 1, foo.end() - 1, bar.begin());
 932  
 933     // print out results of swap:
 934     std::cout << "foo contains:";
 935     for (std::vector<int>::iterator it = foo.begin(); it != foo.end(); ++it)
 936         std::cout << ' ' << *it; // 10 33 33 33 10
 937     std::cout << '\n';
 938  
 939     std::cout << "bar contains:";
 940     for (std::vector<int>::iterator it = bar.begin(); it != bar.end(); ++it)
 941         std::cout << ' ' << *it; // 10 10 10 33 33
 942     std::cout << '\n';
 943 }
 944  
 945     return 0;
 946 }
 947  
 948 ///////////////////////////////////////////////
 949 static bool mycomp(char c1, char c2) { return std::tolower(c1)<std::tolower(c2); }
 950  
 951 int test_algorithm_lexicographical_compare()
 952 {
 953     char foo[] = "Apple";
 954     char bar[] = "apartment";
 955  
 956     std::cout << std::boolalpha;
 957  
 958     std::cout << "Comparing foo and bar lexicographically (foo<bar):\n";
 959  
 960     std::cout << "Using default comparison (operator<): ";
 961     std::cout << std::lexicographical_compare(foo, foo + 5, bar, bar + 9); // true
 962     std::cout << '\n';
 963  
 964     std::cout << "Using mycomp as comparison object: ";
 965     std::cout << std::lexicographical_compare(foo, foo + 5, bar, bar + 9, mycomp); // false
 966     std::cout << '\n';
 967  
 968     return 0;
 969 }
 970  
 971 //////////////////////////////////////
 972 static bool myfn(int i, int j) { return i<j; }
 973  
 974 int test_algorithm_min_max()
 975 {
 976 {
 977     std::cout << "min(1, 2)==" << std::min(1, 2) << '\n'; // 1
 978     std::cout << "min(2, 1)==" << std::min(2, 1) << '\n'; // 1
 979     std::cout << "min('a', 'z')==" << std::min('a', 'z') << '\n'; // a
 980     std::cout << "min(3.14, 2.72)==" << std::min(3.14, 2.72) << '\n'; // 2.72
 981 }
 982  
 983 {
 984     int myints[] = { 3, 7, 2, 5, 6, 4, 9 };
 985  
 986     // using default comparison:
 987     std::cout << "The smallest element is " << *std::min_element(myints, myints + 7) << '\n'; // 2
 988     std::cout << "The largest element is " << *std::max_element(myints, myints + 7) << '\n'; // 9
 989  
 990     // using function myfn as comp:
 991     std::cout << "The smallest element is " << *std::min_element(myints, myints + 7, myfn) << '\n'; // 2
 992     std::cout << "The largest element is " << *std::max_element(myints, myints + 7, myfn) << '\n'; // 9
 993  
 994     // using object myobj as comp:
 995     std::cout << "The smallest element is " << *std::min_element(myints, myints + 7, myobject2) << '\n'; // 2
 996     std::cout << "The largest element is " << *std::max_element(myints, myints + 7, myobject2) << '\n'; // 9
 997 }
 998  
 999 {
1000     std::cout << "max(1,2)==" << std::max(1, 2) << '\n'; // 2
1001     std::cout << "max(2,1)==" << std::max(2, 1) << '\n'; // 2
1002     std::cout << "max('a','z')==" << std::max('a', 'z') << '\n'; // z
1003     std::cout << "max(3.14,2.73)==" << std::max(3.14, 2.73) << '\n'; // 3.14
1004 }
1005  
1006 {
1007     auto result = std::minmax({ 1, 2, 3, 4, 5 });
1008  
1009     std::cout << "minmax({1,2,3,4,5}): ";
1010     std::cout << result.first << ' ' << result.second << '\n'; // 1 5
1011 }
1012  
1013 {
1014     std::array<int, 7> foo{ 3, 7, 2, 9, 5, 8, 6 };
1015  
1016     auto result = std::minmax_element(foo.begin(), foo.end());
1017  
1018     // print result:
1019     std::cout << "min is " << *result.first; // 2
1020     std::cout << ", at position " << (result.first - foo.begin()) << '\n'; // 2
1021     std::cout << "max is " << *result.second; // 9
1022     std::cout << ", at position " << (result.second - foo.begin()) << '\n'; // 3
1023 }
1024  
1025     return 0;
1026 }
1027  
1028 ///////////////////////////////////////////
1029 int test_algorithm_mismatch()
1030 {
1031     std::vector<int> myvector;
1032     for (int i = 1; i<6; i++) myvector.push_back(i * 10); // myvector: 10 20 30 40 50
1033  
1034     int myints[] = { 10, 20, 80, 320, 1024 };                //   myints: 10 20 80 320 1024
1035  
1036     std::pair<std::vector<int>::iterator, int*> mypair;
1037  
1038     // using default comparison:
1039     mypair = std::mismatch(myvector.begin(), myvector.end(), myints);
1040     std::cout << "First mismatching elements: " << *mypair.first; // 30
1041     std::cout << " and " << *mypair.second << '\n'; // 80
1042  
1043     ++mypair.first; ++mypair.second;
1044  
1045     // using predicate comparison:
1046     mypair = std::mismatch(mypair.first, myvector.end(), mypair.second, mypredicate);
1047     std::cout << "Second mismatching elements: " << *mypair.first; // 40
1048     std::cout << " and " << *mypair.second << '\n'; // 320
1049  
1050     return 0;
1051 }
1052  
1053 //////////////////////////////////////////
1054 /* The behavior of std::move_backward template is equivalent to:
1055 template<class BidirectionalIterator1, class BidirectionalIterator2>
1056 BidirectionalIterator2 move_backward ( BidirectionalIterator1 first,
1057     BidirectionalIterator1 last,
1058     BidirectionalIterator2 result )
1059 {
1060     while (last!=first) *(--result) = std::move(*(--last));
1061     return result;
1062 }
1063 */
1064 int test_algorithm_move()
1065 {
1066 {
1067     std::vector<std::string> foo = { "air", "water", "fire", "earth" };
1068     std::vector<std::string> bar(4);
1069  
1070     // moving ranges:
1071     std::cout << "Moving ranges...\n";
1072     std::move(foo.begin(), foo.begin() + 4, bar.begin());
1073  
1074     std::cout << "foo contains " << foo.size() << " elements:";// 4
1075     std::cout << " (each in an unspecified but valid state)";
1076     std::cout << '\n';
1077  
1078     std::cout << "bar contains " << bar.size() << " elements:"; // 4
1079     for (std::string& x : bar) std::cout << " [" << x << "]"; // [air] [water] [fire] [earch]
1080     std::cout << '\n';
1081  
1082     // moving container:
1083     std::cout << "Moving container...\n";
1084     foo = std::move(bar);
1085  
1086     std::cout << "foo contains " << foo.size() << " elements:"; // 4
1087     for (std::string& x : foo) std::cout << " [" << x << "]"; // [air] [water] [fire] [earch]
1088     std::cout << '\n';
1089     std::cout << "bar contains " << bar.size() << " elements" << std::endl; // 0
1090     //std::cout << "bar is in an unspecified but valid state";
1091     //std::cout << '\n';
1092 }
1093  
1094 {
1095     std::string elems[10] = { "air", "water", "fire", "earth" };
1096  
1097     // insert new element at the beginning:
1098     std::move_backward(elems, elems + 4, elems + 5);
1099     elems[0] = "ether";
1100  
1101     std::cout << "elems contains:";
1102     for (int i = 0; i<10; ++i)
1103         std::cout << " [" << elems[i] << "]"; // [ether] [air] [water] [fire] [earch]
1104     std::cout << '\n';
1105 }
1106  
1107     return 0;
1108 }
1109  
1110 //////////////////////////////////////////////
1111 // random generator function:
1112 int myrandom(int i) { return std::rand() % i; }
1113  
1114 int test_algorithm_shuffle()
1115 {
1116 {
1117     std::srand(unsigned(std::time(0)));
1118     std::vector<int> myvector;
1119  
1120     // set some values:
1121     for (int i = 1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9
1122  
1123     // using built-in random generator:
1124     std::random_shuffle(myvector.begin(), myvector.end());
1125  
1126     // using myrandom:
1127     std::random_shuffle(myvector.begin(), myvector.end(), myrandom);
1128  
1129     // print out content:
1130     std::cout << "myvector contains:";
1131     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1132         std::cout << ' ' << *it;
1133  
1134     std::cout << '\n';
1135 }
1136  
1137 {
1138     std::array<int, 5> foo{ 1, 2, 3, 4, 5 };
1139  
1140     // obtain a time-based seed:
1141     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
1142  
1143     shuffle(foo.begin(), foo.end(), std::default_random_engine(seed));
1144  
1145     std::cout << "shuffled elements:";
1146     for (int& x : foo) std::cout << ' ' << x;
1147     std::cout << '\n';
1148 }
1149  
1150     return 0;
1151 }
1152  
1153 //////////////////////////////////////////
1154 int test_algorithm_remove()
1155 {
1156 {
1157     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };      // 10 20 30 30 20 10 10 20
1158  
1159     // bounds of range:
1160     int* pbegin = myints;                                   // ^
1161     int* pend = myints + sizeof(myints) / sizeof(int);      // ^                       ^
1162  
1163     pend = std::remove(pbegin, pend, 20);                   // 10 30 30 10 10 ?  ?  ?
1164                                                             // ^              ^
1165     std::cout << "range contains:";
1166     for (int* p = pbegin; p != pend; ++p)
1167         std::cout << ' ' << *p; // 10 30 30 10 10
1168     std::cout << '\n';
1169 }
1170  
1171 {
1172     int myints[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };            // 1 2 3 4 5 6 7 8 9
1173  
1174     // bounds of range:
1175     int* pbegin = myints;                                    // ^
1176     int* pend = myints + sizeof(myints) / sizeof(int);       // ^                 ^
1177  
1178     pend = std::remove_if(pbegin, pend, IsOdd);              // 2 4 6 8 ? ? ? ? ?
1179                                                              // ^       ^
1180     std::cout << "the range contains:";
1181     for (int* p = pbegin; p != pend; ++p)
1182         std::cout << ' ' << *p; // 2 4 6 8
1183     std::cout << '\n';
1184 }
1185  
1186 {
1187     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };               // 10 20 30 30 20 10 10 20
1188     std::vector<int> myvector(8);
1189  
1190     std::remove_copy(myints, myints + 8, myvector.begin(), 20);      // 10 30 30 10 10 0 0 0
1191  
1192     std::cout << "myvector contains:";
1193     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1194         std::cout << ' ' << *it; // 10 30 30 10 10 0 0 0
1195     std::cout << '\n';
1196 }
1197  
1198 {
1199     int myints[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
1200     std::vector<int> myvector(9);
1201  
1202     std::remove_copy_if(myints, myints + 9, myvector.begin(), IsOdd);
1203  
1204     std::cout << "myvector contains:";
1205     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1206         std::cout << ' ' << *it; // 2 4 6 8 0 0 0 0 0
1207     std::cout << '\n';
1208 }
1209  
1210     return 0;
1211 }
1212  
1213 //////////////////////////////////////////////
1214 int test_algorithm_replace()
1215 {
1216 {
1217     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };
1218     std::vector<int> myvector(myints, myints + 8);            // 10 20 30 30 20 10 10 20
1219  
1220     std::replace(myvector.begin(), myvector.end(), 20, 99);   // 10 99 30 30 99 10 10 99
1221  
1222     std::cout << "myvector contains:";
1223     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1224         std::cout << ' ' << *it; // 10 99 30 30 99 10 10 99
1225     std::cout << '\n';
1226 }
1227  
1228 {
1229     std::vector<int> myvector;
1230  
1231     // set some values:
1232     for (int i = 1; i<10; i++) myvector.push_back(i);               // 1 2 3 4 5 6 7 8 9
1233  
1234     std::replace_if(myvector.begin(), myvector.end(), IsOdd, 0);    // 0 2 0 4 0 6 0 8 0
1235  
1236     std::cout << "myvector contains:";
1237     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1238         std::cout << ' ' << *it; // 0 2 0 4 0 6 0 8 0
1239     std::cout << '\n';
1240 }
1241  
1242 {
1243     int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };
1244  
1245     std::vector<int> myvector(8);
1246     std::replace_copy(myints, myints + 8, myvector.begin(), 20, 99);
1247  
1248     std::cout << "myvector contains:";
1249     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1250         std::cout << ' ' << *it; // 10 99 30 30 99 10 10 99
1251     std::cout << '\n';
1252 }
1253  
1254 {
1255     std::vector<int> foo, bar;
1256  
1257     // set some values:
1258     for (int i = 1; i<10; i++) foo.push_back(i);                         // 1 2 3 4 5 6 7 8 9
1259  
1260     bar.resize(foo.size());   // allocate space
1261     std::replace_copy_if(foo.begin(), foo.end(), bar.begin(), IsOdd, 0); // 0 2 0 4 0 6 0 8 0
1262  
1263     std::cout << "bar contains:";
1264     for (std::vector<int>::iterator it = bar.begin(); it != bar.end(); ++it)
1265         std::cout << ' ' << *it; // 0 2 0 4 0 6 0 8 0
1266     std::cout << '\n';
1267 }
1268  
1269     return 0;
1270 }
1271  
1272 ///////////////////////////////////////////////////
1273 int test_algorithm_reverse()
1274 {
1275 {
1276     std::vector<int> myvector;
1277  
1278     // set some values:
1279     for (int i = 1; i<10; ++i) myvector.push_back(i);   // 1 2 3 4 5 6 7 8 9
1280  
1281     std::reverse(myvector.begin(), myvector.end());     // 9 8 7 6 5 4 3 2 1
1282  
1283     // print out content:
1284     std::cout << "myvector contains:";
1285     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1286         std::cout << ' ' << *it; // 9 8 7 6 5 4 3 2 1
1287     std::cout << '\n';
1288 }
1289  
1290 {
1291     int myints[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
1292     std::vector<int> myvector;
1293  
1294     myvector.resize(9);    // allocate space
1295  
1296     std::reverse_copy(myints, myints + 9, myvector.begin());
1297  
1298     // print out content:
1299     std::cout << "myvector contains:";
1300     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1301         std::cout << ' ' << *it; // 9 8 7 6 5 4 3 2 1
1302  
1303     std::cout << '\n';
1304 }
1305  
1306     return 0;
1307 }
1308  
1309 ////////////////////////////////////////////
1310 /*
1311 The behavior of std::rotate template (C++98) is equivalent to:
1312 template <class ForwardIterator>
1313 void rotate (ForwardIterator first, ForwardIterator middle, ForwardIterator last)
1314 {
1315     ForwardIterator next = middle;
1316     while (first!=next) {
1317         swap (*first++,*next++);
1318         if (next==last) next=middle;
1319         else if (first==middle) middle=next;
1320     }
1321 }
1322 */
1323  
1324 int test_algorithm_rotate()
1325 {
1326 {
1327     std::vector<int> myvector;
1328  
1329     // set some values:
1330     for (int i = 1; i<10; ++i) myvector.push_back(i);                    // 1 2 3 4 5 6 7 8 9
1331  
1332     std::rotate(myvector.begin(), myvector.begin() + 3, myvector.end()); // 4 5 6 7 8 9 1 2 3
1333     // print out content:
1334     std::cout << "myvector contains:";
1335     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1336         std::cout << ' ' << *it; // 4 5 6 7 8 9 1 2 3
1337     std::cout << '\n';
1338 }
1339  
1340 {
1341     int myints[] = { 10, 20, 30, 40, 50, 60, 70 };
1342  
1343     std::vector<int> myvector(7);
1344  
1345     std::rotate_copy(myints, myints + 3, myints + 7, myvector.begin());
1346  
1347     // print out content:
1348     std::cout << "myvector contains:";
1349     for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
1350         std::cout << ' ' << *it; // 40 50 60 70 10 20 30
1351     std::cout << '\n';
1352 }
1353  
1354     return 0;
1355 }
1356  
1357 //////////////////////////////////////
1358 /*
1359 The behavior of std::set_difference template is equivalent to:
1360 template <class InputIterator1, class InputIterator2, class OutputIterator>
1361 OutputIterator set_difference (InputIterator1 first1, InputIterator1 last1,
1362     InputIterator2 first2, InputIterator2 last2, OutputIterator result)
1363 {
1364     while (first1!=last1 && first2!=last2) {
1365         if (*first1<*first2) { *result = *first1; ++result; ++first1; }
1366         else if (*first2<*first1) ++first2;
1367         else { ++first1; ++first2; }
1368     }
1369     return std::copy(first1,last1,result);
1370 }
1371 */
1372  
1373 int test_algorithm_set()
1374 {
1375 {
1376     int first[] = { 5, 10, 15, 20, 25 };
1377     int second[] = { 50, 40, 30, 20, 10 };
1378     std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
1379     std::vector<int>::iterator it;
1380  
1381     std::sort(first, first + 5);     //  5 10 15 20 25
1382     std::sort(second, second + 5);   // 10 20 30 40 50
1383  
1384     it = std::set_difference(first, first + 5, second, second + 5, v.begin());
1385                                //  5 15 25  0  0  0  0  0  0  0
1386     v.resize(it - v.begin());                      //  5 15 25
1387  
1388     std::cout << "The difference has " << (v.size()) << " elements:\n"; // 3
1389     for (it = v.begin(); it != v.end(); ++it)
1390         std::cout << ' ' << *it; // 5 15 25
1391     std::cout << '\n';
1392 }
1393  
1394 {
1395     int first[] = { 5, 10, 15, 20, 25 };
1396     int second[] = { 50, 40, 30, 20, 10 };
1397     std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
1398     std::vector<int>::iterator it;
1399  
1400     std::sort(first, first + 5);     //  5 10 15 20 25
1401     std::sort(second, second + 5);   // 10 20 30 40 50
1402  
1403     it = std::set_intersection(first, first + 5, second, second + 5, v.begin());
1404                                                    // 10 20 0  0  0  0  0  0  0  0
1405     v.resize(it - v.begin());                      // 10 20
1406  
1407     std::cout << "The intersection has " << (v.size()) << " elements:\n"; // 2
1408     for (it = v.begin(); it != v.end(); ++it)
1409         std::cout << ' ' << *it; // 10 20
1410     std::cout << '\n';
1411 }
1412  
1413 {
1414     int first[] = { 5, 10, 15, 20, 25 };
1415     int second[] = { 50, 40, 30, 20, 10 };
1416     std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
1417     std::vector<int>::iterator it;
1418  
1419     std::sort(first, first + 5);     //  5 10 15 20 25
1420     std::sort(second, second + 5);   // 10 20 30 40 50
1421  
1422     it = std::set_symmetric_difference(first, first + 5, second, second + 5, v.begin());
1423                                //  5 15 25 30 40 50  0  0  0  0
1424     v.resize(it - v.begin());                      //  5 15 25 30 40 50
1425  
1426     std::cout << "The symmetric difference has " << (v.size()) << " elements:\n"; // 6
1427     for (it = v.begin(); it != v.end(); ++it)
1428         std::cout << ' ' << *it; // 5 15 25 30 40 50
1429     std::cout << '\n';
1430 }
1431  
1432 {
1433     int first[] = { 5, 10, 15, 20, 25 };
1434     int second[] = { 50, 40, 30, 20, 10 };
1435     std::vector<int> v(10);                      // 0  0  0  0  0  0  0  0  0  0
1436     std::vector<int>::iterator it;
1437  
1438     std::sort(first, first + 5);     //  5 10 15 20 25
1439     std::sort(second, second + 5);   // 10 20 30 40 50
1440  
1441     it = std::set_union(first, first + 5, second, second + 5, v.begin());
1442                                // 5 10 15 20 25 30 40 50  0  0
1443     v.resize(it - v.begin());                      // 5 10 15 20 25 30 40 50
1444  
1445     std::cout << "The union has " << (v.size()) << " elements:\n"; // 8
1446     for (it = v.begin(); it != v.end(); ++it)
1447         std::cout << ' ' << *it; // 5 10 15 20 25 30 40 50
1448     std::cout << '\n';
1449 }
1450  
1451     return 0;
1452 }
1453  
1454 /////////////////////////////////////
1455 int op_increase(int i) { return ++i; }
1456  
1457 int test_algorithm_transform()
1458 {
1459     std::vector<int> foo;
1460     std::vector<int> bar;
1461  
1462     // set some values:
1463     for (int i = 1; i<6; i++)
1464         foo.push_back(i * 10);                         // foo: 10 20 30 40 50
1465  
1466     bar.resize(foo.size());                         // allocate space
1467  
1468     std::transform(foo.begin(), foo.end(), bar.begin(), op_increase);
1469                                                            // bar: 11 21 31 41 51
1470  
1471     // std::plus adds together its two arguments:
1472     std::transform(foo.begin(), foo.end(), bar.begin(), foo.begin(), std::plus<int>());
1473                                                            // foo: 21 41 61 81 101
1474  
1475     std::cout << "foo contains:";
1476     for (std::vector<int>::iterator it = foo.begin(); it != foo.end(); ++it)
1477         std::cout << ' ' << *it; // 21 41 61 81 101
1478     std::cout << '\n';
1479  
1480     return 0;
1481 }
1482  
1483 /////////////////////////////////////////
1484 int test_algorithm_unique()
1485 {
1486 {
1487     int myints[] = { 10, 20, 20, 20, 30, 30, 20, 20, 10 };           // 10 20 20 20 30 30 20 20 10
1488     std::vector<int> myvector(myints, myints + 9);
1489  
1490     // using default comparison:
1491     std::vector<int>::iterator it;
1492     it = std::unique(myvector.begin(), myvector.end());              // 10 20 30 20 10 ?  ?  ?  ?
1493                                                                      //                ^
1494  
1495     myvector.resize(std::distance(myvector.begin(), it));            // 10 20 30 20 10
1496  
1497     // using predicate comparison:
1498     std::unique(myvector.begin(), myvector.end(), myfunction);   // (no changes)
1499  
1500     // print out content:
1501     std::cout << "myvector contains:";
1502     for (it = myvector.begin(); it != myvector.end(); ++it)
1503         std::cout << ' ' << *it; // 10 20 30 20 10
1504     std::cout << '\n';
1505 }
1506  
1507 {
1508     int myints[] = { 10, 20, 20, 20, 30, 30, 20, 20, 10 };
1509     std::vector<int> myvector(9);                                   // 0  0  0  0  0  0  0  0  0
1510  
1511     // using default comparison:
1512     std::vector<int>::iterator it;
1513     it = std::unique_copy(myints, myints + 9, myvector.begin());   // 10 20 30 20 10 0  0  0  0
1514                                                                    //                ^
1515  
1516     std::sort(myvector.begin(), it);                               // 10 10 20 20 30 0  0  0  0
1517                                                                    //                ^
1518  
1519     // using predicate comparison:
1520     it = std::unique_copy(myvector.begin(), it, myvector.begin(), myfunction);
1521                                                                    // 10 20 30 20 30 0  0  0  0
1522                                                                    //          ^
1523  
1524     myvector.resize(std::distance(myvector.begin(), it));    // 10 20 30
1525  
1526     // print out content:
1527     std::cout << "myvector contains:";
1528     for (it = myvector.begin(); it != myvector.end(); ++it)
1529         std::cout << ' ' << *it; // 10 20 30
1530     std::cout << '\n';
1531 }
1532  
1533     return 0;
1534 }
1535  
1536 } // namespace algorithm_
