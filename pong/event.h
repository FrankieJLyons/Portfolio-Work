#include <functional>
#include <vector>

// Event system
template<typename... Args>
class Event {
public:
    Event() {}
    ~Event() {
        ClearListeners();
    }

    void AddListener(std::function<void(Args...)> listener) {
        listeners.push_back(listener);
    }

    void ClearListeners() {
        listeners.clear();
    }

    void Notify(Args... args) {
        for (auto listener : listeners){
            listener(args...);
        }
    }

private:
    std::vector<std::function<void(Args...)>> listeners;
};