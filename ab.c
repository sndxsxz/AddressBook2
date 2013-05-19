/*
 * ab
 */

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>

/************************** global definition *****************************/

/* address entry data structure */
struct human{
    char name[30];
    char age[3];                  /* name of the person */  
    char mobile[12];                /* mobile number of the person */
    char address[40];               /* home address of the person */
    struct human *next;          /* pointer to next entry */
 };

struct doc{
	char name[20];
	struct human *str;
};

struct human *head, *last;       /* head & tail pointer of the address linklist */

struct human *lilei,*hanmeimei;

struct doc *entries;

int addr_num = 0;                   /* number of address entries */
struct human *addr_info[100];    /* search result */
char file_name[100];                /* file name record */
int getin = 2;
int getoff ,getoff1;
int keyword=1;
char command[30];

/***************************** socket  gobal definition*********************************/

#define DEST_IP                "192.168.1.106"
#define DEST_PORT               1234

int    socketfd;
struct sockaddr_in server;

/************************** Sub-function declaration *****************************/
int addr_command();                                            /* Command line */
void inputs(char *explain, char *valueAddr, int count);        /* Input function */
struct human *des_store(struct human *info);             /* Add a node to linklist */
void addr_add();                                               /* Add an address entry */
void ls_get(struct human *head);                            /* Display all the address entries */
int addr_find(int choice);                                     /* Find address entries */
void addr_remove();                                            /* Delete address entries by name|mobile|address */
void cat_get();     						/* Search address entries by name|mobile|address */	
void cd_get();                                       
void addr_help();                                              /* Help information */
void addr_display_a(struct human **info, int result);       /* Display node infomation */
int addr_strcmp(char * dest, char *src);                       /* Sub-string matching */
int addr_save(int argc);                                       /* Save to an ab file */
int addr_reload();                                             /* Reload an ab file */
int addr_load_file();                                          /* To edit an existed ab file ,load the file first */
void validate_arg(int argc, char **argv);                      /* Validate command line arguments */

void sock_init();
void socket_send();


/************************** Main function *****************************/
int main(int argc, char **argv){
	FILE *cf_ptr;
    head = last = NULL;

    
    validate_arg(argc, argv);
    if (argc == 3){
        if ((strcmp(&argv[2][0], "-new")) == 0){
        	if ((cf_ptr = fopen(&argv[1][3], "w+")) == NULL){
        		printf("Could not open file! First create Failed! \n");
        		return 1;
            }
            printf("First create OK!\n");
            strcpy(file_name, &argv[1][3]);
        }
    printf("The file path and name is: %s\n", &argv[1][3]);
    }
    if (argc == 2){
    	if ((cf_ptr = fopen(&argv[1][3], "r+")) == NULL){
        		printf("Could not open file! \n");
        		printf("The file which you assigned may be NOT exist.\n");
        		return 1;
            }
            printf("Open file: %s successfully!\n", &argv[1][3]);            
            strcpy(file_name, &argv[1][3]);
            addr_load_file();
    }
    printf("-------------------------------\n");
    printf("ab start\n");
    printf("-------------------------------\n");
    printf("\n");

    while (1){
        switch (addr_command()){
        case 1:    addr_add(); 
                   break;
        case 2:    ls_get(head); 
                   break;
        case 3:    addr_remove(); 
                   break;
        case 4:    cat_get(); 
                   break;
        case 5:    addr_reload(); 
                   break;
        case 6:    addr_save(argc); 
                   break;
        case 7:    addr_help(); 
                   break;
	case 8:    cd_get();
		   break;
        case 9:    sock_init();
		   break;
	case 10:   socket_send();
		   break;
	case 11:   return(0);
         
        }
   }
}

