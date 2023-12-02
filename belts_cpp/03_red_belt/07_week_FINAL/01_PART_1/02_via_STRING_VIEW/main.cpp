#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "parse.h"
#include "search_server.h"
#include "test_runner.h"

void TestSpeed(const std::vector<std::string>& docs,
               const std::vector<std::string>& queries) {
    std::istringstream docs_input(Join('\n', docs));
    std::istringstream queries_input(Join('\n', queries));

    SearchServer srv;
    srv.UpdateDocumentBase(docs_input);

    std::ostringstream queries_output;
    srv.AddQueriesStream(queries_input, queries_output);
}

void Test_Duration() {
    const std::vector<std::string> docs = {
        "the c++ language has gone through a major transformation in the past decade with the development and release of c++11 and then later with its newer versions c++14 and c++17 these new standards have introduced new concepts simplified or extended existing syntax and semantics and overall transformed the way we write code. c++11 looks like a new language and code written using the new standards is called modern c++ code",
        "tirane is the capital of albania automatic type deduction is one of the most important and widely used features in modern c++ the new c++ standards have made it possible to use auto as a placeholder for types in various contexts and let the compiler deduce the actual type in c++11, auto can be used for declaring local variables and for the return type of a function with a trailing return type in c++14 auto can be used for the return type of a function without specifying a trailing type and for parameter declarations in lambda expressions future standard versions are likely to expand the use of auto to even more cases",
        "using auto promotes good object-oriented practices such as preferring interfaces over implementations the lesser the number of types specified the more generic the code is and more open to future changes which is a fundamental principle of object-oriented programming",
        "it means less typing and less concern for actual types that we don't care about anyways  it is very often that even though we explicitly specify the type  we don't actually care about it  a very common case is with iterators  but one can think of many more  when you want to iterate over a range  you don't care about the actual type of the iterator  you are only interested in the iterator itself; so  using auto saves time used for typing possibly long names and helps you focus on actual code and not type names  in the following example  in the first for loop  we explicitly use the type of the iterator  it is a lot of text to type  the long statements can actually make the code less readable  and you also need to know the type name that you actually don't care about  the second loop with the auto specifier looks simpler and saves you from typing and caring about actual types ",
        "the auto specifier is only a placeholder for the type  not for the const/volatile and references specifiers  if you need a const/volatile and/or reference type  then you need to specify them explicitly  in the following example  foo get() returns a reference to int; when variable x is initialized from the return value  the type deduced by the compiler is int  and not int&  therefore  any change to x will not propagate to foo x_  in order to do so  one should use auto&",
        "if you use the auto specifier but still need to know the type you can do so in any ide by putting the cursor over a variable for instance if you leave the ide however that is not possible anymore and the only way to know the actual type is to deduce it yourself from the initialization expression which could probably mean searching through the code for function return types",
        "the auto can be used to specify the return type from a function in c++11 this requires a trailing return type in the function declaration in c++14 this has been relaxed and the type of the return value is deduced by the compiler from the return expression if there are multiple return values they should have the same type ",
        "as mentioned earlier auto does not retain const/volatile and reference qualifiers this leads to problems with auto as a placeholder for the return type from a function to explain this let us consider the preceding example with fooget() this time we have a wrapper function called proxy_get() that takes a reference to a foo calls get() and returns the value returned by get() which is an int& however the compiler will deduce the return type of proxy_get() as being int not int& trying to assign that value to an int& fails with an error",
        "the last important case where auto can be used is with lambdas as of c++14 both lambda return type and lambda parameter types can be auto such a lambda is called a generic lambda because the closure type defined by the lambda has a templated call operator the following shows a generic lambda that takes two auto parameters and returns the result of applying operator+ on the actual types",
        "In C++ it is possible to create synonyms that can be used instead of a type name This is achieved by creating a typedef declaration This is useful in several cases such as creating shorter or more meaningful names for a type or names for function pointers However typedef declarations cannot be used with templates to create template type aliases An std::vector<T> for instance is not a type (std::vector<int> is a type) but a sort of family of all types that can be created when the type placeholder T is replaced with an actual type",
        "a typedef declaration introduces a synonym (or an alias in other words) for a type it does not introduce another type (like a class struct union or enum declaration) type names introduced with a typedef declaration follow the same hiding rules as identifier names they can also be redeclared but only to refer to the same type (therefore you can have valid multiple typedef declarations that introduce the same type name synonym in a translation unit as long as it is a synonym for the same type) the following are typical examples of typedef declarations",
        "an alias-declaration is however more readable and more clear about the actual type that is aliased when it comes to creating aliases for array types and function pointer types in the examples from the how to do it section it is easily understandable that array_t is a name for the type array of 10 integers and fn is a name for a function type that takes two parameters of type byte and double and returns void that is also consistent with the syntax for declaring std::function objects",
        "apart from the different methods of initializing the data there are also some limitations for instance the only way to initialize a standard container was to first declare an object and then insert elements into it; vector was an exception because it is possible to assign values from an array that can be prior initialized using aggregate initialization on the other hand however dynamically allocated aggregates could not be initialized directly all the examples in the how to do it section use direct initialization but copy initialization is also possible with brace-initialization the two forms direct and copy initialization may be equivalent in most cases but copy initialization is less permissive because it does not consider explicit constructors in its implicit conversion sequence that must produce an object directly from the initializer whereas direct initialization expects an implicit conversion from the initializer to an argument of the constructor dynamically allocated arrays can only be initialized using direct initialization of the classes shown in the preceding examples foo is the one class that has both a default constructor and a constructor with parameters to use the default constructor to perform default initialization we need to use empty braces that is {} to use the constructor with parameters we need to provide the values for all the arguments in braces {} unlike non-aggregate types where default initialization means invoking the default constructor for aggregate types default initialization means initializing with zeros",
        "constructors are a place where non-static class member initialization is done many developers prefer assignments in the constructor body aside from the several exceptional cases when that is actually necessary initialization of non-static members should be done in the constructor's initializer list or as of c++11 using default member initialization when they are declared in the class prior to c++11 constants and non-constant non-static data members of a class had to be initialized in the constructor initialization on declaration in aclass was only possible for static constants as we will see further this limitation was removed in c++11 that allows initialization ofnon-statics in the class declaration this initialization is called default member initialization and is explained in the next sections",
        "many developers however do not use the initializer list but prefer assignments in the constructor's body or even mix assignments and the initializer list that could be for several reasons--for largerclasses with many members the constructor assignments may look easier to read than long initializer lists perhaps split on many lines or it could be because they are familiar with other programming languages that don't have an initializer list or because unfortunately for various reasons they don't even know about it",
        "using assignments in the constructor is not efficient as this can create temporary objects that are later discarded if not initialized in the initializer list non-static members are initialized via their default constructor and then when assigned a value in the constructor's body the assignment operator is invoked this can lead to inefficient work if the default constructor allocates a resource (such as memory or a file) and that has to be deallocated and reallocated in the assignment operator",
        "in the preceding example defaultheight and defaultwidth are both constants therefore the values do not depend on the way the object is constructed so they are initialized when declared the textflow object is a non-constant non-static data member whose value also does not depend on the way the object is initialized (it could be changed via another member function) therefore it is also initialized using default member initialization when it is declared text on the other hand is also a non-constant non-static data member but its initial value depends on the way the object is constructed and therefore it is initialized in the constructor's initializer list using a value passed as an argument to the constructor",
        "c++11 provides standardized methods for specifying and querying the  alignment requirements of a type (something that was previously possible only through compiler-specific methods) controlling the alignment is important in order to boost performance on different processors and enable the use of some instructions that only work with data on particular alignments for example intel sse and intel sse2 require 16 bytes alignment of data whereas for intel advanced vector extensions (or intel avx) it is highly recommended to use 32 bytes alignment this recipe explores the alignas specifier for controlling the alignment requirements and the alignof operator that retrieves the alignment requirements of a type",
        "unlike other similar programming languages c++ does not have a specific syntax for declaring interfaces (that are basically classes with pure virtual methods only) and also has some deficiencies related to how virtual methods are declared in c++ the virtual methods are introduced with the virtual keyword however the keyword virtual is optional for declaring overrides in derived classes that can lead to confusion when dealing with large classes or hierarchies you may need to navigate throughout the hierarchy up to the base to figure out whether a function is virtual or not on the other hand sometimes it is useful to make sure that a virtual function or even a derived class can no longer be overridden or derived further in this recipe we will see how to use c++11 special identifiers override and final to declare virtual functions or classes",
        "as we have seen in the preceding recipe the range-based for loops known as for each in other programming languages allows you to iterate over the elements of a range providing a simplified syntax over the standard for loops and making the code more readable in many situations however range-based for loops do not work out of the box with any type representing a range but require the presence of a begin() and end() function (for non-array types) either as a member or free function in this recipe we will see how to enable a custom type to be used in range-based for loops",
        "before c++11 a constructor with a single parameter was considered a converting constructor with c++11 every constructor without the explicit specifier is considered a converting constructor such a constructor defines an implicit conversion from the type or types of its arguments to the type of the class classes can also define converting operators that convert the type of the class to another specified type all these are useful in some cases but can create problems in other cases in this recipe we will see how to use explicit constructors and conversion operators",
        "perhaps a more realistic example to understand where problems could arise would be to consider a string buffer implementation this would be a class that contains an internal buffer of characters the class may provide several conversion constructors: a default constructor a constructor that takes a size_t parameter representing the size of the buffer to preallocate and a constructor that takes a pointer to char that should be used to allocate and initialize the internal buffer succinctly such a string buffer could look like this",
        "in this case b4 is initialized with a char since an implicit conversion to size_t exists the constructor with a single parameter will be called the intention here is not necessarily clear perhaps it should have been instead of 'a' in which case the third constructor would have been called however b5 is most likely an error because maxsize is an enumerator representing an itemsizes and should have nothing to do with a string buffer size these erroneous situations are not flagged by the compiler in any way using the explicit specifier in the declaration of a constructor that constructor becomes an explicit constructor and no longer allows implicit constructions of objects of a class type to exemplify this we will slightly change the string_buffer class earlier to declare all constructors explicit",
        "the larger a program the greater the chances are you could run into name collisions with file locals when your program is linked functions or variables that are declared in a source file and are supposed to be local to the translation unit may collide with other similar functions or variables declared in another translation unit that is because all symbols that are not declared static have external linkage and their names must be unique throughout the program the typical c solution for this problem is to declare those symbols static changing their linkage from external to internal and therefore making them local to a translation unit in this recipe we will look at the c++ solution for this problem",
        "first of all it declares a namespace with a unique name (what the name is and how it generates that name is a compiler implementation detail and should not be a concern) at this point the namespace is empty and the purpose of this line is to basically establish the namespace second a using directive brings everything from the _unique_name_namespace into the current namespace third the namespace with the compiler-generated name is defined as it was in the original source code (when it had no name) by defining the translation unit local print() functions in an unnamed namespace they have local visibility only yet their external linkage no longer produces linkage errors since they now have external unique names unnamed namespaces are also working in a perhaps more obscure situation involving templates template arguments cannot be names with internal linkage so using static variables is not possible on the other hand symbols in an unnamed namespace have external linkage and can be used as template arguments this problem is shown in the following example where declaring t1 produces a compiler error because the non-type argument expression has internal linkage however t2 is correct because size2 has external linkage",
        "all these engines and engine adaptors are producing pseudo-random numbers the library however provides another engine called random_device that is supposed to produce non-deterministic numbers but this is not an actual constraint as physical sources of random entropy might not be available therefore implementations of random_device could actually be based on a pseudo-random engine the random_device class cannot be seeded like the other engines and has an additional method called entropy() that returns the random device entropy which is 0 for a deterministic generator and nonzero for a non-deterministic generator however this is not a reliable method for determining whether the device is actually deterministic or non-deterministic for instance both gnu libstdc++ and llvm libc++ implement a non-deterministic device but return 0 for entropy on the other hand vc++ and boostrandom return 32 and 10 respectively for entropy",
        "each of the engines provided by the library has advantages and disadvantages the linear congruential engine has a small internal state but it is not very fast on the other hand the subtract with carry engine is very fast but requires more memory for its internal state the mersenne twister is the slowest of them and the one that has the largest internal state but when initialized appropriately can produce the longest non-repeating sequence of numbers in thefollowing examples we will use std::mt19937 a 32-bit mersenne twister with 19937 bits of internal state the arrows show the inheritance scheme of the classes this might look very overwhelming at first but we will get to use most of these classes in this chapter and get familiar with them class by class when looking at those classes in the c++ stl documentation we will not find them directly with these exact names that is because the names in the diagram are what we see as application programmers but they are really mostly just typedefs of classes with a basic_ class name prefix (for example we will have an easier job searching the stl documentation for basic_istream rather than istream) the basic_* i/o stream classes are templates that can be specialized for different character types the classes in the diagram are specialized on char values we will use these specializations throughout the book if weprefix those class names with the w character we get wistream wostream and so on--these are the specialization typedefs for wchar_t instead of char for example",
        "at the top of the diagram we see std::ios_base we will basically never use it directly but it is listed for completeness because all other classes inherit from it the next specialization is std::ios which embodies the idea of an object which maintains a stream of datathat can be in good state run empty of data state (eof) or some kindof fail state the first specializations we are going to actually useare std::istream and std::ostream the ",
        "and the o prefix stand for input and output we have seen them in our earliest days of c++ programming in the simplest examples in form of the objects std::cout and std::cin (but also std::cerr) these areinstances of those classes which are always globally available we do data output via ostream and input via istream a class which inherits from both istream and ostream is iostream it combines both input and output capabilities when we understand how all classes from the trioconsisting of istream ostream and iostream can be used we basicallyare ready to immediately put all following ones to use too: ifstream ofstream and fstream inherit from istream ostream and iostream respectively but lift their capabilities to redirect the i/o from and to files from the computer's filesystemthe istringstream ostringstream and iostringstream work pretty analogously they help build strings in memory and/or consuming data from them obviously strings can be added with the + operator like numbers but that has nothing to do with math but results in concatenated strings in order to mix this with string_view we need to convert to std::string first however it is really important to note that when mixing strings and string views in code we must never assume that the underlying string behind a string_view is zero terminated! this is why we would rather write + string{some_string_view} than s + some_string_viewdata() aside from that std::string provides a member function append which can handle string_view instances but italters the string instead of returning a new one with the string view content appended std::string_view is useful but be cautious when mixing it with strings and string functions we cannot assume that they are zero-terminatedwhich breaks things quickly in a standard string environmentfortunately there are often proper function overloads which can deal with them the right way if we want to do complex string concatenation with formatting and so on we should however not do that piece by piece on string instances the std::stringstream std::ostringstream and std::istringstream classes are much better suited for this as they enhance the memory management while appending and provide all the formatting features we know from streams in general the std::ostringstream class is what we chose in this section because we were going to create a string instead of parsing it an std::istringstream instance could have been instantiated from an existing string which we could have then comfortably parsed into variables of other types if we want to combine both std::stringstream is the perfect all-rounder in this section we used string::find_first_not_of andstring::find_last_not_of both functions accept a c-style string which acts as a list of characters that should be skipped while searching a different character if we have a string instance that carries the string  foo bar and we call find_first_not_of( bfo ) on it it will return us the value 5 because the 'a' character is the first one that is not in the bfo string the order of the characters in the argument string is not important the same functions exist with inverted logic although we did not use them in this recipe: string::find_first_of and string::find_last_of similar to iterator based functions we need to check if these functions return an actual position in the string or a value that denotes that they did not find a character position fulfilling the constraints if they did not find one they return string::npos from the character positions we retrieved from these functions in our helper function we built us a substring without surrounding whitespace using string::substring this function accepts a relative offset and a string length and then returns a new string instance with its own memory which contains only that substring for example string{abcdef}substr(2 2) will return us a new string cd",
        "at the top of the diagram we see std::ios_base we will basically never use it directly but it is listed for completeness because all other classes inherit from it the next specialization is std::ios which embodies the idea of an object which maintains a stream of data that can be in good state run empty of data state (eof) or some kind of fail state the first specializations we are going to actually use are std::istream and std::ostream the  prefix stand for input and output we have seen them in our earliest days of c++ programming in the simplest examples in form of the objects std::cout and std::cin (but also std::cerr) these are instances of those classes which are always globally",
        "we have just seen that we can call a function that accepts a string_view argument with basically anything that is string like in the sense that it stores characters in a contiguous way no copy of the underlying string was made in any of our print calls it is interesting to note that in our print(argv[0]) call the string view automatically determined the string length because this is a zero-terminated string by convention the other way around one cannot assume that it is possible to determine a string_view instances's data length by counting the number of items until a zero terminator is reached because of this we must always be careful about where we reach around a pointer to the string view data using string_view::data() usual string functions mostly assume zero termination and thus can buffer overflow very badly with raw pointers to the payload of a string view it is always better to use interfaces that already expect a string view apart from that we get a lot of the luxury interface we know from std::string already available we do data output via ostream and input via istream a class which inherits from both istream and ostream is iostream it combines both input and output capabilities when we understand how all classes from the trio consisting of istream ostream and iostream can be used we basically are ready to immediately put all following ones to use too: ifstream ofstream and fstream inherit from istream ostream and iostream respectively but lift their capabilities to redirect the i/o from and to files from the computer's filesystemthe istringstream ostringstream and iostringstream work pretty analogously they help build strings in memory and/or consuming data from them obviously strings can be added with the + operator like numbers but that has nothing to do with math but results in concatenated strings in order to mix this with string_view we need to convert to std::string first however it is really important to note that when mixing strings and string views in code we must never assume that the underlying string behind a string_view is zero terminated! this is why we would rather write abc  + string{some_string_view} than abc + some_string_viewdata() aside from that std::string provides a member function append which can handle string_view instances but it alters the string instead of returning a new one with the string view content appended std::string_view is useful but be cautious when mixing it with strings and string functions we cannot assume that they are zero-terminatedwhich breaks things quickly in a standard string environmentfortunately there are often proper function overloads which can deal with them the right way if we want to do complex string concatenation with formatting and so on we shouldhowever not do that piece by piece on string instances the std::stringstream std::ostringstream and std::istringstream classes are much better suited for this as they enhance the memory management while appending and provide all the formatting features we know from streams in general the std::ostringstream class is what we chose in this section because we were going to create a string instead of parsing it an std::istringstream instance could have been nstantiated from an existing string which we could have then сomfortably parsed into variables of other types if we want to combine both std::stringstream is the perfect all-rounder in this section we used string::find_first_not_of and string::find_last_not_of both functions accept a c-style string which acts as a list of characters that should be skipped while searching a different character if we have a string instance that carries the string  foo bar  and we call find_first_not_of( bfo on it it will return us the value 5 because the character is the first one that is not in the string the order of the characters in the argument string is not important the same functions exist with inverted logic although we did not use them in this recipe: string::find_first_of and string::find_last_of similar to iterator based functions we need to check if these functions return an actual position in the string or a value that denotes that they did not find a character position fulfilling the constraints if they did not find one they return string::npos from the character positions we retrieved from these functions in our helper function we built us a substring without surrounding whitespace using string::substring this function accepts a relative offset and a string length and then returns a new string instance with its own memory which contains only that substring for example string{abcdef}substr(2 2) will return us a new string cd if we for example try to read an integer but the input contains foobar",
        "as the next token then parsing this into the integer is not possible and the stream object enters a fail state this is only critical for the parsing attempt but not for the whole program it is okay to reset it and then to try anything else in our recipe program we tried to read a list of names after a potentially failing attempt to read two numbers in the case of a failing attempt to read those numbers in we used cinclear() to put cin back into a working state but then its internal cursor was still on what we typed instead of numbers in order to drop this old input and clear the pipe for the names input we used the very long expression cinignore(std::numeric_limits<std::streamsize>::max() '\n'); this is necessary to clear whatever is in the buffer at this point because we want to start with a really fresh buffer when we ask the user for a list of names"};

    const std::vector<std::string> queries = {
        /*1*/ "c++ c++11 c++17",
        /*2*/ "used have a are and such",
        /*3*/ "major transformation past such",
        /*4*/ "can type actual there specifying constructor does",
        /*5*/ "used member that return synonyms still need",
        /*6*/ "typedef declaration introduces a synonym",
        /*7*/ "different methods of initializing",
        /*8*/ "does not retain const",
        /*9*/ "foo is the one class that",
        /*10*/ "then back attempt this put a the with string",
        /*11*/ "c++ new code is the type and",
        /*12*/ "use of auto even case for",
        /*13*/ "very common we care about",
        /*14*/ "case with but explicitly",
        /*15*/ "of after for with character",
        /*16*/ "the of is after for different with character are",
        /*17*/ "foo used back member that return synonyms still need are synonym should",
        /*19*/ "major c++ used code transformation this past specifying such piece",
        /*20*/ "typedef declaration introduces a synonym very common we care about there specifying constructor does",
        /*21*/ "foo is the one class major does not retain const transformation past after for different with such that"};

    TestSpeed(docs, queries);
}

