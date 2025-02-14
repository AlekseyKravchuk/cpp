#include "parsing.h"

namespace po = boost::program_options;
namespace fs = std::filesystem;
using std::string;
using std::cout;
using std::cerr;

void echo_client_check_arguments(int argc,
                                 char* argv[],
                                 string& server_ip,
                                 uint16_t& server_port) {
    // Declare the supported options using the options_description class.
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("ipaddr,i", po::value<string>()->required(), "server IP address to connect to")
            ("port,p", po::value<uint16_t>()->required(), "server port to connect to");

    // переменная для хранения значений опций
    po::variables_map vars_map;

    // парсинг аргументов
    try {
        po::store(po::parse_command_line(argc, argv, desc), vars_map);

        if (vars_map.count("help")) {
            std::cout << desc;
            exit(EXIT_SUCCESS);
        }

        // Вызов notify предназначен для обработки обязательных аргументов;
        // проверяет, корректно ли заполнены значения опций в vars_map (объект po::variables_map)
        // и вызывает обработчики (notifiers), если они были заданы для опций.
        po::notify(vars_map);

        // Присваиваем значения из командной строки в переменные
        server_ip = vars_map["ipaddr"].as<string>();
        server_port = vars_map["port"].as<uint16_t>();
    } catch (const po::error& e) {
        // Если обязательная опция отсутствует, выводим пользовательское сообщение
        std::cerr << "Error: Missing required option: " << e.what() << "\n";

        fs::path full_path = fs::absolute(argv[0]); // Получаем абсолютный путь
        fs::path relative_path = fs::relative(full_path, fs::current_path()); // Преобразуем в относительный

        std::cout << "usage: " << relative_path << " <server_ip> <port>\n";
        desc.print(std::cerr);
        exit(EXIT_FAILURE);
    } catch (const std::exception& e) {  // Ловим другие возможные ошибки
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

void echo_server_check_arguments(int argc,
                                 char* argv[],
                                 uint16_t& port_listen_to) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce help message")
            ("port,p", po::value<uint16_t>()->required(), "port to listen on");

    po::variables_map vars_map;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vars_map);

        if (vars_map.count("help")) {
            std::cout << desc << "\n";
            exit(EXIT_SUCCESS);;
        }

        // Вызов notify для обработки обязательных аргументов
        po::notify(vars_map);

        // Присваиваем значения из командной строки в переменные
        port_listen_to = vars_map["port"].as<uint16_t>();

        std::cout << "Command args successfully parsed, port to listen to (server side): " << port_listen_to << "\n";
    } catch (const po::error& e) {
        // Если обязательная опция отсутствует, выводим пользовательское сообщение
        fs::path full_path = fs::absolute(argv[0]); // Получаем абсолютный путь
        fs::path relative_path = fs::relative(full_path, fs::current_path()); // Преобразуем в относительный

        std::cerr << "Error: Missing required option: " << e.what() << "\n";
        std::cout << "usage: " << relative_path << " <port_listen_to> <file_to_read_text_from>\n";
        desc.print(std::cerr);
        exit(EXIT_FAILURE);
    } catch (const std::exception& e) {
        // Ловим другие возможные ошибки
        std::cerr << "Error: " << e.what() << "\n";
        exit(EXIT_FAILURE);
    }
}