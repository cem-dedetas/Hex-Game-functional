#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

int main() {
  srand((unsigned)time(NULL));
  int r[2];

  for(int i =0; i<2; ++i)r[i] = rand()%12;

  cout<<r[0]<<endl<<r[1];

  for(int i =0; i<2; ++i)r[i] = rand()%12;

  cout<<r[0]<<endl<<r[1];

  return 0;
}