void TestFunctionality(const vector<string>& docs,
                       const vector<string>& queries,
                       const vector<string>& expected) {
    istringstream docs_input(Join('\n', docs));
    istringstream queries_input(Join('\n', queries));

    SearchServer srv;
    srv.UpdateDocumentBase(docs_input);
    ostringstream queries_output;
    srv.AddQueriesStream(queries_input, queries_output);

    const string result = queries_output.str();
    const auto lines = SplitBy(Strip(result), '\n');
    ASSERT_EQUAL(lines.size(), expected.size());
    for (size_t i = 0; i < lines.size(); ++i) {
        ASSERT_EQUAL(lines[i], expected[i]);
    }
}

void TestSerpFormat() {
    const vector<string> docs = {"london is the capital of great britain",
                                 "i am travelling down the river"};
    const vector<string> queries = {"london", "the"};
    const vector<string> expected = {
        "london: {docid: 0, hitcount: 1}",
        Join(' ', vector<string>{"the:", "{docid: 0, hitcount: 1}",
                                 "{docid: 1, hitcount: 1}"})};

    TestFunctionality(docs, queries, expected);
}

void TestTop5() {
    const vector<string> docs = {
        /*0*/ "milk a",
        /*1*/ "milk b",
        /*2*/ "milk c",
        /*3*/ "milk d",
        /*4*/ "milk e",
        /*5*/ "milk f",
        /*6*/ "milk g",
        /*7*/ "water a",
        /*8*/ "water b",
        /*9*/ "fire and earth"};

    const vector<string> queries = {"milk", "water", "rock"};
    const vector<string> expected = {
        Join(' ', vector<string>{"milk:", "{docid: 0, hitcount: 1}",
                                 "{docid: 1, hitcount: 1}", "{docid: 2, hitcount: 1}",
                                 "{docid: 3, hitcount: 1}", "{docid: 4, hitcount: 1}"}),
        Join(' ',
             vector<string>{
                 "water:",
                 "{docid: 7, hitcount: 1}",
                 "{docid: 8, hitcount: 1}",
             }),
        "rock:",
    };
    TestFunctionality(docs, queries, expected);
}

