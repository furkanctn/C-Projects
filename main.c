// Furkan Cetin    150119267
// Kutay  Baskurt  150119645

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_LEN 50 

//customer structure definition
struct customer{
	char name[MAX_LEN];
	char surname[MAX_LEN];
    int id;
	struct customer *customer_next;
	struct basket *basketlist;

};
//product structure definition
struct product{
	char name[MAX_LEN];
    char category[MAX_LEN];
    int price;
	int id;
	int order;
	struct product *product_next;
};
//basket structure definition
struct basket{
	int id;
	struct basket *basket_next;
	struct productNode *productList;
	int amount;
};

struct productNode{
	int id ;
	struct productNode *next;
};

//Global startup variables
struct customer *q = NULL;
struct product *q2 = NULL;
struct basket * basket_head=NULL;
struct product *product_head= NULL;
struct customer *customer_head= NULL;

void productInsert(struct product *std);
void customerInsert(struct customer *std);

void print_product(struct product *std2,int count) {

	printf("\t%d. %s %s\t%d\n",count, std2->name, std2->category, std2->price);
}
void print_customer(struct customer *std,int count) {

	printf("\t%d. %s %s\n",count, std->name, std->surname);
}
//Reading function of the product.txt file
void read_productFile(char *input) {

	FILE *fp;
	char name[MAX_LEN], type[MAX_LEN];
	int amount;
	int order;
	fp = fopen("product.txt", "r");

	while ( fscanf(fp, "%d %s %s\t %d",&order, name, type, &amount) != EOF ) {

		struct product *std1 = malloc(sizeof(struct product));

		if (std1 == NULL) {

			puts("Memory allocation failed!");
			exit(-1);
		}

        std1->id = order;
		strcpy(std1->name, name);
		strcpy(std1->category, type);
		std1->price = amount;
		
		std1->product_next = NULL;
		
		productInsert(std1);
	}	
	fclose(fp);
}
//Reading function of the customer.txt file
void read_customerFile(char *input){

	FILE *fp2;
	char name[MAX_LEN], surname[MAX_LEN];
	int order;
	fp2 = fopen("customer.txt", "r");

	while ( fscanf(fp2,"%d %s %s\t ",&order, name,surname) != EOF ) {

		struct customer *std = malloc(sizeof(struct customer));
		if (std == NULL) {

			puts("Memory allocation failed!");
			exit(-1);
		}
        std->id = order;
		strcpy(std->name, name);
		strcpy(std->surname, surname);
		
		std->customer_next = NULL;
		
		customerInsert(std);
	}	
	fclose(fp2);
}
//Reading function of the basket.txt file
void read_BasketFile(char *input){

	FILE *basketFile;
	int customerid,basketid,productid;
	basketFile = fopen("basket.txt", "r");

	while ( fscanf(basketFile, "%d %d %d\t ",&customerid,&basketid,&productid) != EOF ) {

        struct customer* current_customer = customer_head;

        // finds the correct customer
		while(current_customer != NULL && current_customer->id != customerid){
			current_customer=current_customer->customer_next;
		}            
        // finds basket
        struct basket* current_basket = current_customer->basketlist;
        while (current_basket != NULL && current_basket->id != basketid)
            current_basket = current_basket->basket_next;

        if (current_basket == NULL) {
            struct basket* newbasket = malloc(sizeof (struct basket));
            newbasket->id = basketid;

            struct productNode* newproductnode = malloc(sizeof (struct productNode));
            newproductnode->id = productid;
            newbasket->productList = newproductnode;
			current_customer->basketlist = newbasket;
        }

        else {
            struct productNode* newproductnode = malloc(sizeof (struct productNode));
            newproductnode->id = productid;

            // finds productnode
            struct productNode *currentproductnode = current_basket->productList;
            while (currentproductnode->next != NULL) {
                currentproductnode = currentproductnode->next;
            }
            currentproductnode->next = newproductnode;
        }

	}	
	fclose(basketFile);

}
//this function print products
void printProduct(void) {
	struct product *current;
	current = product_head->product_next;
    int count = 1;
	puts("The list in name-alphabetical order:");
	while (current->product_next != NULL) {
		print_product(current,count++);
		current = current->product_next;
	}
	print_product(current,count);
}
//this function print customers
void printCustomer() {

	struct customer *current;
	current = customer_head->customer_next;
    int count = 1;
	puts("The list in customer name-alphabetical order:");
	while (current->customer_next != NULL) {
		print_customer(current,count++);
		current = current->customer_next;
	}
	print_customer(current,count);
}
//putting the data taken from the file into the product structure
void productInsert(struct product *std){

    struct product *previousPtr =NULL;
    struct product *currentPtr = product_head->product_next;

    while(currentPtr != NULL && strcmp(std->name,currentPtr->name)>0){
        previousPtr =currentPtr;
        currentPtr = currentPtr->product_next;
    }
    if(previousPtr ==NULL){
        std->product_next = product_head->product_next;
        product_head->product_next=std;
    }
    else{
        
        previousPtr->product_next=std;
        std->product_next = currentPtr;
    }
}
//putting the data taken from the file into the customer structure
void customerInsert(struct customer *std1){

    struct customer *previousPtr =NULL;
    struct customer *currentPtr = customer_head->customer_next;

    while(currentPtr != NULL && strcmp(std1->name,currentPtr->name)>0){

        previousPtr =currentPtr;
        currentPtr = currentPtr->customer_next;
    }
    if(previousPtr ==NULL){

        std1->customer_next = customer_head->customer_next;
        customer_head->customer_next=std1;
    }
    else{
        
        previousPtr->customer_next=std1;
        std1->customer_next = currentPtr;
    }
}
//add new customer function
void add_customer(char *name ,char *surname){

	struct customer *eklenecek =malloc(sizeof(struct customer));
	strcpy(eklenecek->name,name);
	strcpy(eklenecek->surname,surname);
	eklenecek->customer_next=NULL;

	q =customer_head;
	while(q->customer_next!=NULL){

		q=q->customer_next;
	}	
	q->customer_next = eklenecek;

}
//delete customer function
void removeCustomer(char*name,char*surname){

	struct customer *current = NULL;
	q =customer_head;
	while(q->customer_next!=NULL && strcmp(q->customer_next->name,name)!=0){
		q=q->customer_next;
	}
	current = q->customer_next;
	q->customer_next = q->customer_next->customer_next;
	free(current);
}

