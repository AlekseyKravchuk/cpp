#include "iterator"
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using namespace chrono;

#include <algorithm>

template <typename It>
class IteratorRange {
  public:
    IteratorRange(It first, It last) : first(first), last(last) {}

    It begin() const {
        return first;
    }

    It end() const {
        return last;
    }

    size_t size() const {
        return last - first;
    }

  private:
    It first, last;
};

template <typename Container>
auto Head(Container& c, int top) {
    return IteratorRange(std::begin(c), std::begin(c) + std::min<size_t>(std::max(top, 0), c.size()));
}

template <typename Container>
std::string Join(char c, const Container& cont) {
    std::ostringstream os;

    for (const auto& item : Head(cont, cont.size() - 1)) {
        os << item << c;
    }

    os << *rbegin(cont);
    return os.str();
}

struct TotalDuration {
    string message;
    steady_clock::duration value;

    explicit TotalDuration(const string& msg = "")
        : message(msg + ": "),
          value(0) {}

    ~TotalDuration() {
        ostringstream os;
        os << message
           //    << duration_cast<milliseconds>(value).count()
           << duration_cast<microseconds>(value).count()
           //    << " ms" << endl;
           << " microseconds" << endl;
        cerr << os.str();
    }
};

class AddDuration {
  public:
    explicit AddDuration(steady_clock::duration& dest)
        : add_to(dest),
          start(steady_clock::now()) {}

    explicit AddDuration(TotalDuration& dest)
        : AddDuration(dest.value) {}

    ~AddDuration() {
        add_to += steady_clock::now() - start;
    }

  private:
    steady_clock::duration& add_to;
    steady_clock::time_point start;
};

// // Yandex implementation
// #define UNIQ_ID_IMPL_2(lineno) _a_local_var_##lineno
// #define UNIQ_ID_IMPL(lineno) UNIQ_ID_IMPL_2(lineno)
// #define UNIQ_ID UNIQ_ID_IMPL(__LINE__)

#define TOKEN_PASTE(x, y) _x##y
#define CAT(x, y) TOKEN_PASTE(x, y)
#define UNIQ_ID CAT(id_, __LINE__)

#define ADD_DURATION(value) \
    AddDuration UNIQ_ID{value}

istream& ReadLine(istream& input, string& s, TotalDuration& dest) {
    ADD_DURATION(dest);
    return getline(input, s);
}

std::vector<std::string_view> SplitIntoWordsView(std::string_view sview) {
    std::vector<std::string_view> result;

    size_t start_pos = 0;

    while (true) {
        size_t space_pos = sview.find(' ', start_pos);

        // нужно учесть случай, когда пробел не был найден => space_pos == std::string_view::npos
        result.push_back(space_pos == std::string_view::npos ? sview.substr(start_pos)
                                                             : sview.substr(start_pos, space_pos - start_pos));

        // если пробел не нейден, заканчиваем цикл
        if (space_pos == sview.npos) {
            break;
        } else { // иначе двигаем "start_pos" на первую непробельную позицию
            while (sview[space_pos] == ' ') {
                ++space_pos;
            }
            start_pos = space_pos;
        }
    }

    return result;
}

