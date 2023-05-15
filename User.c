#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "header.h"
#include <netinet/in.h>
#define PORT 8080

void loginMenu()
{
    printf("-----------------------------------\n");
    printf("Hello and welcome to OS Market!\n");
    printf("What would you like to login as?\n");
    printf("1. Admin\n");
    printf("2. User\n");
    printf("3. Exit\n");
    printf("-----------------------------------\n");
}

void adminMenu()
{
    printf("-----------------------------------\n");
    printf("What would you like to do?\n");
    printf("1. Add Product\n");
    printf("2. Delete Product\n");
    printf("3. Update Product\n");
    printf("4. Display Products\n");
    printf("5. Log out\n");
    printf("-----------------------------------\n");
}

void userMenu()
{
    printf("-----------------------------------\n");
    printf("What would you like to do?\n");
    printf("1. View Available Products\n");
    printf("2. Add item to Cart\n");
    printf("3. Update Cart items\n");
    printf("4. View Cart\n");
    printf("5. Payment\n");
    printf("6. Log out\n");
    printf("-----------------------------------\n");
}

void displayProducts(struct Product pdt)
{
    printf("Product id : %d\t", pdt.prod_id);
    printf("Product name : %s\t", pdt.prod_name);
    printf("Quantity : %d\t", pdt.qty);
    printf("Price : %d\n", pdt.price);
    printf("----------------------------------------------------------------------\n");
}

void displayReceipt(struct Receipt rcpt)
{
    printf("Product id : %d\t", rcpt.prod_id);
    printf("Product name : %s\t", rcpt.prod_name);
    printf("Quantity : %d\t", rcpt.qty);
    printf("Price : %d\t", rcpt.price);
    printf("Payment Status : %s\t", rcpt.payment_status);
    printf("Comment : %s\n", rcpt.msg);
    printf("---------------------------------------------------------------------------------------------------------\n");
}

