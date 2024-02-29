#include <stdio.h>
#include <stdlib.h>


typedef struct Graph * graph;

struct Graph{           //struct to the graph
    char From[20];
    char To[20];
    int distance;
    int cost;
    int visited;
    graph parent;
    graph next;
    graph nextL;
};

int size = 0;

void swap(graph *a, graph *b){ //swap in heap
  graph temp = *b;
  *b = *a;
  *a = temp;
}

void heapify(graph array[], int size, int i){  //heapify to put the lowest distance in the top of tree
  if (size == 1)
  {
    printf("Single element in the heap");
  }
  else
  {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && array[l]->distance < array[largest]->distance)
      largest = l;
    if (r < size && array[r]->distance < array[largest]->distance)
      largest = r;
    if (largest != i)
    {
      swap(&array[i], &array[largest]);
      heapify(array, size, largest);
    }
  }
}

void insert(graph array[], graph node){  //insert to the heap
  if (size == 0)
  {
    array[0] = node;
    size += 1;
  }
  else
  {
    array[size] = node;
    size += 1;
    for (int i = size / 2 - 1; i >= 0; i--)
    {
      heapify(array, size, i);
    }
  }
}

void deleteRoot(graph array[], graph node){   //delete node from heap and heapify
  int i;
  for (i = 0; i < size; i++)
  {
    if (strcmp(node->From,array[i]->From) == 0 && strcmp(node->To,array[i]->To) == 0 )

      break;
  }
    //printf("%s %s %d\n",array[i]->From,array[i]->To,i);
  swap(&array[i], &array[size - 1]);
  size -= 1;
  for (int i = size / 2 - 1; i >= 0; i--)
  {
    heapify(array, size, i);
  }
}

void read_file_and_insert(graph head,graph heap[100]){                        //function to read bus file
    FILE *f;
    f = fopen("cities.txt","r");
    char *token;
    char line[200];
    int j=0;
    while(fgets(line,sizeof(line),f)){     //get the lines line by line

        int i=0;
        token = strtok(line," ");  //cut the line to words in all
        char take[3][20];
        while(token != NULL){
            strcpy(take[i],token);     //put the tokens in array
            token = strtok(NULL," ");
            i++;
            }
        graph node = malloc(sizeof(struct Graph));
        strcpy(node->From,take[0]);
        strcpy(node->To,take[1]);
        int num1 = atoi(take[2]);
        node->distance = num1;
        node->next = NULL;
        node->nextL = NULL;
        if(head->nextL == NULL){
            head->nextL = node;
        }
        else{
            graph temp = head;
            while(temp->nextL != NULL){
                temp = temp->nextL;
                if(strcmp(temp->From,node->From) == 0){      //creat the graph and insert node in it
                   graph tempTwo = temp;
                    if(temp->next == NULL){

                    temp->next = node;
                    break;
                    }
                    else{
                        while(tempTwo->next != NULL){
                            tempTwo = tempTwo->next;
                        }

                        tempTwo->next = node;
                        break;
                    }

                }
                if(temp->nextL == NULL){
                    temp->nextL = node;
                    break;
                }
            }
        }


    }

    fclose(f);
    printf("READ FILE AND CREAT GRAPH........\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
}

void Dijkstra(graph head,char from[20],graph heap[100]) {

    int infinite = 100000000;

    graph temp = head;
    while(temp != NULL){               //put all the costs to infinite
        graph tempL = temp;
        while(tempL != NULL){
            tempL->cost = infinite;
            tempL = tempL->next;
        }
        temp = temp->nextL;
    }

    temp = head->nextL;
    while(strcmp(temp->From,from) != 0)   //get the node of the start
        temp = temp->nextL;


    graph tempL = temp->next;
    while(tempL != NULL){                 //change the values if all the node the have a realtion with the start
        tempL->cost = tempL->distance;
        tempL->parent = temp;
        tempL->visited = 0;
        insert(heap,tempL);
        tempL = tempL->next;
    }
    temp->parent = NULL;
    temp->visited = 1;

    while(size != 0){
        graph min = heap[0];
        deleteRoot(heap,heap[0]);     //get the smallest cost node
        min->visited = 1;

        graph getCH = head->nextL;
        while(strcmp(min->To,getCH->From) != 0)    //get her cheldrens
            getCH = getCH->nextL;

        while(getCH != NULL){                                     //change values of her cheldrens
            if(getCH->visited != 1){
                if((min->cost+getCH->distance) < getCH->cost){
                    getCH->cost = min->cost+getCH->distance;

                    getCH->parent = min;
                    insert(heap,getCH);
                }
            }
            getCH = getCH->next;
        }
    }
  }

void path(graph head,char name[20]){          //print the path
    graph temp = head->nextL;
    while(strcmp(temp->From,name) != 0){
        temp = temp->nextL;
        if(temp == NULL){
            printf("No city with this name.\n");
            break;
        }
    }

    if(temp->parent == NULL){
        printf("You are here in %s\n",temp->From);
    }
    else if(temp->parent->parent == NULL){
        printf(" (%d : %s) <-- (%d : %s)\n",temp->parent->cost,temp->From,0,temp->To);
    }
    else{
    graph tempP = temp;
    while(tempP != NULL){
        printf(" (%d : %s) <-- ",tempP->parent->cost,tempP->From);
        tempP = tempP->parent;
        if(tempP->parent == NULL){
            printf(" (%d : %s) <-- ",0,tempP->From);
            break;
            }
        else if(tempP->parent->parent == NULL){
            printf(" (%d : %s) <-- ",0,tempP->From);
            break;
        }

    }
    printf("\n");
    }
}

void pathFile(graph head,char name[20]){   //print the path in file
    FILE *f;
    f = fopen("shortest_path.txt","w");
    graph temp = head->nextL;
    while(strcmp(temp->From,name) != 0){
        temp = temp->nextL;
        if(temp == NULL){
            printf("No city with this name.\n");
            break;
        }
    }

    if(temp->parent == NULL){
        fprintf(" (%d : %s) <-- (%d : %s)",temp->parent->cost,temp->From,0,temp->To);
    }
    else if(temp->parent->parent == NULL){
        fprintf(f,"You are here in %s",temp->From);
    }
    else{
    graph tempP = temp;
    while(tempP != NULL){
        fprintf(f," (%d : %s) <-- ",tempP->parent->cost,tempP->From);
        tempP = tempP->parent;
        if(tempP->parent == NULL){
            fprintf(f," (%d : %s) <-- ",0,tempP->From);
            break;
            }

    }
    }
    fclose(f);
}

int main()
{
    graph heap[100];                             //heap array
    graph head = malloc(sizeof(struct Graph));   //the graph
    head->next = NULL;
    head->nextL = NULL;
    char source_city[20],destination_city[20];
    int choice;
    printf("THE MENU.\n1. Load cities.\n\n2. Enter source city. \n\n3. Enter destination city.\n\n4. Exit.\nEnter your choice: ");
    scanf("%d",&choice);
    while(1){
        switch (choice){
           case 1:
                read_file_and_insert(head,heap);
                break;
           case 2:
                printf("Enter the name: ");
                getchar();
                gets(source_city);
                Dijkstra(head,source_city,heap);
                break;
           case 3:
                printf("Enter the name: ");
                getchar();
                gets(destination_city);
                path(head,destination_city);
                break;
           case 4:
                pathFile(head,destination_city);
                break;
        }
        if(choice == 4)
            break;

    printf("1. Load cities.\n2. Enter source city. \n3. Enter destination city.\n4. Exit.\nEnter your choice: ");
    scanf("%d",&choice);
    }


    return 0;
}
