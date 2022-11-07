#include <cstddef>  // std::size_t
#include <vector>

using namespace std;

class Tentacle {
   public:
    explicit Tentacle(int id) noexcept;
    int GetId() const noexcept;

    Tentacle* GetLinkedTentacle() const noexcept {
        return linked_tentacle_;
    }
    void LinkTo(Tentacle& tentacle) noexcept {
        linked_tentacle_ = &tentacle;
    }
    void Unlink() noexcept {
        linked_tentacle_ = nullptr;
    }

   private:
    int id_ = 0;
    Tentacle* linked_tentacle_ = nullptr;
};

class Octopus {
   public:
    Octopus();
    explicit Octopus(int num_tentacles);

    void AddTentacle();
    size_t GetTentacleCount() const noexcept;
    const Tentacle& GetTentacle(size_t index) const;
    Tentacle& GetTentacle(size_t index);

    ~Octopus();

   private:
    void Cleanup() noexcept;

    vector<Tentacle*> tentacles_;
};

int main() {
    Octopus octopus1();
    // Octopus octopus2;

    // // Два осьминога прицепляются друг к другу щупальцами
    // octopus1.GetTentacle(1).LinkTo(octopus2.GetTentacle(3));
    // octopus2.AddTentacle();
    // octopus2.GetTentacle(octopus2.GetTentacleCount() - 1).LinkTo(octopus1.GetTentacle(0));
}