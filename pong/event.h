#include <functional>
#include <vector>

// Event system
template<typename... Args>
class Event
{
public:
    void AddListener(std::function<void(Args...)> listener)
    {
        listeners.push_back(listener);
    }

    void Notify(Args... args)
    {
        for (auto listener : listeners)
        {
            listener(args...);
        }
        ++count;
    }

    int GetCount() const {
        return count;
    }

private:
    std::vector<std::function<void(Args...)>> listeners;
    int count = 0;
};