int main()
{
    printf("Establishing connection to server\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("Error: ");
        return -1;
    }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    // server.sin_addr.s_addr = INADDR_ANY;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5000);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Error: ");
        return -1;
    }

    int choice = 0;
    while (1)
    {
        loginMenu();
        printf("Enter choice: ");
        scanf("%d", &choice);
        write(sockfd, &choice, sizeof(int));
        if (choice == 1)
        {
            while (1)
            {
                adminMenu();
                printf("Enter choice: ");
                scanf("%d", &choice);
                write(sockfd, &choice, sizeof(int));
                if (choice == 1)
                {
                    int pid = 0, price = 0, qty = 0;
                    char pname[50];
                    while (pid < 1)
                    {
                        printf("Enter product ID: ");
                        scanf("%d", &pid);
                        if (pid < 1)
                        {
                            printf("Enter valid product ID!\n");
                        }
                    }
                    printf("Enter product name: ");
                    scanf("%s", pname);
                    while (price < 1)
                    {
                        printf("Enter price: ");
                        scanf("%d", &price);
                        if (price < 1)
                        {
                            printf("Enter valid price!\n");
                        }
                    }
                    while (qty < 1)
                    {
                        printf("Enter quantity: ");
                        scanf("%d", &qty);
                        if (qty < 1)
                        {
                            printf("Enter valid quantity!\n");
                        }
                    }
                    write(sockfd, &pid, sizeof(int));
                    // printf("Hello");
                    write(sockfd, pname, sizeof(pname));
                    // printf("Hello");
                    write(sockfd, &qty, sizeof(int));
                    // printf("Hello");
                    write(sockfd, &price, sizeof(int));
                    // printf("Hello");
                    char msg[100];
                    int rt = read(sockfd, msg, sizeof(msg));
                    printf("-----------------------------------\n");
                    write(1, &msg, rt);
                    // printf("\n");
                }
                else if (choice == 2)
                {
                    int pid = 0;
                    while (pid < 1)
                    {
                        printf("Enter product ID: ");
                        scanf("%d", &pid);
                        if (pid < 1)
                        {
                            printf("Enter valid product ID!\n");
                        }
                    }
                    write(sockfd, &pid, sizeof(int));
                    char msg[100];
                    int rt = read(sockfd, msg, sizeof(msg));
                    printf("-----------------------------------\n");
                    write(1, &msg, rt);
                    // printf("\n");
                }
                else if (choice == 3)
                {
                    int pid = 0, price = 0, qty = 0;
                    while (pid < 1)
                    {
                        printf("Enter product ID: ");
                        scanf("%d", &pid);
                        if (pid < 1)
                        {
                            printf("Enter valid product ID!\n");
                        }
                    }
                    while (price < 1)
                    {
                        printf("Enter price: ");
                        scanf("%d", &price);
                        if (price < 1)
                        {
                            printf("Enter valid price!\n");
                        }
                    }
                    while (qty < 1)
                    {
                        printf("Enter quantity: ");
                        scanf("%d", &qty);
                        if (qty < 1)
                        {
                            printf("Enter valid quantity!\n");
                        }
                    }
                    write(sockfd, &pid, sizeof(int));
                    write(sockfd, &qty, sizeof(int));
                    write(sockfd, &price, sizeof(int));
                    char msg[100];
                    int rt = read(sockfd, msg, sizeof(msg));
                    printf("----------------------------------------------------------------------\n");
                    write(1, &msg, rt);
                    // printf("\n");
                }
                else if (choice == 4)
                {
                    struct Product item;
                    while (1)
                    {
                        read(sockfd, &item, sizeof(struct Product));
                        if (item.prod_id != -1)
                        {
                            printf("----------------------------------------------------------------------\n");
                            displayProducts(item);
                        }
                        else
                        {
                            printf("\n");
                            break;
                        }
                    }
                }
                else if (choice == 5)
                {
                    break;
                }
                else
                {
                    printf("Enter valid input!!!\n");
                }
            }
        }
        else if (choice == 2)
        {
            while (1)
            {
                userMenu();
                printf("Enter choice: ");
                scanf("%d", &choice);
                write(sockfd, &choice, sizeof(int));
                if (choice == 1)
                {
                    struct Product item;
                    while (1)
                    {
                        // printf("1\n");
                        read(sockfd, &item, sizeof(struct Product));
                        if (item.prod_id != -1)
                        {
                            printf("----------------------------------------------------------------------\n");
                            displayProducts(item);
                        }
                        else
                        {
                            printf("\n");
                            break;
                        }
                    }
                }
                else if (choice == 2)
                {
                    int cust_id = -1;
                    while (cust_id < 0)
                    {
                        printf("Enter your ID(if you haven't been assigned, enter 0): ");
                        scanf("%d", &cust_id);
                        if (cust_id < 0)
                        {
                            printf("Enter valid customer ID!\n");
                        }
                    }
                    write(sockfd, &cust_id, sizeof(int));
                    if (cust_id == 0)
                    {
                        read(sockfd, &cust_id, sizeof(int));
                        printf("Your ID is: %d\n", cust_id);
                    }
                    int pid = 0, qty = 0;
                    while (pid < 1)
                    {
                        printf("Enter ID of product you want to add to cart: ");
                        scanf("%d", &pid);
                        if (pid < 1)
                        {
                            printf("Enter valid product ID!\n");
                        }
                    }
                    while (qty < 1)
                    {
                        printf("Enter qty that you want to add to cart: ");
                        scanf("%d", &qty);
                        if (qty < 1)
                        {
                            printf("Enter valid quantity!\n");
                        }
                    }

                    write(sockfd, &pid, sizeof(int));
                    write(sockfd, &qty, sizeof(int));
                    char msg[100];
                    int rt = read(sockfd, msg, sizeof(msg));
                    printf("-----------------------------------\n");
                    write(1, &msg, rt);
                    printf("\n");
                }
                else if (choice == 3)
                {
                    int cust_id = -1;
                    while (cust_id < 0)
                    {
                        printf("Enter your ID(if you haven't been assigned, enter 0): ");
                        scanf("%d", &cust_id);
                        if (cust_id < 0)
                        {
                            printf("Enter valid customer ID!\n");
                        }
                    }
                    if (cust_id == 0)
                    {
                        printf("You need to have items in your cart to update it!");
                        printf("\n");
                        continue;
                    }
                    else
                    {
                        write(sockfd, &cust_id, sizeof(int));
                        int ch;
                        printf("Would you like to update(1) or delete(2) an item?");
                        printf("Enter choice: ");
                        scanf("%d", &ch);
                        write(sockfd, &ch, sizeof(int));
                        if (ch == 1)
                        {
                            int pid = 0, qty = 0;
                            while (pid < 1)
                            {
                                printf("Enter ID of product you want to update in cart: ");
                                scanf("%d", &pid);
                                if (pid < 1)
                                {
                                    printf("Enter valid product ID!\n");
                                }
                            }
                            while (qty < 1)
                            {
                                printf("Enter qty that you want to update to: ");
                                scanf("%d", &qty);
                                if (qty < 1)
                                {
                                    printf("Enter valid quantity!\n");
                                }
                            }

                            write(sockfd, &pid, sizeof(int));
                            write(sockfd, &qty, sizeof(int));
                            // printf("Inside\n");
                        }
                        else if (ch == 2)
                        {
                            int pid = 0;
                            while (pid < 1)
                            {
                                printf("Enter ID of product you want to delete from cart: ");
                                scanf("%d", &pid);
                                if (pid < 1)
                                {
                                    printf("Enter valid product ID!\n");
                                }
                            }

                            write(sockfd, &pid, sizeof(int));
                        }
                        char msg[100];
                        int rt = read(sockfd, msg, sizeof(msg));
                        printf("-----------------------------------\n");
                        write(1, &msg, rt);
                        // printf("\n");
                    }
                }
                else if (choice == 4)
                {
                    int cust_id = -1;
                    while (cust_id < 0)
                    {
                        printf("Enter your ID(if you haven't been assigned, enter 0): ");
                        scanf("%d", &cust_id);
                        if (cust_id < 0)
                        {
                            printf("Enter valid customer ID!\n");
                        }
                    }
                    if (cust_id == 0)
                    {
                        printf("Your cart is empty!\n");
                        printf("\n");
                        continue;
                    }
                    else
                    {
                        write(sockfd, &cust_id, sizeof(int));
                        struct Product item;
                        while (1)
                        {
                            read(sockfd, &item, sizeof(struct Product));
                            if (item.prod_id != -1)
                            {
                                printf("Your cart items are as follows: ");
                                printf("----------------------------------------------------------------------\n");
                                displayProducts(item);
                            }
                            else
                            {
                                printf("End of Cart!\n");
                                printf("\n");
                                break;
                            }
                        }
                    }
                }
                else if (choice == 5)
                {
                    int total_amt = 0;
                    int cust_id = -1;
                    while (cust_id < 0)
                    {
                        printf("Enter your ID(if you haven't been assigned, enter 0): ");
                        scanf("%d", &cust_id);
                        if (cust_id < 0)
                        {
                            printf("Enter valid customer ID!\n");
                        }
                    }
                    if (cust_id == 0)
                    {
                        printf("Your cart is empty!\n");
                        printf("\n");
                        continue;
                    }
                    else
                    {
                        write(sockfd, &cust_id, sizeof(int));
                        struct Product item;
                        printf("The following are your cart items: \n");
                        struct Receipt rcpt;
                        while (1)
                        {
                            read(sockfd, &rcpt, sizeof(struct Receipt));
                            if (rcpt.prod_id != -1)
                            {
                                printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
                                displayReceipt(rcpt);
                            }
                            else
                            {
                                break;
                            }
                        }
                        read(sockfd, &total_amt, sizeof(int));
                        printf("Any items missing are due to either unavailability or due to discontinuation of the product!\n\n");
                        printf("This is your total amount due: %d\n", total_amt);
                        printf("\n");
                    }

                    while (1)
                    {
                        printf("Enter yes/no to confirm your payment: ");
                        char answer[10];
                        scanf("%s", answer);

                        if (strcmp(answer, "yes") == 0)
                        {
                            int ans = 1;
                            write(sockfd, &ans, sizeof(int));
                            struct Receipt rcpt;
                            while (1)
                            {
                                read(sockfd, &rcpt, sizeof(struct Receipt));
                                if (rcpt.prod_id != -1)
                                {
                                    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
                                    displayReceipt(rcpt);
                                }
                                else
                                {
                                    break;
                                }
                            }
                            printf("Thanks for shopping with us!\n");
                            printf("-----------------------------------\n");
                            printf("\n");
                            break;
                        }
                        else if (strcmp(answer, "no") == 0)
                        {
                            int ans = 0;
                            write(sockfd, &ans, sizeof(int));
                            printf("No worries!\n");
                            printf("-----------------------------------\n");
                            break;
                        }
                        else
                        {
                            printf("Enter valid input!\n");
                        }
                    }
                }
                else if (choice == 6)
                {
                    break;
                }
                else
                {
                    printf("Enter valid input!!!\n");
                }
            }
        }
        else if (choice == 3)
        {
            break;
        }
        else
        {
            printf("Enter valid input!!!\n");
        }
    }

    close(sockfd);
    return 1;
}