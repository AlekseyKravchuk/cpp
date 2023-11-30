// program parses command line arguments and print out natural sequence in direct or reverse mode
#include <iostream>

// for getopt()
// #include <unistd.h>

// for getopt_long
#include <getopt.h>


/*
  getopt_long() description (defined in getopt.h):
  int getopt_long(int argc, char * const argv[],  const char *optstring, const struct option *longopts, int *longindex);

  Для работы функции getopt_long ей нужны следующие данные:

  argc - счетчик количества параметров командой строки argc.
  argv - значения парамеров командной строки argv[].
  optstring - список коротких названий параметров, например: const char* const short_opts = "n:o:";
  longopts - специальный массив с названиями длинных параметров, например:
  const option long_opts[] = {
         {"num", required_argument, nullptr, 'n'},
         {"beep", no_argument, nullptr, 'b'},
         {"sigma", required_argument, nullptr, 's'},
         {"writeFile", required_argument, nullptr, 'w'},
         {"help", no_argument, nullptr, 'h'},
         {nullptr, no_argument, nullptr, 0}
  };
  longindex - указатель на переменную, в которую будет помещен индекс текущего параметра из массива longopts.

  Для того, чтобы одновременно использовать и короткие, и длинные опции, указатели flag в структуре типа "option"
  (в примере, она называется long_opts) устанавливают в nullptr (NULL), а значения val устанавливают в названия коротких параметров.
  При этом вся обработка результатов происходит в switch структуре, так же как и в случае с getopt(), единственно, нужно определить
  структуру типа "option"
 */


void print_reverse(int n) {
  if(n == 0) {
    return;
  }
  else {
    std::cout << n << " ";
    print_reverse(n-1);
  }
}


void print_direct(int n) {
  if(n == 0) {
    return;
  }
  else {
    print_direct(n-1);
    std::cout << n << " ";
  }
}


void printHelp()
{
    std::cout <<
        "--num <n>:           Set amount of numbers to be displayed \n"
        "--order <val>:       Set the order, possible values: direct or reverse\n"
        "--help:              Show help" << std::endl;
    exit(1);
}


int main(int argc, char* argv[]) {
    int num{0}; // number of nuturals

    if(argc != 5) {
        std::cout << "Usage: " << argv[0] << " -n<--num> number -o<--order> [direct|reverse]" << std::endl;
        return -1;
    }

    // ------------------------------ parsing of command line arguments ------------------------------
    void (*print_ptr) (int){nullptr};

    // если ключевое слово CONST находится в объявлении СЛЕВА от *, то константным явл. указываемое значение
    // если ключевое слово CONST находится в объявлении СПРАВА от *, то константным явл. УКАЗАТЕЛЬ
    const char* const short_opts = "n:o:";

    const option long_opts[] = {
                                {"num", required_argument, nullptr, 'n'},
                                {"order", required_argument, nullptr, 'o'},
                                {"help", no_argument, nullptr, 'h'}
    };

    while(true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if(opt == -1) {
            break;
        }

        switch(opt) {
            case 'n':
                if(optarg) {
                    num = std::stoi(optarg);
                }
                break;
            case 'o':
                if(optarg) {
                    if(std::string(optarg) == "direct") {
                        print_ptr = print_direct;
                    } else if (std::string(optarg) == "reverse") {
                        print_ptr = print_reverse;
                    } else {
                        std::cout << "Wrong argument for option --order\n";
                        std::cout << "Usage: " << argv[0] << " -n<--num> number -o<--order> [direct|reverse]" << std::endl;
                        printHelp();
                        return -1;
                    }
                }
                break;
            case 'h':
                printHelp();
                break;
            default:
                printHelp();
                return -1;
        }
    }
    print_ptr(num);

    std::cout << std::endl;

    return 0;
}