void TestHitcount() {
    const vector<string> docs = {
        "the river goes through the entire city there is a house near it",
        "the wall",
        "walle",
        "is is is is",
    };
    const vector<string> queries = {"the", "wall", "all", "is", "the is"};
    const vector<string> expected = {
        Join(' ',
             vector<string>{
                 "the:",
                 "{docid: 0, hitcount: 2}",
                 "{docid: 1, hitcount: 1}",
             }),
        "wall: {docid: 1, hitcount: 1}",
        "all:",
        Join(' ',
             vector<string>{
                 "is:",
                 "{docid: 3, hitcount: 4}",
                 "{docid: 0, hitcount: 1}",
             }),
        Join(' ',
             vector{
                 "the is:",
                 "{docid: 3, hitcount: 4}",
                 "{docid: 0, hitcount: 3}",
                 "{docid: 1, hitcount: 1}",
             }),
    };
    TestFunctionality(docs, queries, expected);
}

void TestRanking() {
    const vector<string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockgolm is the capital of sweden",
        "riga is the capital of latvia",
        "tallin is the capital of estonia",
        "warsaw is the capital of poland",
    };

    const vector<string> queries = {"moscow is the capital of russia"};
    const vector<string> expected = {
        Join(' ', vector<string>{
                      "moscow is the capital of russia:",
                      "{docid: 7, hitcount: 6}",
                      "{docid: 14, hitcount: 6}",
                      "{docid: 0, hitcount: 4}",
                      "{docid: 1, hitcount: 4}",
                      "{docid: 2, hitcount: 4}",
                  })};
    TestFunctionality(docs, queries, expected);
}

