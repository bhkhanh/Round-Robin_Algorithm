/*###############################################
# University of Information Technology          #
# Operating System                              #
# File: RoundRobin.c                            #
# Round Robin algorithm                         #
###############################################*/
//assuming that all arrival time are 0
//DONE

#include <stdio.h>
#include <stdbool.h>

struct ProcessControlBlock
{
    int processName, burstTime;
    int arrivalTime;
    int waitingTime, turnaroundTime;
    int startTime, stopTime;
};

//for users to input information about PCB (process order, arrival time=0 & burst time)
void Input_PCB(int numberProcess, struct ProcessControlBlock pcb[])
{
    for (int i = 0; i < numberProcess; i++)
    {
        printf("Enter the Process Name: ");
        scanf("%d", &pcb[i].processName);

        printf("Enter the Burst Time: ");
        scanf("%d", &pcb[i].burstTime);

        printf("Arrival Time: 0");
        pcb[i].arrivalTime = 0;

        printf("\n\n");
    }
}

void Calculating_WaitingTime(int numberProcess, int quantumTime, struct ProcessControlBlock pcb[])
{
    int remainingBurstTime[numberProcess];
    int finish[numberProcess];

    for (int i = 0; i < numberProcess; i++)
    {
        remainingBurstTime[i] = pcb[i].burstTime;
    }

    int currentTime = 0;

    while (1)
    {
        bool done = true;

        for (int i = 0; i < numberProcess; i++)
        {
            if (i == 0)
                pcb[0].startTime = 0;
            else
            {
                if (remainingBurstTime[i-1] == 0)
                    pcb[i].startTime = finish[i-1];
            }


            if (remainingBurstTime[i] > 0)
            {
                done = false;
                if (remainingBurstTime[i] > quantumTime)
                {
                    currentTime += quantumTime;

                    finish[i] = currentTime;

                    remainingBurstTime[i] -= quantumTime;
                }
                else
                {
                    currentTime = currentTime + remainingBurstTime[i];

                    finish[i] = currentTime;

                    pcb[i].waitingTime = currentTime - pcb[i].burstTime;
                    remainingBurstTime[i] = 0;
                }
            }
        }
        if (done == true)
            break;
    }
}

//calculate Turnaround Time of all processes
void Calculating_TurnaroundTime(int numberProcess, struct ProcessControlBlock pcb[])
{
    for (int i = 0; i < numberProcess; i++)
    {
        pcb[i].turnaroundTime = pcb[i].burstTime + pcb[i].waitingTime;
    }
}

void Output_PCB(int numberProcess, struct ProcessControlBlock pcb[], int quantumTime)
{
    printf("Quantum Time is %d\n\n", quantumTime);
    printf("[*] PCB Before Sorting & Calculating [*] \n");
    printf("-------------------------------------------- \n");
    printf("\t\t ROUND ROBIN \n");
    printf("-------------------------------------------- \n");
    printf("Process Name \t Arrival Time \t Burst Time \n");
    for (int i = 0; i < numberProcess; i++)
    {
        printf("%d \t\t %d \t\t %d", pcb[i].processName, pcb[i].arrivalTime, pcb[i].burstTime);
        printf("\n");
    }
    printf("-------------------------------------------- \n");

    //=================================================================================================
    Calculating_WaitingTime(numberProcess, quantumTime, pcb);
    Calculating_TurnaroundTime(numberProcess, pcb);

    printf("\n[*] PCB After Sorting & Calculating [*] \n");
    printf("------------------------------------------------------------------------------------ \n");
    printf("\t\t\t\t ROUND ROBIN \n");
    printf("------------------------------------------------------------------------------------ \n");
    printf("Process Name \t Arrival Time \t Burst Time \t Waiting Time \t Turnaround Time \n");
    for (int i = 0; i < numberProcess; i++)
    {
        printf("%d \t\t %d \t\t %d \t\t %d \t\t %d", pcb[i].processName, pcb[i].arrivalTime, pcb[i].burstTime, pcb[i].waitingTime, pcb[i].turnaroundTime);
        printf("\n");
    }
    printf("------------------------------------------------------------------------------------ \n");
    printf("\n");
}