//Creating a new basket with user-received information
struct basket *createBasket(int *id, int *amount){

	struct basket *newBasket = malloc(sizeof(struct basket));
	newBasket->id =*id;
	newBasket->amount =*amount;
	newBasket->basket_next =NULL;
	newBasket->productList = NULL;
	return newBasket;
}
//This function if does not find a basket, create new basket.
struct customer *findCustById(int *givenID ,int *amount){

	struct customer *customerPt =customer_head;
	struct product *productPt =product_head;
	while(customerPt !=NULL){
		if(customerPt->id == *givenID){

			printf("Customer id = %d , Customer name %s finded!!\n",customerPt->id,customerPt->name);

			if(customerPt->basketlist ==NULL){

				printf("Customer basket is not available creating now...\n");

                struct basket *createBasketForCust = createBasket(&customerPt->id , amount);
                customerPt->basketlist = createBasketForCust;
                break;
			}
			struct basket *createBasketForCust = createBasket(&customerPt->id , amount);
			customerPt->basketlist = createBasketForCust;
			break;
	}
		customerPt =customerPt->customer_next;
	}
	return customerPt;
}
//this function prints the customers who bought the product
struct customer *printshopper(int *givenProductID ){

	struct customer *customerPt =customer_head->customer_next;
	struct productNode *productPt;
	struct basket *basketPtr;