int main() {
    const std::vector<std::string> docs = {
        "the c++ language has gone through a major transformation in the past decade with the development and release of c++11 and then later with its newer versions c++14 and c++17 these new standards have introduced new concepts simplified or extended existing syntax and semantics and overall transformed the way we write code. c++11 looks like a new language and code written using the new standards is called modern c++ code"
        "tirane is the capital of albania",
        "automatic type deduction is one of the most important and widely used features in modern c++ the new c++ standards have made it possible to use auto as a placeholder for types in various contexts and let the compiler deduce the actual type in c++11, auto can be used for declaring local variables and for the return type of a function with a trailing return type in c++14 auto can be used for the return type of a function without specifying a trailing type and for parameter declarations in lambda expressions future standard versions are likely to expand the use of auto to even more cases",
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
        "apart from the different methods of initializing the data there are also some limitations for instance the only way to initialize a standard container was to first declare an object and then insert elements into it; vector was an exception because it is possible to assign values from an array that can be prior initialized using aggregate initialization on the other hand however dynamically allocated aggregates could not be initialized directly all the examples in the how to do it section use direct initialization but copy initialization is also possible with brace-initialization the two forms direct and copy initialization may be equivalent in most cases but copy initialization is less permissive because it does not consider explicit constructors in its implicit conversion sequence that must produce an object directly from the initializer whereas direct initialization expects an implicit conversion from the initializer to an argument of the constructor dynamically allocated arrays can only be initialized using direct initialization",
        "of the classes shown in the preceding examples foo is the one class that has both a default constructor and a constructor with parameters to use the default constructor to perform default initialization we need to use empty braces that is {} to use the constructor with parameters we need to provide the values for all the arguments in braces {} unlike non-aggregate types where default initialization means invoking the default constructor for aggregate types default initialization means initializing with zeros",
        "constructors are a place where non-static class member initialization is done many developers prefer assignments in the constructor body aside from the several exceptional cases when that is actually necessary initialization of non-static members should be done in the constructor's initializer list or as of c++11 using default member initialization when they are declared in the class prior to c++11 constants and non-constant non-static data members of a class had to be initialized in the constructor initialization on declaration in a class was only possible for static constants as we will see further this limitation was removed in c++11 that allows initialization of non-statics in the class declaration this initialization is called default member initialization and is explained in the next sections",
        "many developers however do not use the initializer list but prefer assignments in the constructor's body or even mix assignments and the initializer list that could be for several reasons--for larger classes with many members the constructor assignments may look easier to read than long initializer lists perhaps split on many lines or it could be because they are familiar with other programming languages that don't have an initializer list or because unfortunately for various reasons they don't even know about it",
        "using assignments in the constructor is not efficient as this can create temporary objects that are later discarded if not initialized in the initializer list non-static members are initialized via their default constructor and then when assigned a value in the constructor's body the assignment operator is invoked this can lead to inefficient work if the default constructor allocates a resource (such as memory or a file) and that has to be deallocated and reallocated in the assignment operator",
        "in the preceding example defaultheight and defaultwidth are both constants therefore the values do not depend on the way the object is constructed so they are initialized when declared the textflow object is a non-constant non-static data member whose value also does not depend on the way the object is initialized (it could be changed via another member function) therefore it is also initialized using default member initialization when it is declared text on the other hand is also a non-constant non-static data member but its initial value depends on the way the object is constructed and therefore it is initialized in the constructor's initializer list using a value passed as an argument to the constructor",
        "c++11 provides standardized methods for specifying and querying the alignment requirements of a type (something that was previously possible only through compiler-specific methods) controlling the alignment is important in order to boost performance on different processors and enable the use of some instructions that only work with data on particular alignments for example intel sse and intel sse2 require 16 bytes alignment of data whereas for intel advanced vector extensions (or intel avx) it is highly recommended to use 32 bytes alignment this recipe explores the alignas specifier for controlling the alignment requirements and the alignof operator that retrieves the alignment requirements of a type",
        "unlike other similar programming languages c++ does not have a specific syntax for declaring interfaces (that are basically classes with pure virtual methods only) and also has some deficiencies related to how virtual methods are declared in c++ the virtual methods are introduced with the virtual keyword however the keyword virtual is optional for declaring overrides in derived classes that can lead to confusion when dealing with large classes or hierarchies you may need to navigate throughout the hierarchy up to the base to figure out whether a function is virtual or not on the other hand sometimes it is useful to make sure that a virtual function or even a derived class can no longer be overridden or derived further in this recipe we will see how to use c++11 special identifiers override and final to declare virtual functions or classes",
        "as we have seen in the preceding recipe the range-based for loops known as for each in other programming languages allows you to iterate over the elements of a range providing a simplified syntax over the standard for loops and making the code more readable in many situations however range-based for loops do not work out of the box with any type representing a range but require the presence of a begin() and end() function (for non-array types) either as a member or free function in this recipe we will see how to enable a custom type to be used in range-based for loops",
        "before c++11 a constructor with a single parameter was considered a converting constructor with c++11 every constructor without the explicit specifier is considered a converting constructor such a constructor defines an implicit conversion from the type or types of its arguments to the type of the class classes can also define converting operators that convert the type of the class to another specified type all these are useful in some cases but can create problems in other cases in this recipe we will see how to use explicit constructors and conversion operators",
        "perhaps a more realistic example to understand where problems could arise would be to consider a string buffer implementation this would be a class that contains an internal buffer of characters the class may provide several conversion constructors: a default constructor a constructor that takes a size_t parameter representing the size of the buffer to preallocate and a constructor that takes a pointer to char that should be used to allocate and initialize the internal buffer succinctly such a string buffer could look like this",
        "in this case b4 is initialized with a char since an implicit conversion to size_t exists the constructor with a single parameter will be called the intention here is not necessarily clear perhaps it should have been instead of 'a' in which case the third constructor would have been called however b5 is most likely an error because maxsize is an enumerator representing an itemsizes and should have nothing to do with a string buffer size these erroneous situations are not flagged by the compiler in any way using the explicit specifier in the declaration of a constructor that constructor becomes an explicit constructor and no longer allows implicit constructions of objects of a class type to exemplify this we will slightly change the string_buffer class earlier to declare all constructors explicit",
        "the larger a program the greater the chances are you could run into name collisions with file locals when your program is linked functions or variables that are declared in a source file and are supposed to be local to the translation unit may collide with other similar functions or variables declared in another translation unit that is because all symbols that are not declared static have external linkage and their names must be unique throughout the program the typical c solution for this problem is to declare those symbols static changing their linkage from external to internal and therefore making them local to a translation unit in this recipe we will look at the c++ solution for this problem",
        "first of all it declares a namespace with a unique name (what the name is and how it generates that name is a compiler implementation detail and should not be a concern) at this point the namespace is empty and the purpose of this line is to basically establish the namespace second a using directive brings everything from the _unique_name_namespace into the current namespace third the namespace with the compiler-generated name is defined as it was in the original source code (when it had no name) by defining the translation unit local print() functions in an unnamed namespace they have local visibility only yet their external linkage no longer produces linkage errors since they now have external unique names unnamed namespaces are also working in a perhaps more obscure situation involving templates template arguments cannot be names with internal linkage so using static variables is not possible on the other hand symbols in an unnamed namespace have external linkage and can be used as template arguments this problem is shown in the following example where declaring t1 produces a compiler error because the non-type argument expression has internal linkage however t2 is correct because size2 has external linkage",
        "all these engines and engine adaptors are producing pseudo-random numbers the library however provides another engine called random_device that is supposed to produce non-deterministic numbers but this is not an actual constraint as physical sources of random entropy might not be available therefore implementations of random_device could actually be based on a pseudo-random engine the random_device class cannot be seeded like the other engines and has an additional method called entropy() that returns the random device entropy which is 0 for a deterministic generator and nonzero for a non-deterministic generator however this is not a reliable method for determining whether the device is actually deterministic or non-deterministic for instance both gnu libstdc++ and llvm libc++ implement a non-deterministic device but return 0 for entropy on the other hand vc++ and boostrandom return 32 and 10 respectively for entropy",
        "each of the engines provided by the library has advantages and disadvantages the linear congruential engine has a small internal state but it is not very fast on the other hand the subtract with carry engine is very fast but requires more memory for its internal state the mersenne twister is the slowest of them and the one that has the largest internal state but when initialized appropriately can produce the longest non-repeating sequence of numbers in the following examples we will use std::mt19937 a 32-bit mersenne twister with 19937 bits of internal state"};

    std::istringstream iss(Join('\n', docs));
    
    TotalDuration read("Total read");
    TotalDuration parse("Total parse");

    for (string line; ReadLine(iss, line, read);) {
        ADD_DURATION(parse);
        const auto words = SplitIntoWordsView(line);
    }
}
