#include "tests.h"
#include "transport_guide.h"

using namespace std;

int main() {
//    TestAll();
    TransportGuide guide;
    guide.ProcessCreationQueries(cin);
    guide.ProcessRetrievalQueries(cin);

    return 0;
}