void TestBasicSearch() {
    const vector<string> docs = {
        "we are ready to go",
        "come on everybody shake you hands",
        "i love this game",
        "just like exception safety is not about writing try catch everywhere "
        "in your code move semantics are not about typing double ampersand "
        "everywhere in your code",
        "daddy daddy daddy dad dad dad",
        "tell me the meaning of being lonely",
        "just keep track of it",
        "how hard could it be",
        "it is going to be legen wait for it dary legendary",
        "we dont need no education"};

    const vector<string> queries = {"we need some help", "it",
                                    "i love this game", "tell me why",
                                    "dislike", "about"};

    const vector<string> expected = {
        Join(' ', vector<string>{"we need some help:", "{docid: 9, hitcount: 2}",
                                 "{docid: 0, hitcount: 1}"}),
        Join(' ',
             vector<string>{
                 "it:",
                 "{docid: 8, hitcount: 2}",
                 "{docid: 6, hitcount: 1}",
                 "{docid: 7, hitcount: 1}",
             }),
        "i love this game: {docid: 2, hitcount: 4}",
        "tell me why: {docid: 5, hitcount: 2}",
        "dislike:",
        "about: {docid: 3, hitcount: 2}",
    };
    TestFunctionality(docs, queries, expected);
}

void My_TestBasicSearch() {
    const vector<string> docs = {
        /*0*/ "we are ready to love",                                                 // hitcount = 1 (love:1)
        /*1*/ "come on everybody and shake you hands",                                // hitcount = 1 (and:1)
        /*2*/ "i love this game",                                                     // hitcount = 4 (i:1, love:1, this:1, game:1)
        /*3*/ "just like exception safety is not about simply try catch everywhere ", // hitcount = 2 (simply:1, like:1)
        /*4*/ "in your code move semantics are not about typing double ampersand ",   // hitcount = 0 (simply:1, like:1)
        /*5*/ "everywhere in your code",                                              // hitcount = 0 (simply:1, like:1)
        /*6*/ "daddy and daddy and daddy and dad dad dad",                            // hitcount = 3 (daddy:3)
        /*7*/ "masha i can tell yout the meaning of words game and love",             // hitcount = 5 (i:1, and:1, masha:1, love:1, game:1)
        /*8*/ "simply love masha and play game with her",                             // hitcount = 5 (and:1, masha:1, love:1, game:1, simply:1)
        /*9*/ "how hard could it be",                                                 // hitcount = 1 (it:1)
        /*10*/ "it is going to be legen wait for it dary legendary",                  // hitcount = 2 (it:2)
        /*11*/ "we dont need no education"                                            // hitcount = 2
    };

    const vector<string> queries = {
        "i and masha love this game helen simply like it"};

    const vector<string> expected = {Join(
        ' ', vector<string>{"i and masha love this game helen simply like it:",
                            "{docid: 7, hitcount: 5}", "{docid: 8, hitcount: 5}",
                            "{docid: 2, hitcount: 4}", "{docid: 6, hitcount: 3}",
                            "{docid: 3, hitcount: 2}"})};

    TestFunctionality(docs, queries, expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test_Duration);
    RUN_TEST(tr, TestSerpFormat);
    RUN_TEST(tr, TestTop5);
    RUN_TEST(tr, TestHitcount);
    RUN_TEST(tr, TestRanking);
    RUN_TEST(tr, TestBasicSearch);
    RUN_TEST(tr, My_TestBasicSearch);
}
