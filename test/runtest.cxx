#include <iostream>
#include <iomanip>
#include "cutee.h"
using namespace std;
using namespace cutee;

// static vars initialization
CuteeTest* TestList::list[MAX_TEST_COUNT];
int TestList::list_idx = 0;

int main(int argc, char **argv) 
{
	try {
	Runner r(argc, argv);
	r.run();
	} catch(...) {
     return 1;
	}
	return 0;
}
