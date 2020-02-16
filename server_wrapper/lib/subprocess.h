#include <boost/process/child.hpp>
#include <string.h>

class TSubProcess {
    public:
        TSubProcess(const std::string& init);
        void Stop();
        bool IsRunning();
        std::string GetResult() const;
        ~TSubProcess() = default;
    private:
        boost::process::child Child_;
};