//This function is to print the line '--'
void printTheLine(int quantumTime, int remainingTime[], int currentIndex)
{
    if (remainingTime[currentIndex] >= quantumTime)
    {
        for (int i = 0; i < 3; i++)
        {
            printf("--");
        }
        printf(" ");
    }
    else
    {
        for (int i = 0; i < remainingTime[currentIndex]; i++)
        {
            printf("--");
        }
        printf(" ");
    }
}

//print the top or bottom bar because top and bottom line are the same
void printTheTopBottomBar(int numberProcess, struct ProcessControlBlock pcb[], int quantumTime)
{
    int remainProcess = numberProcess;
    int remainingTime[numberProcess];
    int i, j, currentTime, flag=0;

    for(i=0; i<numberProcess; i++)
    {
        remainingTime[i] = pcb[i].burstTime;
    }

    printf(" ");
    currentTime = 0;
    for(i=0; remainProcess !=0;)
    {
        if (remainingTime[i] <= quantumTime && remainingTime[i] > 0)
        {
            currentTime = currentTime + remainingTime[i];

            printTheLine(quantumTime, remainingTime, i);

            remainingTime[i] = 0;
            flag = 1;
        }
        else
        {
            if (remainingTime[i] > 0)
            {
                currentTime = currentTime + quantumTime;

                printTheLine(quantumTime, remainingTime, i);

                remainingTime[i] = remainingTime[i] - quantumTime;
            }
        }
        if (remainingTime[i] == 0 && flag == 1)
        {
            remainProcess--;
            flag = 0;
        }
        if (i == numberProcess-1)
            i = 0;
        else if (pcb[i+1].arrivalTime <= currentTime)
            i++;
        else
            i = 0;
    }
}

//print the middle which is the process name
void print_Middle_Of_Bar(int numberProcess, struct ProcessControlBlock pcb[], int quantumTime)
{
    int remainProcess = numberProcess;
    int remainingTime[numberProcess];
    int i, j, currentTime, flag=0;

    for(i=0; i<numberProcess; i++)
    {
        remainingTime[i] = pcb[i].burstTime;
    }

    currentTime = 0;
    for(i=0; remainProcess !=0;)
    {
        if (remainingTime[i] <= quantumTime && remainingTime[i] > 0)
        {
            currentTime = currentTime + remainingTime[i];

            if (remainingTime[i] >= quantumTime)
            {
                for (int j = 0; j < 2; j++)
                    printf(" ");
                printf("P%d", pcb[i].processName);
                for (int j = 0; j < 2; j++)
                    printf(" ");
                printf("|");
            }
            else
            {
                for (int j = 0; j < remainingTime[i]-1; j++)
                    printf(" ");
                printf("P%d", pcb[i].processName);
                for (int j = 0; j < remainingTime[i]-1; j++)
                    printf(" ");
                printf("|");
            }

            remainingTime[i] = 0;
            flag = 1;
        }
        else
        {
            if (remainingTime[i] > 0)
            {
                currentTime = currentTime + quantumTime;

                if (remainingTime[i] >= quantumTime)
                {
                    for (int j = 0; j < 2; j++)
                        printf(" ");
                    printf("P%d", pcb[i].processName);
                    for (int j = 0; j < 2; j++)
                        printf(" ");
                    printf("|");
                }
                else
                {
                    for (int j = 0; j < remainingTime[i]-1; j++)
                        printf(" ");
                    printf("P%d", pcb[i].processName);
                    for (int j = 0; j < remainingTime[i]-1; j++)
                        printf(" ");
                    printf("|");
                }

                remainingTime[i] = remainingTime[i] - quantumTime;
            }
        }
        if (remainingTime[i] == 0 && flag == 1)
        {
            remainProcess--;
            flag = 0;
        }
        if (i == numberProcess-1)
            i = 0;
        else if (pcb[i+1].arrivalTime <= currentTime)
            i++;
        else
            i = 0;
    }
}

