#include <functional>

class TaskController;
typedef std::function<void(TaskController*)> TaskJobFunction;

typedef void PureJobFunction();