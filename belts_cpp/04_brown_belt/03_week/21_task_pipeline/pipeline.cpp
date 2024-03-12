#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Email {
    string from;
    string to;
    string body;
};

// // ============================== class Worker ==============================
class Worker {
  public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;

    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

  protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const;

  public:

    void SetNext(unique_ptr<Worker> next);

  private:
    unique_ptr<Worker> _next;
};

// вызывает метод Process следующего обработчика, если он был установлен
void Worker::PassOn(unique_ptr<Email> email) const {
    if (_next) {
        _next->Process(std::move(email));
    }
}

// принимает во владение следующий обработчик из цепочки
void Worker::SetNext(unique_ptr<Worker> next) {
    _next = std::move(next);
}
// // ========================== END of class Worker ==========================

// // ============================== class Reader =============================
class Reader : public Worker {
  public:
    explicit Reader(istream& in);
    void Process(unique_ptr<Email> email) override;
    virtual void Run() override;

  private:
    istream& _in;
};

Reader::Reader(istream& in) : _in(in) {}

void Reader::Process(unique_ptr<Email> email) {
    Worker::PassOn(std::move(email));
}

unique_ptr<Email> ReadEmail(istream& in) {
    string from, to, body;
    bool read = getline(in, from) &&
                getline(in, to) &&
                getline(in, body);
    return read
           ? make_unique<Email>(Email{from, to, body})
           : nullptr;
}

void Reader::Run() {
    bool keep_read{true};
    while (keep_read) {
        unique_ptr<Email> email = ReadEmail(_in);
        if (email) {
            Process(std::move(email));
        } else {
            keep_read = false;
        }
    }
}
// // =========================== END of class Reader ==========================

// // ============================== class Filter =============================
class Filter : public Worker {
  public:
    // Function - callable type (returns 'bool', accepts 1 parameter: const Email&)
    using Function = function<bool(const Email&)>;

  public:
    explicit Filter(Function f) : _filter_func(std::move(f)) {}
    void Process(unique_ptr<Email> email) override;

  private:
    Function _filter_func;
};

void Filter::Process(unique_ptr<Email> email) {
    if (_filter_func(*email)) {
        Worker::PassOn(std::move(email));
    }
}
// // =========================== END of class Filter ==========================

// // ============================== class Copier ==============================
class Copier : public Worker {
  public:
    explicit Copier(string to) : _to(std::move(to)) {}
    void Process(unique_ptr<Email> email) override;

  private:
    string _to;
};

void Copier::Process(unique_ptr<Email> email) {
    unique_ptr<Email> copy;
    if (_to != email->to) {
        copy = make_unique<Email>(Email{email->from, _to, email->body});
    }
    Worker::PassOn(std::move(email));

    if (copy) {
        Worker::PassOn(std::move(copy));
    }
}
// // =========================== END of class Copier ==========================

// // ============================== class Sender ==============================
class Sender : public Worker {
  public:
    explicit Sender(ostream& os) : _os(os) {}       // принимает поток для записи сообщений
    void Process(unique_ptr<Email> email) override;

  private:
    ostream& _os;
};

// записывает сообщения в поток в том же формате, в котором они были прочитаны
void Sender::Process(unique_ptr<Email> email) {
    _os << email->from << endl
        << email->to << endl
        << email->body << endl;

    Worker::PassOn(std::move(email));
}
// // =========================== END of class Sender ==========================


// // ========================= class PipelineBuilder ==========================
class PipelineBuilder {
  public:
    explicit PipelineBuilder(istream& in);               // добавляет в качестве первого обработчика Reader
    PipelineBuilder& FilterBy(Filter::Function filter);  // добавляет новый обработчик Filter
    PipelineBuilder& CopyTo(string recipient);           // добавляет новый обработчик Copier
    PipelineBuilder& Send(ostream& out);                 // добавляет новый обработчик Sender
    unique_ptr<Worker> Build();                          // возвращает готовую цепочку обработчиков

  private:
    vector<unique_ptr<Worker>> _workers;
};

PipelineBuilder::PipelineBuilder(istream& in) {
    _workers.push_back(make_unique<Reader>(in));
}

PipelineBuilder& PipelineBuilder::FilterBy(Filter::Function filter) {
    _workers.push_back(make_unique<Filter>(filter));
    return *this;
}

PipelineBuilder& PipelineBuilder::CopyTo(string recipient) {
    _workers.push_back(make_unique<Copier>(std::move(recipient)));
    return *this;
}

PipelineBuilder& PipelineBuilder::Send(ostream& out) {
    _workers.push_back(make_unique<Sender>(out));
    return *this;
}

// строит цепочку с помощью метода Worker::SetNext и возвращает первый обработчик в построенной цепочке
unique_ptr<Worker> PipelineBuilder::Build() {
    if (_workers.size() > 1) {
        for (size_t i = _workers.size() - 1; i > 0; --i) {
            _workers[i-1]->SetNext(std::move(_workers[i]));
        }
    }

    return std::move(_workers[0]);
}
// // ===================== END of class PipelineBuilder =======================

void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy([](const Email& email) {
        return email.from == "erich@example.com";
    });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "erich@example.com\n"
            "richard@example.com\n"
            "Are you sure you pressed the right button?\n"
    );

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
