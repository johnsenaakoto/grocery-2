#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
FILE * fp2;

typedef struct bstnode_t{//type definition for node structure for binary tree
  int ID;
  char name[20];
  float balance;
  struct bstnode_t* left;
  struct bstnode_t* right;
}bstnode_t;

bstnode_t* populate_tree(char *); //populate_tree parses customer data from the customers.txt file
void InsertNode(bstnode_t**, int, char*, float); //InsertNode adds a new customer to the customer loyalty accounts
void DeleteNode(bstnode_t** root, int data); //DeleteNode removes a customer from the database
void PreOrder(bstnode_t*); //PreOrder prints the BST in the order <root>,<left>,<right>
void InOrder(bstnode_t*); //InOrder prints the BST in the order <left>, <root>,<right>
void PostOrder(bstnode_t*); //PostOrder prints the BST in the order <left>,<right>, <root>
void ProcessOrder(char*, bstnode_t**); //ProcessOrder updates the customer database by adding their current orders
void SearchandUpdate(bstnode_t**, int, float);//SearchandUpdate is a helper function that searches for and updates customer nodes
bool Search(bstnode_t* root, int data);//Search is a functions that checks if a customer exists or not, returns bool True if the customer exists in the database or bool False otherwise
bstnode_t* Min(bstnode_t* root);//Min finds the minimum value node in a tree/subtree

int main(void) {
  char filename[12];
  bstnode_t* root = NULL;
  root=populate_tree("customers.txt"); //BST is populate with data from customers.txt
  char operation[10];

  while(strcmp(operation, "quit")-sizeof(operation)!=0){ //This while loop keeps us in the system that adds, deletes, or process the BST
    int ID;
    char cust_name[20];
    char order_fname[12];
    float store_balance;
    printf("\nWhat operation do you want to conduct? add, delete, process, quit>>");
    fgets(operation, 10, stdin);

    if(strcmp(operation, "add")-sizeof(operation)==0){ // Calls the function InsertNode that adds a customer to the database
      printf("Enter customer ID:");
      scanf("%d", &ID);
      printf("Enter customer store balance:");
      scanf("%f", &store_balance);
      getchar();
      printf("Enter customer name:");
      fgets(cust_name, 20, stdin);
      int len_cust_name = strlen(cust_name);
      if(cust_name[len_cust_name-1]=='\n'){
        cust_name[len_cust_name-1]=0;
      }
      InsertNode(&root, ID, cust_name, store_balance);
    }
    else if(strcmp(operation, "delete")-sizeof(operation)==0){ //Calls the function DeleteNode, which deletes a customer from the database
      printf("Enter customer ID:");
      scanf("%d", &ID);
      getchar();
      DeleteNode(&root, ID);
    }
    else if(strcmp(operation, "process")-sizeof(operation)==0){//Calls ProcessOrder, which process orders from a text file
      printf("Enter order filename:");
      fgets(order_fname, 11, stdin);
      getchar();
      ProcessOrder(order_fname, &root);
    }
    else{
      if(strcmp(operation, "quit")-sizeof(operation)!=0){
        printf("Enter valid operation\n");
      }
    }
  }

  char print_order[5];
  printf("Please enter output format: pre, post, or in order>>"); //Requests the user to print the current BST
  scanf("%s", print_order);
  printf("Writing to newcustomers.txt in %s-order, shutting down...\n", print_order);
  fp2 = fopen("newcustomers.txt", "w");
  if(strcmp(print_order, "pre")==0){
    PreOrder(root);
  }
  else if(strcmp(print_order,"post")==0){
    PostOrder(root);
  }
  else if(strcmp(print_order,"in")==0){
    InOrder(root);
  }
  else{
    printf("Print failed! Invalid print instruction\n");
  }
  fclose(fp2);
}

void InsertNode(bstnode_t** root, int data, char* s, float rewards)//InsertNode inserts node into the BST
{
  
  if(*root==NULL){
    *root=(bstnode_t*)malloc(sizeof(bstnode_t));
    (*root)->ID=data;
    strcpy((*root)->name,s);
    (*root)->balance=rewards;
    (*root)->left=NULL;
    (*root)->right=NULL;
  }
  else{
    if (data < (*root)->ID){
      InsertNode(&(*root)->left, data, s, rewards);
    }
    else if(data > (*root)->ID){
      InsertNode(&(*root)->right, data, s, rewards);
    }
    else{
      printf("Error! ID %04d has already exists", data);
    }
  }
}

void DeleteNode(bstnode_t** root, int data){
  if(Search(*root, data)){
    if(root == NULL){
      return;
    }
    else if(data < (*root)->ID){
      DeleteNode(&(*root)->left, data);
    }
    else if(data > (*root)->ID){
      DeleteNode(&(*root)->right, data);
    }
    else{
      if((*root)->left==NULL && (*root)->right == NULL){
        free(*root);
        *root=NULL;
      }
      else if((*root)->left==NULL){
        bstnode_t* temp=*root;
        *root=(*root)->right;
        free(temp);

      }
      else if((*root)->right==NULL){
        bstnode_t* temp=*root;
        *root=(*root)->left;
        free(temp);

      }
      else{
        bstnode_t* temp=Min((*root)->right);
        (*root)->ID=temp->ID;
        DeleteNode(&(*root)->right,temp->ID);
      }
    }
  }
  else{
    printf("Error! ID does not exist in database");
  }
}

