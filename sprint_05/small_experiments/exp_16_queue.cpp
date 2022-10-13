// queue::front
#include <iostream>       // std::cout
#include <queue>          // std::queue

int main ()
{
  std::queue<int> myqueue;

  myqueue.push(77);
  myqueue.push(16);
  myqueue.push(88);

  auto c1 = myqueue.front();
  auto c2 = myqueue.front();

  myqueue.pop();


  std::cout << "myqueue.front() is now " << myqueue.front() << '\n';

  return 0;
}