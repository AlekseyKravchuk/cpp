import psutil
import argparse

# Команда покажет открытые TCP/UDP соединения для процесса с PID 38325. Если ничего не найдено, значит, процесс действительно не имеет открытых сокетов.
# sudo ss -tulpn | grep 38325

def find_sockets_by_pid(pid):
    try:
        process = psutil.Process(pid)
        print(f"Процесс найден: {process.name()}")

        # Получение всех соединений
        connections = psutil.net_connections(kind="inet")
        filtered_connections = [conn for conn in connections if conn.pid == pid]

        if not filtered_connections:
            print(f"Процесс {pid} не имеет открытых сокетов.")
            return

        print(f"Сокеты для процесса {pid} ({process.name()}):")
        for conn in filtered_connections:
            laddr = f"{conn.laddr.ip}:{conn.laddr.port}" if conn.laddr else "N/A"
            raddr = f"{conn.raddr.ip}:{conn.raddr.port}" if conn.raddr else "N/A"
            print(f"  - Тип: {conn.type}, Статус: {conn.status}, Локальный: {laddr}, Удалённый: {raddr}")
    except psutil.NoSuchProcess:
        print(f"Процесс с PID {pid} не найден.")
    except Exception as e:
        print(f"Ошибка: {e}")


def main():
    parser = argparse.ArgumentParser(description="Отслеживание сокетов по PID процесса.")
    parser.add_argument("pid", type=int, help="PID процесса, который нужно отслеживать.")
    args = parser.parse_args()

    find_sockets_by_pid(args.pid)


if __name__ == "__main__":
    main()
