#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

class HumanRole {
   public:
    HumanRole(const std::string& name, const std::string& role_type)
        : _name(name), _role_type(role_type) {}

    virtual void Walk(const std::string& destination) const {
        std::cout << _role_type << ": "s << _name << " walks to: "s << destination << std::endl;
    }

    std::string Name() const { return _name; }

    std::string RoleType() const { return _role_type; }

   protected:
    const std::string _name{};
    const std::string _role_type{};
};

class Student : public HumanRole {
   public:
    Student(const std::string& name, const std::string& favourite_song)
        : HumanRole(name, "Student"s), _favourite_song(favourite_song) {}

    void Learn() {
        std::cout << _role_type << ": "s << _name << " learns"s << std::endl;
    }

    void Walk(const std::string& destination) const override {
        HumanRole::Walk(destination);
        SingSong();
    }

    void SingSong() const {
        std::cout << _role_type << ": "s << _name << " sings a song: "s << _favourite_song << std::endl;
    }

   private:
    const std::string _favourite_song;
};

class Teacher : public HumanRole {
   public:
    Teacher(const std::string& name, const std::string& subject)
        : HumanRole(name, "Teacher"s), _subject(subject) {}

    void Teach() {
        std::cout << _role_type << ": "s << _name << " teaches: "s << _subject << std::endl;
    }

   private:
    const std::string _subject;
};

class Policeman : public HumanRole {
   public:
    Policeman(const std::string& name)
        : HumanRole(name, "Policeman"s) {}

    void Check(const HumanRole& hr) {
        std::cout << _role_type << ": " << _name
                  << " checks "s << hr.RoleType() << ". "s << hr.RoleType() << "'s name is: " << hr.Name() << std::endl;
    }
};

void VisitPlaces(const HumanRole& h, const std::vector<std::string>& places) {
    for (auto p : places) {
        h.Walk(p);
    }
}

int main() {
    Teacher t("Jim", "Math");
    Student s("Ann", "We will rock you");
    Policeman p("Bob");

    VisitPlaces(t, {"Moscow", "London"});
    p.Check(s);
    VisitPlaces(s, {"Moscow", "London"});
    return 0;
}