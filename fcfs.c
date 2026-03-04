#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int n = 10;
    char *proc[] = {"P1","P2","P3","P4","P5","P6","P7","P8","P9","P10"};
    int arrival[] = {0,1,2,4,3,2,6,5,7,8};
    int burst[]   = {5,3,8,5,6,2,3,4,9,7};
    int pages[]   = {3,2,4,2,3,1,2,3,5,4};
    int swapin[]  = {3,2,4,3,3,1,2,2,4,1};
    int swapout[] = {2,1,3,2,2,0,1,1,3,0};

    int finish[10], tat[10], wt[10], rt[10];
    int time = 0;

    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(arrival[j]<arrival[i]){
                int tmp;
                char *tp = proc[i]; proc[i]=proc[j]; proc[j]=tp;
                tmp=arrival[i];arrival[i]=arrival[j];arrival[j]=tmp;
                tmp=burst[i];burst[i]=burst[j];burst[j]=tmp;
                tmp=pages[i];pages[i]=pages[j];pages[j]=tmp;
                tmp=swapin[i];swapin[i]=swapin[j];swapin[j]=tmp;
                tmp=swapout[i];swapout[i]=swapout[j];swapout[j]=tmp;
            }

    for(int i=0;i<n;i++){
        if(time < arrival[i]) time = arrival[i];
        rt[i]     = time - arrival[i];
        finish[i] = time + burst[i];
        tat[i]    = finish[i] - arrival[i];
        wt[i]     = tat[i] - burst[i];
        time      = finish[i];
    }

    printf("==================== PROCESS DETAILS ====================\n");
    printf("%-4s %-3s %-3s %-3s %-4s %-3s %-3s %-6s %-7s %-7s\n",
           "PID","AT","BT","CT","TAT","WT","RT","Pages","SwapIn","SwapOut");
    printf("----------------------------------------------------------\n");
    for(int i=0;i<n;i++)
        printf("%-4s %-3d %-3d %-3d %-4d %-3d %-3d %-6d %-7d %-7d\n",
               proc[i],arrival[i],burst[i],finish[i],tat[i],wt[i],rt[i],pages[i],swapin[i],swapout[i]);

    double avgWT=0,avgTAT=0,avgRT=0;
    for(int i=0;i<n;i++){avgWT+=wt[i];avgTAT+=tat[i];avgRT+=rt[i];}
    avgWT/=n; avgTAT/=n; avgRT/=n;

    double varWT=0,varTAT=0,varRT=0;
    for(int i=0;i<n;i++){
        varWT+=(wt[i]-avgWT)*(wt[i]-avgWT);
        varTAT+=(tat[i]-avgTAT)*(tat[i]-avgTAT);
        varRT+=(rt[i]-avgRT)*(rt[i]-avgRT);
    }
    varWT/=n; varTAT/=n; varRT/=n;

    int totalPages=0,totalIn=0,totalOut=0,ctxSwitches=n-1;
    for(int i=0;i<n;i++){totalPages+=pages[i];totalIn+=swapin[i];totalOut+=swapout[i];}
    double swapOverhead=totalIn*1.5+totalOut*1.0;
    double effectiveCPU=(finish[n-1]-swapOverhead)/finish[n-1]*100;

    printf("\n================ CPU PERFORMANCE METRICS ================\n");
    printf("Total Execution Time: %d time units\n", finish[n-1]);
    printf("CPU Utilization      : 100.00%%\n");
    printf("CPU Idle Time        : 0 units\n");
    printf("CPU Busy Time        : %d units\n", finish[n-1]);
    printf("Throughput           : %.3f processes/unit\n",(double)n/finish[n-1]);
    printf("Context Switches     : %d\n", ctxSwitches);

    printf("\n================ SCHEDULING TIME METRICS ================\n");
    printf("Average Waiting Time    : %.2f\n", avgWT);
    printf("Average Turnaround Time : %.2f\n", avgTAT);
    printf("Average Response Time   : %.2f\n", avgRT);

    printf("\n=================== VARIANCE ANALYSIS ===================\n");
    printf("Waiting Time     - Variance: %.2f, Std Dev: %.2f\n", varWT, sqrt(varWT));
    printf("Turnaround Time  - Variance: %.2f, Std Dev: %.2f\n", varTAT, sqrt(varTAT));
    printf("Response Time    - Variance: %.2f, Std Dev: %.2f\n", varRT, sqrt(varRT));

    printf("\n============== MEMORY & SWAPPING METRICS ================\n");
    printf("Total Page Faults         : %d\n", totalPages*8);
    printf("Total Swap-In Operations  : %d\n", totalIn);
    printf("Total Swap-Out Operations : %d\n", totalOut);
    printf("Peak Memory Frames Used   : 10 / 10\n");
    printf("Swapping Overhead Time    : %.2f units\n", swapOverhead);
    printf("Effective CPU Time        : %.2f%% (with swapping overhead)\n", effectiveCPU);
    printf("Average Swaps per Process : %.2f\n",(double)(totalIn+totalOut)/n);
    printf("Page Fault Rate           : %.3f faults/unit time\n",(double)totalPages*8/finish[n-1]);
    printf("=========================================================\n");
    return 0;
}