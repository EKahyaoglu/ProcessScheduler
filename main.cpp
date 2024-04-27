// Project Name: Process Scheduler
// Author: Eren Kahyaoglu

/* DOCUMENTATION: The purpose of this program is to create Gantt charts for process scheduling on the algorithms of First-Come First Served (FCFS), Shortest Time Remaining First (STRF), and Round Robin (RR). The user will enter their full name, alongside how many processes they would like to put in the ready queue (maximum of 10). The program will then randomly generate values for the burst time (BT) and arrival time (AT) using a random number generator, creating Gantt charts for each of the aforementioned algorithms. Additionally, the program will also calculate the average waiting time for each of the algorithms. */

/* Some abbreviations used in the program:
BT --> Burst Time
AT --> Arrival Time
TAT --> Turnaround Time
WT --> Waiting Time
CT --> Completion Time */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cctype>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>

// Declaring variables using a structure
struct Process
{
  int id = 0;  
  double BT = 0.0;
  int AT = 0;
  int CT = 0;
  double TAT = CT - AT;
  double WT = TAT - BT;
  double average_WT = 0.0;
  double average_TAT = 0.0;
};

std::string user_name = "";

// Function to generate random values for BT and AT
void randomizer(std::vector<Process>& processes)
{
  srand(time(nullptr));

  for(auto& process : processes)
  {
    process.AT = 1 + (rand() % processes.size());
    process.BT = 1 + (rand() % 10);
  }
}

// Function to sort processes based on AT
bool sortByAT(const Process& a, const Process& b) 
{ return a.AT < b.AT; }