/* Command line */
int addr_command(){
    int choice;
    printf("ab>");  
    fgets(command, sizeof(command), stdin);  
    command[strlen(command) - 1] = '\0';
    
    while (strcmp(command, "add") && strcmp(command, "ls") && strcmp(command, "remove") && strncmp(command, "cat",3)
           && strcmp(command, "reload") && strcmp(command, "!help") && strncmp(command, "cd",2) && strcmp(command,"socket") && strcmp(command,"socksend") && strcmp(command, "!quit") && strcmp(command, "save")){
        printf("%s: COMMAND NOT FOUND\n", command);
        printf("Please type '!help' to get more help.\n");
        printf("ab>");
        fgets(command, sizeof(command), stdin);  
        command[strlen(command) - 1] = '\0';
    }

    if (!strcmp(command, "add")) choice = 1;
    if (!strcmp(command, "ls")) choice = 2;
    if (!strcmp(command, "remove")) choice = 3;
    if (!strncmp(command, "cat",3)) choice = 4;
    if (!strcmp(command, "reload")) choice = 5;
    if (!strcmp(command, "save")) choice = 6;
    if (!strcmp(command, "!help")) choice = 7;
    if (!strncmp(command, "cd",2)) choice = 8;	
    if (!strcmp(command, "socket")) choice = 9;
    if (!strcmp(command, "socksend")) choice = 10;	
    if (!strcmp(command, "!quit")) choice = 11;	
	
    return(choice);
}