	while(customerPt !=NULL){

        basketPtr = customerPt->basketlist;

		while(basketPtr!=NULL){

			productPt = basketPtr->productList;

			while(productPt!=NULL){

				if(productPt->id == *givenProductID){

					printf("customer : %s\n",customerPt->name);
				}
				productPt = productPt->next;
			}
			basketPtr = basketPtr->basket_next;
		}
		customerPt = customerPt->customer_next ;
	}
    
}
void printTotallAmount() {
	
	int basketId;
	int prevId;
	int totalAmount;
	
	struct customer *customerPt = customer_head;
	
	while(customerPt != NULL){
		
		struct basket *basketPt = customerPt->basketlist;
		
		prevId = 0;
		totalAmount = 0;
		
		while (basketPt != NULL){
			
			basketId = basketPt->id;
			
			if(basketId != prevId){
				prevId = basketId;
				totalAmount += basketPt->amount;
			}

			basketPt = basketPt->basket_next;
		}
		
		printf("\n%d.  %s %s  \t   %d\n", customerPt->id, customerPt->name, customerPt->surname, totalAmount);
		
		customerPt = customerPt->customer_next;
	}
   
}
int main(int argc, char *argv[]) {

	char name[MAX_LEN],surname[MAX_LEN];
	int choice;	
	int selection;
	int prodc,prodc2;
	int exit = 1;
	
	product_head = malloc(sizeof(struct product));
	product_head->product_next = NULL;

	customer_head = malloc(sizeof(struct customer));
	customer_head->customer_next = NULL;

	int count =0;

	
	read_productFile(argv[0]);
	read_customerFile(argv[1]);
	read_BasketFile(argv[0]);


	printProduct();
	printf("\n");
	printCustomer();
	
	while(exit ==1){

		printf("\n1- Add Customer\n");
		printf("2- Remove Customer\n");
		printf("3- Add Basket\n");
		printf("4- List the customers who bought a specific product:\n");
		printf("5- List the total shopping amounts of each customer:\n");
		printf("6- Exit\n");
		printf("please select a choice\n");
		scanf("%d",&choice);

		switch (choice){
			
		case 1:
			printCustomer();

			printf("Please enter your name and surname: ");
			scanf("%s %s",name,surname);

			q =customer_head;

			while(q->customer_next!=NULL){

				q=q->customer_next;

				if(strcmp(q->name,name)==0){

					printf(" Unfortunately there is a customer with this name \n");
					break;
				}
				else if(q->customer_next == NULL){

					add_customer(name,surname);
					printf("Customer added..\n");
					printCustomer();
					break;
				}	
			}
			break;	
		case 2:
			printCustomer();
			printf("Please enter your name and surname: ");
			scanf("%s %s",name,surname);

			q = customer_head;

			while(q->customer_next !=NULL){

				q=q->customer_next;
				if(strcmp(q->name,name) == 0){

					removeCustomer(name,surname);
					printf("Customer is deleted..\n");
					printCustomer();
					break;
				}
				else if(q->customer_next == NULL){

					printf("Customer not found.. \n");
					break;
				}
			}
		case 3:
			printCustomer();

			printf("please select a customer:");
			scanf("%d",&selection);

			printProduct();

			printf("please select a product:");
			scanf("%d",&prodc);

			q2 = product_head;
			int i;
			for( i = 0; i< prodc; i++){
				q2 =q2->product_next;
			}
			prodc2 =q2->price;
			struct customer *findedCustomer = findCustById(&selection,&prodc2);
			printf("finded Customer basket id = %d , finded Customer basket amount = %d ",findedCustomer->basketlist->id , findedCustomer->basketlist->amount);
			break;
		case 4:
			printProduct();
			printf("please select a product: \n");
			scanf("%d",&selection);
			struct customer *findedproduct = printshopper(&selection);
			break;

		case 5:
			printTotallAmount();
			break;

		
		case 6:
			exit =0;
			printf("Good BYE..\n");

		}
	}	
}