bstnode_t* Min(bstnode_t* root){
  if(root->left == NULL){
    return(root);
  }
  else{
    return(Min(root->left));
  }
}

bool Search(bstnode_t* root, int data){
  if(root == NULL) return false;
  else if(root->ID == data){
    return true;
  }
  else if(root->ID <= data){
    return Search(root->right, data);
  }
    else{
      return Search(root->left, data);
  }
}

void SearchandUpdate(bstnode_t** root, int data, float order_total){//Search finds and updates customer nodes based on orders.txt
  if(root == NULL){
    root=NULL;
  }
  else if(data==(*root)->ID){
    float balance_mod = (*root)->balance - order_total; //balance_mod holds the updated balance after subtrating orders
    if(balance_mod >= 0){
      (*root)->balance = balance_mod;
      printf("Customer %04d, %s, paid $%.2f from their store balance and $0.00 at the register, $%.2f remaining store balance\n", (*root)->ID, (*root)->name, order_total, (*root)->balance );
    }
    else if(balance_mod < 0){
      printf("Customer %04d, %s, paid $%.2f from their store balance and $%.2f at the register, $0.00 remaining store balance\n", (*root)->ID, (*root)->name, (*root)->balance, -balance_mod);
      (*root)->balance = 0;
    }
  }
  else if(data < (*root)->ID){
    SearchandUpdate(&(*root)->left, data, order_total);
  }
  else{
    SearchandUpdate(&(*root)->right, data, order_total);
  }
}


bstnode_t* populate_tree(char *filename){//populate_tree reads in data from a file and inserts it in the BST
  FILE *fp;
  int c;
  int i=0;
  int counter=0;
  int ID_temp;
  char name_temp[20];
  float balance_temp;
  bstnode_t* root;
  

 
  char temp_word[25];

  fp=fopen(filename,"r");
  if(fp==NULL){
    perror("Error in opening file");
  }

  while((c=fgetc(fp)) != EOF){
    
    if(c==44 || c == 10){
      i=0;
      if(counter == 0){
        ID_temp=atoi(temp_word);
        counter++;
      }
      else if(counter == 1){
        strcpy(name_temp,temp_word);
        counter++;
      }
      else{
        balance_temp=atof(temp_word);
        counter++;
      }
      memset(temp_word,0,strlen(temp_word));
      
    }
    else{
      if(counter == 3){
        InsertNode(&root, ID_temp, name_temp, balance_temp);
        counter=0;
      }
      if( c== 123 || c==125 || c==44 || c==34)
      {}
      else{
        temp_word[i]=c;
        temp_word[i+1]=0;
        i++;
      }
    }
  }
  balance_temp=atof(temp_word);
  InsertNode(&root, ID_temp, name_temp, balance_temp);
  fclose(fp);
  return(root);

}

void ProcessOrder(char*filename, bstnode_t**root){
  FILE *fp;
  int c;
  int i=0;
  int counter=0;
  int ID_temp;
  float order_temp;

  char temp_word[20];

  fp=fopen(filename,"r");
  if(fp==NULL){
    perror("Error in opening file");
  }

  while((c=fgetc(fp)) != EOF){
    
    if(c==44 || c == 10){
      i=0;
      if(counter == 0){
        ID_temp=atoi(temp_word);
        counter++;
      }
      else{
        order_temp=atof(temp_word);
        counter++;
      }
      memset(temp_word,0,strlen(temp_word));
      
    }
    else{
      if(counter == 2){
        SearchandUpdate(root, ID_temp, order_temp);
        counter=0;
      }
      if( c== 123 || c==125 || c==44)
      {}
      else{
        temp_word[i]=c;
        i++;
      }
    }
  }
  order_temp=atof(temp_word);
  SearchandUpdate(root, ID_temp, order_temp);
  fclose(fp);

}

void PreOrder(bstnode_t* root){//PreOrder prints the BST in the order <root>,<left>,<right>
  if(root==NULL){
    return;
  }
  else{
    fprintf(fp2, "\n%04d, %s, %.2f", root->ID, root->name, root->balance);
    PreOrder(root->left);
    PreOrder(root->right);
  }
}

void InOrder(bstnode_t* root){//InOrder prints the BST in the order <left>,<root>,<right>
  
  if(root == NULL){
    return;
  }
  else{
    InOrder(root->left);
    fprintf(fp2, "\n%04d, %s, %.2f", root->ID, root->name, root->balance);
    InOrder(root->right);
  }
  
}

void PostOrder(bstnode_t* root){//PostOrder prints the BST in the order <left>,<right>,   <root>
  if(root == NULL){
    return;
  }
  else{
    PostOrder(root->left);
    PostOrder(root->right);
    fprintf(fp2, "\n%04d, %s, %.2f", root->ID, root->name, root->balance);
    
  }
}