//print the time line at the end (start time or stop time of each process)
void print_TheTimeLine(int numberProcess, struct ProcessControlBlock pcb[], int quantumTime)
{
    int remainProcess = numberProcess;
    int remainingTime[numberProcess];
    int i, j, currentTime, flag=0;

    for(i=0; i<numberProcess; i++)
    {
        remainingTime[i] = pcb[i].burstTime;
    }

    currentTime = 0;
    printf("0");
    for(i=0; remainProcess !=0;)
    {
        if (remainingTime[i] <= quantumTime && remainingTime[i] > 0)
        {
            currentTime = currentTime + remainingTime[i];

            if (remainingTime[i] >= quantumTime)
            {
                for (int j = 0; j < 3; j++)
                    printf("  ");
                if (currentTime > 9)
                    printf("\b");
                printf("%d", currentTime);
            }
            else
            {
                for (int j = 0; j < remainingTime[i]; j++)
                    printf("  ");
                if (currentTime > 9)
                    printf("\b");
                printf("%d", currentTime);
            }

            remainingTime[i] = 0;
            flag = 1;
        }
        else
        {
            if (remainingTime[i] > 0)
            {
                currentTime = currentTime + quantumTime;

                if (remainingTime[i] >= quantumTime)
                {
                    for (int j = 0; j < 3; j++)
                        printf("  ");
                    if (currentTime > 9)
                        printf("\b");
                    printf("%d", currentTime);
                }
                else
                {
                    for (int j = 0; j < remainingTime[i]; j++)
                        printf("  ");
                    if (currentTime > 9)
                        printf("\b");
                    printf("%d", currentTime);
                }

                remainingTime[i] = remainingTime[i] - quantumTime;
            }
        }
        if (remainingTime[i] == 0 && flag == 1)
        {
            remainProcess--;
            flag = 0;
        }
        if (i == numberProcess-1)
            i = 0;
        else if (pcb[i+1].arrivalTime <= currentTime)
            i++;
        else
            i = 0;
    }
}

//combine all the functions which are printTopBottomBar, print_Middle_Of_Bar and print_TheTimeLine in here
void printGanttChart(int numberProcess, struct ProcessControlBlock pcb[], int quantumTime)
{
    printTheTopBottomBar(numberProcess, pcb, quantumTime);
    printf("\n|");
    print_Middle_Of_Bar(numberProcess, pcb, quantumTime);
    printf("\n");
    printTheTopBottomBar(numberProcess, pcb, quantumTime);
    printf("\n");
    print_TheTimeLine(numberProcess, pcb, quantumTime);
}

//calculate average of waiting time or turnaround time (depends on 'which' variable)
double Calculating_Average(int numberProcess, struct ProcessControlBlock pcb[], int which)
{
    //which = 1 will calculate average waiting time
    //which = 2 will calculate average turnaround time
    int sum;
    switch (which)
    {
    case 1: //average waiting time
        sum = 0;
        for (int i = 0; i < numberProcess; i++)
        {
            sum += pcb[i].waitingTime;
        }
        return (float)sum / (float)numberProcess;
        break;
    
    case 2: //average turnaround time
        sum = 0;
        for (int i = 0; i < numberProcess; i++)
        {
            sum += pcb[i].turnaroundTime;
        }
        return (float)sum / (float)numberProcess;
        break;

    default:
        return 0;
        break;
    }
}

void main()
{
    int numberProcess, quantumTime;
    printf("\nPlease enter total number of processes that you want: ");
    scanf("%d", &numberProcess);
    printf("Please enter the Quantum Time: ");
    scanf("%d", &quantumTime);
    printf("Assumes that all processes have arrival time is 0 \n");
    printf("\n");

    struct ProcessControlBlock pcb[numberProcess];

    Input_PCB(numberProcess, pcb);
    printf("\n");
    Output_PCB(numberProcess, pcb, quantumTime);
    printf("\n");

    printf("Average of all Waiting Time is %.2lf \n", Calculating_Average(numberProcess, pcb, 1));
    printf("Average of all Turnaround Time is %.2lf \n\n", Calculating_Average(numberProcess, pcb, 2));

    printf("[*] Gantt Chart [*] \n");
    printGanttChart(numberProcess, pcb, quantumTime);
    printf("\n\n");
}