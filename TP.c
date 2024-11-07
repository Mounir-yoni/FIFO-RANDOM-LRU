#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

struct Node
{
    int data;
    struct Node *next;
    struct Node *prev;
};

struct Node *head = NULL;

void fifo();
bool hit_fifo(int number_search, int *hit, int *Miss, int cash_memory[]);
void random_ex(int seq_size);
void display(int hit, int miss, int cash_memory[], int seq_size, int index_size);
void LRU(int seq_size);
void Create_new_Node(int data);
void LRU_hit(int number_search, int *hit, int *Miss, int seq_size);
void delet_node(int number_search);
void delete_tail();

int main()
{

    int seq_size, choice;
    printf("Enter the number of words in the sequence: \n");
    scanf("%i", &seq_size);

    printf("1/FIFO \n2/RANDOM \n3/LRU\n");
    scanf("%i", &choice);

    switch (choice)
    {
    case 1:
        fifo(seq_size);
        break;
    case 2:
        random_ex(seq_size);

        break;
    case 3:
        LRU(seq_size);
        break;
    default:
        break;
    }
}

void fifo(int seq_size)
{
    int temp;
    int Miss = 0;
    int Hit = 0;
    int cash_memory[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    int index_size = 0;

    printf("Enter %i numbers between 0 and 127\n", seq_size);

    for (int index = 0; index < seq_size; index++)
    {
        scanf("%i", &temp);
        if (!hit_fifo(temp, &Hit, &Miss, cash_memory))
        {
            cash_memory[index_size % 8] = temp;
            index_size += 1;
        }
    }
    display(Hit, Miss, cash_memory, seq_size, index_size);
}

bool hit_fifo(int number_search, int *hit, int *Miss, int cash_memory[])
{
    for (int index = 0; index < 8; index++)
    {
        if (number_search == cash_memory[index])
        {
            *hit += 1;
            return true;
        }
    }
    *Miss += 1;
    return false;
}

void display(int hit, int miss, int cash_memory[], int seq_size, int index_size)
{
    float hit_rate = ((float)hit / seq_size) * 100;
    float miss_rate = ((float)miss / seq_size) * 100;

    printf("The cache memory has:\n");
    for (int index = 0; index < 8; index++)
    {
        if (cash_memory[index] != -1)
        {
            printf("%i ", cash_memory[index]);
        }
    }
    printf("\nThe Hit rate is: %.2f%%\n", hit_rate);
    printf("The Miss rate is: %.2f%%\n", miss_rate);
}

void random_ex(int seq_size)
{

    int temp;
    int Miss = 0;
    int Hit = 0;
    int cash_memory[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    int index_size = 0;
    ;

    printf("enter %i number between 0-127\n", seq_size);

    for (int index = 0; index < seq_size; index++)
    {

        scanf("%i", &temp);
        if (!hit_fifo(temp, &Hit, &Miss, cash_memory))
        {

            if (index_size >= 8)
            {
                cash_memory[rand() % 8] = temp;
            }
            else
            {
                cash_memory[index_size] = temp;
                index_size += 1;
            }
        }
    }
    display(Hit, Miss, cash_memory, seq_size, index_size);
}

void LRU(int seq_size)
{
    int Miss = 0;
    int Hit = 0;
    int new_number;
    // اختبار القيم
    printf("enter %i number\n", seq_size);
    for (int index = 0; index < seq_size; index++)
    {
        scanf("%i", &new_number);
        LRU_hit(new_number, &Hit, &Miss, seq_size);
    }

    struct Node *prin = head;
    printf("Contents of the circular doubly linked list:\n");
    if (head != NULL)
    {
        do
        {
            printf("%i ", prin->data);
            prin = prin->next;
        } while (prin != head);
    }
    printf("Hit: %i, Miss: %i\n", Hit, Miss);
}

void LRU_hit(int number_search, int *hit, int *Miss, int seq_size)
{
    if (head == NULL)
    {
        Create_new_Node(number_search);
        (*Miss)++;
        return;
    }

    struct Node *comp = head;
    do
    {
        if (comp->data == number_search)
        {
            (*hit)++;

            if (comp != head)
            {
                comp->prev->next = comp->next;
                comp->next->prev = comp->prev;

                comp->next = head;
                comp->prev = head->prev;
                head->prev->next = comp;
                head->prev = comp;
                head = comp;
            }
            return;
        }
        comp = comp->next;
    } while (comp != head);

    (*Miss)++;

    if (*Miss > 8)
    {
        delete_tail();
    }

    Create_new_Node(number_search);
}

void Create_new_Node(int data)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->data = data;

    if (head == NULL)
    {
        head = new_node;
        new_node->next = head;
        new_node->prev = head;
    }
    else
    {
        new_node->next = head;
        new_node->prev = head->prev;
        head->prev->next = new_node;
        head->prev = new_node;
        head = new_node;
    }
}

void delete_tail()
{
    if (head == NULL)
        return;

    struct Node *tail = head->prev;

    if (head == tail)
    {
        free(head);
        head = NULL;
    }
    else
    {
        tail->prev->next = head;
        head->prev = tail->prev;
        free(tail);
    }
}