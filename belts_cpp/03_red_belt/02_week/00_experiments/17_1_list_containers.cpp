#include <iostream>
#include <list>
#include <string>
#include <utility>  // std::move

using namespace std::literals;

int main() {
    std::list<int> lst_1 = {1, 2, 3, 4, 5};
    std::list<int> lst_2 = {11, 12, 13, 14, 15, 16, 17};

    // {
    //     auto it = std::next(std::next(lst_1.begin()));

    //     // 1-ый вариант перегруженного метода splice
    //     // void splice( const_iterator pos, list& other ); 	(1)
    //     lst_1.splice(it, lst_2);
    // }

    // {
    //     auto it = std::next(std::next(lst_1.begin()));

    //     // 2-ой вариант перегруженного метода splice
    //     // void splice( const_iterator pos, list&& other );	    (2) 	(since C++11)
    //     lst_1.splice(it, std::move(lst_2));
    // }

    // {
    //     auto it = std::next(lst_1.begin(), 3);

    //     // 3-й вариант перегруженного метода splice
    //     // void splice( const_iterator pos, list& other, const_iterator it );	(3)
    //     // перемещает элемент, на который указывает итератор "it", из "other" в "*this"
    //     // и вставляет его перед элементом, на который указывает итератор "pos"
    //     lst_1.splice(it, lst_2, std::next(lst_2.begin(), 2));
    // }

    // {
    //     auto it = std::next(lst_1.begin(), 3);

    //     // 4-й вариант перегруженного метода splice
    //     // void splice( const_iterator pos, list&& other, const_iterator it );	(4)
    //     // перемещает элемент, на который указывает итератор "it", из "other" в "*this"
    //     // и вставляет его перед элементом, на который указывает итератор "pos"
    //     lst_1.splice(it, std::move(lst_2), std::next(lst_2.begin(), 2));
    // }

    {
        // 5-й вариант перегруженного метода splice
        // void splice( const_iterator pos, list& other,
        //              const_iterator first, const_iterator last);        (5)

        // перемещает элементы из диапазона "[first, last)" из "other" в "*this";
        // элементы вставляются перед элементом, на который указывает итератор "pos"
        lst_1.splice(std::next(lst_1.begin(), 3), lst_2,
                               std::next(lst_2.begin(), 2), std::next(lst_2.begin(), 4));
    }

    std::cout << "Contents of list l1 after splice operation" << std::endl;

    for (auto it = lst_1.begin(); it != lst_1.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    return 0;
}
