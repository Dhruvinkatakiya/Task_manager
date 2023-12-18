#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
using namespace std;

class Task
{
public:
    int task_id;
    string title, description, deadline;
    int priority;
    vector<int> dependencies;

    Task(int id, const string &t, const string &desc, const string &ddl, int pri, const vector<int> &deps)
        : task_id(id), title(t), description(desc), deadline(ddl), priority(pri), dependencies(deps) {}

    void removeDependency(int dep_id)
    {
        dependencies.erase(remove(dependencies.begin(), dependencies.end(), dep_id), dependencies.end());
    }
};

class TaskManager
{
private:
    vector<Task> tasks;
    priority_queue<Task, vector<Task>, function<bool(const Task &, const Task &)>> priorityQueue;

    queue<int> taskQueue;

public:
    TaskManager() : priorityQueue([](const Task &a, const Task &b) { return a.priority > b.priority; }) {}

    void addTask(const Task &task)
    {
        tasks.push_back(task);
        priorityQueue.push(task);
    }

    Task markComplete()
    {
        if (!priorityQueue.empty())
        {
            Task completedTask = priorityQueue.top();
            priorityQueue.pop();
            processDependencies(completedTask);
            return completedTask;
        }
        else
        {
            return Task(-1, "", "", "", -1, {});
        }
    }

    void processDependencies(const Task &completedTask)
    {
        queue<int> temp;
            for (int dep_id : completedTask.dependencies)
            {
                temp.push(dep_id);
            }

            while (!temp.empty())
            {
                int dep_id = temp.front();
                temp.pop();

                auto dep_it = find_if(tasks.begin(), tasks.end(), [dep_id](const Task &t)
                                      { return t.task_id == dep_id; });

                if (dep_it != tasks.end())
                {
                    Task &dependencyTask = *dep_it;
                    dependencyTask.removeDependency(completedTask.task_id);

                    processDependencies(dependencyTask);
                    markComplete();
                }
            }
    }

    bool displayTasks() const
    {
        if (tasks.empty())
        {
            cout << "-----no have any task------\n"; 
            //return false;
        }
        else
        {
               cout << "\n----Your All Task----\n";
            for (const auto &task : tasks)
            {  
                cout << "Task ID: " << task.task_id << "\n";
                cout << "Title: " << task.title << "\n";
                cout << "Description: " << task.description << "\n";
                cout << "Deadline: " << task.deadline << "\n";
                cout << "Priority: " << task.priority << "\n";
                cout << "Dependencies: ";
                for (int dep_id : task.dependencies)
                {
                    cout << dep_id << " ";
                }
                cout << "\n\n";
            }

            return true;
        }
    }

    void displayPriorityQueue() const
    {
        priority_queue<Task, vector<Task>, function<bool(const Task &, const Task &)>> tempQueue = priorityQueue;
        
        if (tempQueue.empty())
        {
            cout << "\n-----no have any task------\n";
            
        }
        else{
            cout << "\n-----Task As Priority wise-----\n"; 
        while (!tempQueue.empty())
        {    
            const Task &task = tempQueue.top();
            cout << "Task ID: " << task.task_id << ", Title: " << task.title << " , Priority: " << task.priority << "\n";
            tempQueue.pop();
        }
        cout << "\n";
    }
    }
};

int main()
{
    TaskManager taskManager;
    int taskId, priority;
    string title, description, deadline;
    vector<int> dependencies;

    char choice;
    do
    {
        cout << "                                        \n";
        cout << "Choose Menu:\n";
        cout << "       1. Add Task\n";
        cout << "       2. Display Tasks\n";
        cout << "       3. Mark Task as Complete\n";
        cout << "       4. Display task As Priority Wise \n";
        cout << "       5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case '1':
            cout << "\n------Add Task Details------\n";
            cout <<"Enter Task ID: ";
            cin >> taskId;

            cout << "Enter Title: ";
            cin >> title;

            cout << "Enter Description: ";
            cin >> description;

            cout << "Enter Deadline: ";
            cin >> deadline;

            cout << "Enter Priority: ";
            cin >> priority;

            int dep;
            for (int i = 0;; i++)
            {
                cout << "Enter Dependencies : ";

                cin >> dep;

                if (dep == -1)
                {
                    break;
                }
                else
                {
                    dependencies.push_back(dep);
                }

            }

            taskManager.addTask(Task(taskId, title, description, deadline, priority, dependencies));

            dependencies.clear();
            break;

        case '2':
            taskManager.displayTasks();
            break;

        case '3':
        {
            Task completedTask = taskManager.markComplete();
            if (completedTask.task_id != -1)
            {
                cout << "\nTask marked as complete:\n";
                cout << "Task ID: " << completedTask.task_id << "\n";
                cout << "Title: " << completedTask.title << "\n\n";
            }
            else
            {
                cout << "\nPriority Queue is empty. No task to mark as complete.\n";
            }

            // Display remaining tasks
            //taskManager.displayTasks();
        }
        break;

        case '4':
            taskManager.displayPriorityQueue();
            break;

        case '5':
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
            break;
        }

    } while (choice != '5');

    return 0;
}