/* FIRST COME FIRST SERVED */
void generateGanttChartFCFS(std::vector<Process>& processes, int process_count)
{
    // Calculate CT, TAT, and WT for each process
    int total_WT = 0;
    int total_TAT = 0;
    int current_time = 0;

    for(int i = 0; i < process_count; ++i)
    {
      processes[i].id = i;
    }

    for(auto& process : processes)
    {
        process.CT = current_time + process.BT;
        process.TAT = process.CT - process.AT;
        process.WT = std::max(0, current_time - process.AT);

        total_WT += process.WT;
        total_TAT += process.TAT;
        current_time = process.CT;
    }

    // Calculate average WT and average TAT
    double average_WT = static_cast<double>(total_WT) / process_count;
    double average_TAT = static_cast<double>(total_TAT) / process_count;

    // Print Gantt chart
    std::cout << "\nGantt Chart (FCFS):" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "|";

    for(auto& process : processes)
    {
        std::cout << "P" << process.id << " |";
    }
  
    std::cout << std::endl;
    std::cout << "0";
    current_time = 0;

    for(auto& process : processes)
    {
        current_time += process.BT;
        std::cout << std::setw(4) << current_time;
    }
    std::cout << std::endl;

    // Print process details
    std::cout << std::endl;
    std::cout << "Process Details: " << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Process ID\tArrival Time\tBurst Time\t\tCompletion Time\t\tTurnaround Time\t\tWaiting Time" << std::endl;

    for(auto& process : processes)
    {
        std::cout << process.id << "\t\t\t" << process.AT << "\t\t\t\t" << process.BT << "\t\t\t\t" << process.CT << "\t\t\t\t\t" << process.TAT << "\t\t\t\t\t" << process.WT << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Average Waiting Time: " << average_WT << std::endl;
    std::cout << "Average Turnaround Time: " << average_TAT << std::endl;
}

/* SHORTEST TIME REMAINING FIRST */
void generateGanttChartSTRF(std::vector<Process>& processes, int process_count)
{
    // Calculate CT, TAT, and WT for each process
    int total_WT = 0;
    int total_TAT = 0;
    int current_time = 0;

    // Create a vector to store remaining burst times
    std::vector<int> remaining_BT(process_count, 0);

    // Initialize remaining burst times
    for (int i = 0; i < process_count; ++i) {
        remaining_BT[i] = processes[i].BT;
    }

    int completed_processes = 0;
    while (completed_processes < process_count) {
        int min_BT = std::numeric_limits<int>::max();
        int shortest_PI = -1;

        // Find the process with the shortest remaining burst time
        for (int i = 0; i < process_count; ++i) {
            if (processes[i].AT <= current_time && remaining_BT[i] < min_BT && remaining_BT[i] > 0) {
                min_BT = remaining_BT[i];
                shortest_PI = i;
            }
        }

        if (shortest_PI == -1) {
            // No process is ready to execute, move to the next arrival time
            int next_AT = std::numeric_limits<int>::max();
            for (int i = 0; i < process_count; ++i) {
                if (processes[i].AT > current_time && processes[i].AT < next_AT) {
                    next_AT = processes[i].AT;
                }
            }
            current_time = next_AT;
        } else {
            // Execute the shortest process
            processes[shortest_PI].CT = current_time + min_BT;
            processes[shortest_PI].TAT = processes[shortest_PI].CT - processes[shortest_PI].AT;
            processes[shortest_PI].WT = processes[shortest_PI].TAT - processes[shortest_PI].BT;

            total_WT += processes[shortest_PI].WT;
            total_TAT += processes[shortest_PI].TAT;
            current_time += min_BT;
            remaining_BT[shortest_PI] = 0;
            ++completed_processes;
        }
    }

    // Calculate average WT and average TAT
    double average_WT = static_cast<double>(total_WT) / process_count;
    double average_TAT = static_cast<double>(total_TAT) / process_count;

    // Print Gantt chart
    std::cout << "Gantt Chart (STRF):" << std::endl;
    std::cout << "-------------------" << std::endl;
    std::cout << "|";

    for(auto& process : processes)
    {
        std::cout << "P" << process.id << " |";
    }

    std::cout << std::endl;
    std::cout << "0";
    current_time = 0;

    for(auto& process : processes)
    {
      std::cout << std::setw(4) << process.CT;
    }
    std::cout << std::endl;

    // Outputting the Gantt Chart
    std::cout << std::endl;
    std::cout << "Process Details: " << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Process ID\tArrival Time\tBurst Time\t\tCompletion Time\t\tTurnaround Time\t\tWaiting Time" << std::endl;
  
    for(auto& process : processes)
    {
        std::cout << process.id << "\t\t\t" << process.AT << "\t\t\t\t" << process.BT << "\t\t\t\t" << process.CT << "\t\t\t\t\t" << process.TAT << "\t\t\t\t\t" << process.WT << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Average Waiting Time: " << average_WT << std::endl;
    std::cout << "Average Turnaround Time: " << average_TAT << std::endl;
}


/* ROUND ROBIN */
void generateGanttChartRR(std::vector<Process>& processes, int process_count)
{
    const int quantum_time = 2; // Quantum time for Round Robin

    // Calculate CT, TAT, and WT for each process
    int total_WT = 0;
    int total_TAT = 0;
    int current_time = 0;

    // Create a vector to store remaining burst times
    std::vector<int> remaining_BT(process_count, 0);

    // Initialize remaining burst times
    for (int i = 0; i < process_count; ++i) {
        remaining_BT[i] = processes[i].BT;
    }

    int completed_processes = 0;
    while (completed_processes < process_count) {
        bool all_processes_completed = true;

        // Traverse all processes
        for (int i = 0; i < process_count; ++i) {
            if (processes[i].AT <= current_time && remaining_BT[i] > 0) {
                all_processes_completed = false;

                // Execute the process for quantum time or remaining burst time, whichever is smaller
                int execution_time = std::min(quantum_time, remaining_BT[i]);
                current_time += execution_time;
                remaining_BT[i] -= execution_time;

                // If the process is completed, update completion time and calculate TAT and WT
                if (remaining_BT[i] == 0) {
                    processes[i].CT = current_time;
                    processes[i].TAT = processes[i].CT - processes[i].AT;
                    processes[i].WT = processes[i].TAT - processes[i].BT;

                    total_WT += processes[i].WT;
                    total_TAT += processes[i].TAT;
                    ++completed_processes;
                }
            }
        }

        // If no process is ready to execute, move to the next arrival time
        if (all_processes_completed) {
            int next_AT = std::numeric_limits<int>::max();
            for (int i = 0; i < process_count; ++i) {
                if (remaining_BT[i] > 0 && processes[i].AT < next_AT) {
                    next_AT = processes[i].AT;
                }
            }
            current_time = next_AT;
        }
    }

    // Calculate average WT and average TAT
    double average_WT = static_cast<double>(total_WT) / process_count;
    double average_TAT = static_cast<double>(total_TAT) / process_count;

    // Print Gantt chart
    std::cout << "Gantt Chart (Round Robin, Quantum Time = 2):" << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << "|";

  for(auto& process : processes)
  {
      std::cout << "P" << process.id << " |";
  }

    std::cout << std::endl;
    std::cout << "0";
    current_time = 0;

    for(auto& process : processes)
    {
        std::cout << std::setw(4) << process.CT;
    }
    std::cout << std::endl;

    // Print process details
    std::cout << std::endl;
    std::cout << "Process Details: " << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << "Process ID\tArrival Time\tBurst Time\t\tCompletion Time\t\tTurnaround Time\t\tWaiting Time" << std::endl;
  
    for(auto& process : processes)
    {
        std::cout << process.id << "\t\t\t" << process.AT << "\t\t\t\t" << process.BT << "\t\t\t\t" << process.CT << "\t\t\t\t\t" << process.TAT << "\t\t\t\t\t" << process.WT << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Average Waiting Time: " << average_WT << std::endl;
    std::cout << "Average Turnaround Time: " << average_TAT << std::endl;
}

/* MAIN FUNCTION */
int main() 
{
    std::cout << "Welcome to the Process Scheduler program!" << std::endl;
  
    while (true)
    {
      std::cout << "Please enter your name to continue: " << std::endl;
      std::getline(std::cin, user_name);
  
      if (!user_name.empty()) 
      {
        bool valid_name = true;
        for (char c : user_name) 
        {
          if (!std::isalpha(c) && c != ' ') 
          {
            valid_name = false;
            std::cout << "Invalid name." << std::endl;
            break;
          }
        }
            if (valid_name) break;
        } else {
            std::cout << "Please enter a valid name." << std::endl;
        }
      }
  
  int process_count = 0;
  
  std::cout << "Hello, " << user_name << "!\nHow many processes would you like to enter? Enter a number between 1 and 10." << std::endl;
  std::cin >> process_count;
  
    while (process_count <= 0 || process_count > 10)
    {
      std::cout << "Invalid input. Please enter a number between 1 and 10." << std::endl;
      std::cin >> process_count;
    }
  
  // Create vector to hold processess and generate random BT and AT for each process
  std::vector<Process> processes(process_count);
  randomizer(processes);
  
  // Sort processes based on their AT
  std::sort(processes.begin(), processes.end(), sortByAT);
  
  /* // Calculate CT, TAT, and WT for each process
  int total_WT = 0;
  int total_TAT = 0;
  int current_time = 0; */
  
  // Call functions to generate Gantt charts
  generateGanttChartFCFS(processes, process_count);
  std::cout << std::endl;
  std::cout << std::endl;
  generateGanttChartSTRF(processes, process_count);
  std::cout << std::endl;
  std::cout << std::endl;
  generateGanttChartRR(processes, process_count);

  return 0;
}
