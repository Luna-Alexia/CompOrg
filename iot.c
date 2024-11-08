#include <stdio.h>

#define MAX_STREAMS 100
typedef struct { 
int id;// ID 
int period;// period 
int last_update_time;// time of last update 
int pending_update;// 1 if pending update is in queue; else 0 
int age;// age 
int age_sum;// sum of ages from time 1 to current time
int sim_time;//added time for sim
} DataStream; 


void Ask(DataStream streams[], int *stream_num) {
    int i;

    printf("Enter number of data streams: "); // asking for the number of streams
    scanf("%d", stream_num);
    if (*stream_num < 1 || *stream_num > 100) {//checks if its negative or below 100
        printf("Please enter a number between 1 to 100.\n");
        return;
    }

    for (i = 0; i < *stream_num; i++) {// asks for when to update periods
        streams[i].id = i + 1; 

        do {// allows me to check condition with while
            printf("Enter period for stream: ");
            scanf("%d", &streams[i].period);
            
            if (streams[i].period < 1) {
                printf("Please enter a number greater than zero\n");// check if its 0 or negative
            }
        } 
        
        while (streams[i].period < 1); // repeat if the period is no good
        streams[i].age = 0; // initiate age to 0
        streams[i].age_sum = 0; // initiate sum to 0

    }

    printf("Enter simulation time: ");// asking for time
    scanf("%d", &streams[0].sim_time);

    if (streams[0].sim_time < 1) {// cannot have a negative time
        printf("Sim time must be greater than 0.\n");
        return;
    }
}

void Server(DataStream streams[], int stream_num) {
    int i;
    int x;
    int time = streams[0].sim_time; // get sim time
    time = time + 1; // adjust to include the last index 

    if (stream_num < 1 || stream_num > 100) { // makes sure periods a none negative and below 100
        return;
    }

    for (i = 1; i < time; i++) {
        printf("Time %d (stream: age)> ", i); //iterates via time

        int oldest_index = -1; // grabs the index of the oldest update
        int age_flag = -1; // keeps tracks of oldest due for an update
        
        for (x = 0; x < stream_num; x++) {// inner loop to check which streams are due
            if (i % streams[x].period == 0) {// checks if the current time is a multiplied of stream's period (by time mod period)
                if (streams[x].age > age_flag) {// compares the two ages with currently grabbed age and flagged age
                    age_flag = streams[x].age; // swaps the flag with the next old age
                    oldest_index = x; // marks the stream as the most outdated current stream
                }
            }
        }

        for (x = 0; x < stream_num; x++) {//goes through the streams
            if (x == oldest_index) {// checks if this is the oldest
                printf("%d: %d*", streams[x].id, streams[x].age + 1); // print out stream and update q
            } 
            else {
                printf("%d: %d ", streams[x].id, streams[x].age + 1); // prints out streams and age
            }

            if (x < stream_num - 1) {
                printf("| ");
            }
        }

        if (oldest_index != -1) {// if this stream is the oldest, print with *
            streams[oldest_index].age = 0; // reset the age of the oldest stream
        }


        for (x = 0; x < stream_num; x++) {
            if (x != oldest_index) {
                streams[x].age += 1; // increments all ages by one if not selected
                streams[x].age_sum += streams[x].age; // updates sum

                //printf("[%d AGE %d]", streams[x].id, streams[x].age_sum);

            }
        }

        if (oldest_index != -1) {// prints "->" next to the selected (oldest) stream
            printf(" -> stream %d selected\n", streams[oldest_index].id);
        } 
        else {
            printf("\n"); 
        }
    }


    double total = 0.0; //intialize double var
    for (x = 0; x < stream_num; x++){
        double avg_age = (double)streams[x].age_sum / time;//finding avearge for each stream
        //printf(" AVERAGE %.2f", avg_age);
        total += avg_age;
    }
    printf("Total average Age of Information at the end of the simulation: %.2f\n", total);

}

int main() {
    DataStream streams[MAX_STREAMS];
    int stream_num; // number of streams

    Ask(streams, &stream_num);
    Server(streams, stream_num);

    return 0;
}