void sock_init()
{
	int len = sizeof(server);
 	if ((socketfd = socket(AF_INET,SOCK_STREAM,0))==-1) 
 	{ 
       	    printf("socket failure"); 
            exit(1); 
        } 
	printf("sockfd is %d\n",socketfd);
        bzero(&server, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
 	server.sin_port=htons(DEST_PORT);       
    	inet_aton(DEST_IP, &server.sin_addr); 
   	connect(socketfd, (struct sockaddr *)&server, len) ;
	//send(socketfd,addr_info,sizeof(addr_info),0);
}

void socket_send()
{
	int sendbytes;
	sendbytes = send(socketfd,addr_info,sizeof(addr_info),0);
	printf("send success\n");
}


void lib_get()
{   if(getin == 2)
	{	
    lilei = (struct human *)malloc(sizeof(struct human));
    if (lilei == NULL){ 
        printf("\nOut of memory!\n");
        return;
    }
    hanmeimei = (struct human *)malloc(sizeof(struct human));
    if (lilei == NULL){ 
        printf("\nOut of memory!\n");
        return;
    }
  
    lilei = des_store(lilei);
    if (addr_num == 0) head = lilei; 
    addr_num++;
 
	sprintf(lilei->name,"%s","lilei");
	sprintf(lilei->age,"%s","27");
	sprintf(lilei->mobile,"%s","13700000000");
	sprintf(lilei->address,"%s","Earth somewhere");
   	//printf("value: { age: %s, mobile : %s, address : %s }\n",lilei->age,lilei->mobile,lilei->address);

    hanmeimei = des_store(hanmeimei);
    if (addr_num == 0) head = hanmeimei; 
    addr_num++;	
	sprintf(hanmeimei->name,"%s","hanmeimei");
	sprintf(hanmeimei->age,"%s","26");
	sprintf(hanmeimei->mobile,"%s","13700000001");
	sprintf(hanmeimei->address,"%s","Earth somewhere else");
	}
        //printf("value: { age: %s, mobile : %s, address : %s }\n",hanmeimei->age,hanmeimei->mobile,hanmeimei->address);
}

/* Add a node to linklist */
struct human *des_store(struct human *info){
    if (last){ 
        last->next = info;
    }
    info->next = NULL;
    last = info;
    return(info);
}

/* Add an address entry */
void addr_add(){
    struct human *info;
    info = (struct human *)malloc(sizeof(struct human));
    if (info == NULL){ 
        printf("\nOut of memory!\n");
        return;
    }
    inputs("key:", info->name, 15);
    inputs("age:", info->age, 3);
    inputs("mobile:", info->mobile, 12);
    inputs("address:", info->address, 40);    
    info = des_store(info);
    if (addr_num == 0) head = info; 
    
    addr_num++;
    printf("value: { age: %s, mobile : %s, address : %s }\n",info->age,info->mobile,info->address);
    printf("Address entry added.\n");
}

/* Input function */
void inputs(char *explain, char *valueAddr, int count){
    char valueStr[40];
    do{
        printf("%s", explain);
        fgets(valueStr, sizeof(valueStr), stdin);  
        valueStr[strlen(valueStr) - 1] = '\0';
        if (strlen(valueStr) >= count) printf("Too long!\n\n");
    }while (strlen(valueStr) >= count);
    strcpy(valueAddr, valueStr);
}

/* Display node infomation */
void addr_display_a(struct human **info, int result){
    int i;
    for (i = 0; i < result; i++){
        printf(" %s, { age: %s mobile: %s address: %s }\n",(*info)->name,(*info)->age, (*info)->mobile,(*info)->address);
        info++;
    }
}

/* Display all the address entries */
void ls_get(struct human *head){
    struct human *p;

    p = head;
    if(head != NULL){
        do{
	    if(getoff == 1)
		{
		printf("%s\n",lilei->name);
		}
	    if(getoff1 == 1)
		{
		printf("%s\n",hanmeimei->name);
		}

            printf("%s\n", p->name);
            p = p->next;
	    
        }while (p != NULL);
    }
     if(getin == 2)
        printf("ab>entries\n");
}

/* Find address entries */
int addr_find(int choice){
    struct human *info;
    info = head;
    char entry[30]; 
   int i;
    int result = 0;
     int n;
    n=strlen(command);

    switch (choice){
    case 1:  
    	     for(i=4;i<=n;i++)
{
		entry[i-4] = command[i];
} 

             if ((entry[n - 2] == '.') && (entry[n - 1] == '*')){            //use '.*' command to search
                 entry[n - 2] = '\0';
                 while (info){
                     if (!addr_strcmp(entry, info->name)){
                         addr_info[result++] = info;
                         info = info->next;
                     }
                     else info = info->next;
                 }
                 break;	
             }
      	     while (info){
                 if (!strcmp(entry, info->name)){
                     addr_info[result++] = info;
                     info = info->next;
                 }
                 else info = info->next;
             } break;
    case 2:  printf("mobile:");
      	     fgets(entry, sizeof(entry), stdin);  
             entry[strlen(entry) - 1] = '\0';
      	     n = strlen(entry);
             if ((entry[n - 2] == '.') && (entry[n - 1] == '*')){             //use '.*' command to search
                 entry[n - 2] = '\0';
                 while (info){
                     if (!addr_strcmp(entry, info->mobile)){
                         addr_info[result++] = info;
                         info = info->next;
                     }
                     else info = info->next;
                 }
                 break;	
             }    
      	     while (info){
                 if (!strcmp(entry, info->mobile)){ 
                     addr_info[result++] = info;
                     info = info->next;
                 }
                 else info = info->next;
             } break;
    case 3:  printf("address:");
      	     fgets(entry, sizeof(entry), stdin);  
             entry[strlen(entry) - 1] = '\0';
      	     n = strlen(entry);
             if ((entry[n - 2] == '.') && (entry[n - 1] == '*')){              //use '.*' command to search
                 entry[n - 2] = '\0';
                 while (info){
                     if (!addr_strcmp(entry, info->address)){
                         addr_info[result++] = info;
                         info = info->next;
                     }
                     else info = info->next;
                 }
                 break;	
      	     }
      	     while (info){
                 if (!strcmp(entry,info->address)){
                     addr_info[result++] = info;
                     info = info->next;
                 }
                 else info = info->next;
             } break;
    }       
    return(result);
}

/* Find address entries */
int addr_find1(int choice){
    struct human *info;
    info = head;
    char entry[30]; 
    size_t n;
    int result = 0;
    switch (choice){
    case 1:  printf("please give the key:");
    	     fgets(entry, sizeof(entry), stdin);  
             entry[strlen(entry) - 1] = '\0';
      	     n = strlen(entry);
	     printf("%s was deleted from JSON",entry);
             if ((entry[n - 2] == '.') && (entry[n - 1] == '*')){            //use '.*' command to search
                 entry[n - 2] = '\0';
                 while (info){
                     if (!addr_strcmp(entry, info->name)){
                         addr_info[result++] = info;
                         info = info->next;
                     }
                     else info = info->next;
                 }
                 break;	
             }
      	     while (info){
                 if (!strcmp(entry, info->name)){
                     addr_info[result++] = info;
                     info = info->next;
                 }
                 else info = info->next;
             } break;
    case 2:  printf("mobile:");
      	     fgets(entry, sizeof(entry), stdin);  
             entry[strlen(entry) - 1] = '\0';
      	     n = strlen(entry);
             if ((entry[n - 2] == '.') && (entry[n - 1] == '*')){             //use '.*' command to search
                 entry[n - 2] = '\0';
                 while (info){
                     if (!addr_strcmp(entry, info->mobile)){
                         addr_info[result++] = info;
                         info = info->next;
                     }
                     else info = info->next;
                 }
                 break;	
             }    
      	     while (info){
                 if (!strcmp(entry, info->mobile)){ 
                     addr_info[result++] = info;
                     info = info->next;
                 }
                 else info = info->next;
             } break;
    case 3:  printf("address:");
      	     fgets(entry, sizeof(entry), stdin);  
             entry[strlen(entry) - 1] = '\0';
      	     n = strlen(entry);
             if ((entry[n - 2] == '.') && (entry[n - 1] == '*')){              //use '.*' command to search
                 entry[n - 2] = '\0';
                 while (info){
                     if (!addr_strcmp(entry, info->address)){
                         addr_info[result++] = info;
                         info = info->next;
                     }
                     else info = info->next;
                 }
                 break;	
      	     }
      	     while (info){
                 if (!strcmp(entry,info->address)){
                     addr_info[result++] = info;
                     info = info->next;
                 }
                 else info = info->next;
             } break;
    }       
    return(result);
}


/* Search address entries by name|mobile|address */
void cat_get(){

    int result;
    
    if (!(result = addr_find(1))){
        printf("name cat not found!\n\n");
    }
    else
        addr_display_a(addr_info, result);
}


/* Delete address entries by name|mobile|address */
void addr_remove(){
    int result;
    struct human *p1, *p2;
    struct human **info_ptr = addr_info;
    if ((result  = addr_find1(1))){ 
    	while (result--){
            if (addr_num == 1){                                          /* if only 1 node */
                head = last = NULL;
                free(*info_ptr);
                *info_ptr = NULL;
            }
            else{
                if (*info_ptr == last){                                  /* to delete the tail node */
                    p2 = head;
                    p1 = head->next;
                    while(p1->next != NULL){ 
                        p2 = p1;
                        p1 = p1->next;
                    }
                    p2->next = NULL;
                    last = p2;
                    free(*info_ptr);
                    *info_ptr = NULL;
                    info_ptr++;
                }
                else if (head == *info_ptr){                             /* to delete the head node */
                    head = (*info_ptr)->next;
                    free(*info_ptr); 
                    *info_ptr = NULL;
                    info_ptr++;
                }
                else if ((*info_ptr != head) && (*info_ptr != last)){    /* to delete the intermediate node */
                    p2 = head; 
                    p1 = head->next; 
                    while (*info_ptr != p1){
                        p2 = p1;
                        p1 = p1->next;
                    }
                    p2->next = p1->next;
                    free(*info_ptr);
                    *info_ptr = NULL;
                    info_ptr++;
                
                }
            }
            addr_num--;
        }
          
    }
    else{ 
        printf("Address entry not found!\n");
    }
    printf("\n");
}

void cd_get(){

if(strstr(command , "entries") == 0)
{	
	printf("NO Such document\n");
        getin = 2;
	
}
else 
{
if(keyword == 1)
{
lib_get();
}
getin = 1;
keyword = 0;
}
}

/* Help information */
void addr_help(){
	printf("---------Help information--------\n");
	printf("The commands are defined internally.\n");
	printf("------To create or reload file-----\n");
    	printf(" 'cd'  to enter to a document \n");
    	printf(" 'ls'  to check the name of ab file. \n");
    	printf(" 'cat' to search the recent name of ab file. \n");
    	printf("------ Address entries operation -----\n");
    	printf(" Type 'add'    command to add new address entry.\n");
    	printf(" Type 'remove' command to remove one or more address entries.\n");
    	printf(" Type '!help'  command to get help.\n");
    	printf(" Type '!quit'  command to quit from the application.\n");
}   

/* Sub-string matching */
int addr_strcmp(char *dest, char *src){
    while (*dest != '\0'){
        if (*dest == *src){
            dest++;
            src++;
        }
        else 
            return 1;	
    }
    return 0;    
}

int addr_strcpy(char *dest, char *src){
    while (*dest != '\0'){
        *dest = *src+4;
            dest++;
            src++;	
    }
    return 0;    
}

/* Save to an ab file */
int addr_save(int argc){
    FILE *cf_ptr; 
    struct human *node_ptr;
    if (argc == 1){
        printf("file path: ");
        fgets(file_name, sizeof(file_name), stdin);  
        file_name[strlen(file_name) - 1] = '\0';
        cf_ptr = fopen(file_name, "w+");
    }
    cf_ptr = fopen(file_name, "w+");
    if (cf_ptr == NULL){
        printf("Could not open file! \n");
        printf("The file which you assigned may be NOT exist.\n");
        return 1;
    }
    printf("Now saving...\n");
    fwrite(&addr_num, sizeof(int), 1, cf_ptr);
    for (node_ptr = head; node_ptr != NULL; node_ptr = node_ptr->next){
        fwrite(node_ptr, sizeof(struct human), 1, cf_ptr);
    }
    printf("Successfully saved!\n");
    fclose(cf_ptr);
    return 0;
}

/* Reload an ab file */
int addr_reload(){
	FILE *cf_ptr; 
    struct human *node_ptr;
    int n = 0;
    printf("file path: ");
    fgets(file_name, sizeof(file_name), stdin);  
    file_name[strlen(file_name) - 1] = '\0';
    printf("Now reloading...\n");
    if ((cf_ptr = fopen(file_name, "r+")) == NULL){
        printf("Could not open file! \n");
        printf("The file which you assigned may be NOT exist.\n");
        return 1;
    }
    fread(&addr_num, sizeof(int), 1, cf_ptr);
    while (n < addr_num){
    	node_ptr = (struct human *)malloc(sizeof(struct human));
    	if (node_ptr == NULL){ 
            printf("\nOut of memory!\n");
            return 1;
        }  
        fread(node_ptr, sizeof(struct human), 1, cf_ptr);
        node_ptr = des_store(node_ptr);
        if (n == 0) head = node_ptr; 
        n++;
    }
    printf("Successfully reloaded!\n");
    fclose(cf_ptr);
    return 0;
}

/* To edit an existed ab file ,load the file first */
int addr_load_file(){
	FILE *cf_ptr; 
    struct human *node_ptr;
    int n = 0;   
    if ((cf_ptr = fopen(file_name, "r+")) == NULL){
        printf("Could not open file! \n");
        printf("The file which you assigned may be NOT exist.\n");
        return 1;
    }
    fread(&addr_num, sizeof(int), 1, cf_ptr);
    while (n < addr_num){
    	node_ptr = (struct human *)malloc(sizeof(struct human));
    	if (node_ptr == NULL){ 
            printf("\nOut of memory!\n");
            return 1;
        }  
        fread(node_ptr, sizeof(struct human), 1, cf_ptr);
        node_ptr = des_store(node_ptr);
        if (n == 0) head = node_ptr; 
        n++;
    }
    fclose(cf_ptr);
    return 0;
}

/* Validate command line arguments */
void validate_arg(int argc, char **argv){
    if ((argc != 1) && (argc != 2) && (argc != 3)){
    	printf("Invalid Arguments!\n");
    	printf("Please type '!help' to get more help.\n\n");
    	exit(1);
    }
    if (argc==3){ 
        if ((strcmp(&argv[2][0], "-new")) != 0){
            printf("If you want creat new file ,use '-new'.\n");
            printf("Please type '!help' to get more help.\n\n");
            exit(1);
        }
        else
            printf("Creat new file for the first use of this software.\n");

    }
    if (argc == 2){
        if ((argv[1][0] != '/') && (argv[1][0] != '-')){ 
        	printf("Invalid Arguments!\n");
        	printf("Argument must start with character '/' or '-'.\n");
        	printf("Please type '!help' to get more help.\n\n");
        	exit(1);
        }
        if (argv[1][1] != 'f'){
        	printf("Invalid Arguments!\n");
        	printf("the character assigned file path and name must be 'f'.\n");
        	printf("Please type '!help' to get more help.\n\n");
        	exit(1);
        }
        if (argv[1][2] != ':'){
        	printf("Invalid Arguments!\n");
        	printf("Between '-f' and [path][filename] must be ':'.\n");
        	printf("Please type '!help' to get more help.\n\n");
        	exit(1);
        }
    }
}
