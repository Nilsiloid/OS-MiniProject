#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "header.h"
#define PORT 8080

void createProductFile()
{
    int fd = open("Products", O_RDWR | O_CREAT | O_EXCL, 0744);
    lseek(fd, 0, SEEK_END);
    struct Product p1;
    p1.price = -1;
    p1.prod_id = -1;
    p1.qty = -1;
    strcpy(p1.prod_name, "-1");
    write(fd, &p1, sizeof(struct Product));
    close(fd);
}

void addProduct(int PID, char pname[50], int quantity, int price, int client_fd)
{
    int fd = open("Products", O_RDWR, 0744);

    struct flock write_lock;

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;

    fcntl(fd, F_SETLKW, &write_lock);
    // printf("%d\n", fd);
    // printf("Inside\n");

    if (fd > 0)
    {
        lseek(fd, 0, SEEK_SET);
        struct Product temp;
        while (read(fd, &temp, sizeof(struct Product)))
        {
            if (temp.prod_id == PID)
            {
                write(client_fd, "Addition of product failed due to duplicate PID!\n", sizeof("Addition of product failed due to duplicate PID!\n"));
                return;
            }
        }
        lseek(fd, -sizeof(struct Product), SEEK_END);

        struct Product p1 = {PID, "", quantity, price};
        strcpy(p1.prod_name, pname);
        int flg = write(fd, &p1, sizeof(struct Product));
        // printf("Yooo\n");
        if (flg > 0)
        {
            write(client_fd, "The product has been added!\n", 29);
        }
        else
        {
            write(client_fd, "Addition of product failed!\n", 29);
        }

        p1.price = -1;
        p1.prod_id = -1;
        p1.qty = -1;
        strcpy(p1.prod_name, "-1");
        write(fd, &p1, sizeof(struct Product));
    }
    write_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &write_lock);
    close(fd);
}

void delProduct(int PID, int client_fd)
{
    int fd = open("Products", O_RDWR | O_CREAT, 0744);
    lseek(fd, 0, SEEK_SET);
    int temp = open("temp", O_RDWR | O_CREAT, 0744);
    lseek(temp, 0, SEEK_SET);
    struct flock write_lock;

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;

    fcntl(fd, F_SETLKW, &write_lock);
    fcntl(temp, F_SETLKW, &write_lock);

    struct Product data;
    int delete_flg = 0;

    while (read(fd, &data, sizeof(struct Product)))
    {
        if (data.prod_id != PID)
        {
            write(temp, &data, sizeof(struct Product));
        }
        else
        {
            delete_flg = 1;
        }
    }
    remove("Products");
    rename("temp", "Products");
    if (delete_flg == 1)
    {
        write(client_fd, "The product has been deleted!\n", 29);
    }
    else
    {
        write(client_fd, "The product doesn't exist in store!\n", sizeof("The product doesn't exist in store!\n"));
    }

    write_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &write_lock);
    fcntl(temp, F_SETLK, &write_lock);

    close(fd);
    close(temp);
}

void updateProduct(int PID, int quantity, int price, int client_fd)
{
    int fd = open("Products", O_RDWR | O_CREAT, 0744);
    lseek(fd, 0, SEEK_SET);
    struct Product item;
    struct flock write_lock;

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;

    fcntl(fd, F_SETLKW, &write_lock);

    int update_flg = 0;

    while (read(fd, &item, sizeof(struct Product)))
    {
        if (item.prod_id == PID)
        {
            update_flg = 1;
            item.qty = quantity;
            item.price = price;
            lseek(fd, -sizeof(struct Product), SEEK_CUR);
            write(fd, &item, sizeof(item));
            close(fd);
            break;
        }
    }
    if (update_flg == 1)
    {
        write(client_fd, "The product has been updated!\n", 29);
    }
    else
    {
        write(client_fd, "The product doesn't exist in store!\n", sizeof("The product doesn't exist in store!\n"));
    }

    write_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &write_lock);
    close(fd);
}

void displayProduct(int client_fd)
{
    int fd = open("Products", O_RDWR | O_CREAT, 0744);

    struct flock read_lock;

    read_lock.l_type = F_RDLCK;
    read_lock.l_whence = SEEK_SET;
    read_lock.l_start = 0;
    read_lock.l_len = 0;

    fcntl(fd, F_SETLKW, &read_lock);

    lseek(fd, 0, SEEK_SET);
    struct Product item;

    while (read(fd, &item, sizeof(struct Product)))
    {
        // read(fd, &item, sizeof(struct Product));

        if (item.prod_id == -1)
        {
            break;
        }
        else
        {
            write(client_fd, &item, sizeof(struct Product));
        }
    }

    struct Product p;
    p.prod_id = -1;
    write(client_fd, &p, sizeof(struct Product));
    read_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &read_lock);
    close(fd);
}

int createCart()
{
    int cart_fd = open("Cart", O_RDWR | O_CREAT, 0744);

    struct flock write_lock;

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;

    fcntl(cart_fd, F_SETLKW, &write_lock);

    lseek(cart_fd, (-1) * sizeof(struct Customer), SEEK_END);

    struct Customer cust;
    cust.cust_id = -1;
    read(cart_fd, &cust, sizeof(struct Customer));
    if (cust.cust_id == -1)
    {
        cust.cust_id += 1;
    }
    cust.cust_id += 1;
    for (int i = 0; i < 50; i++)
    {
        cust.cart_items[i].prod_id = -1;
        // printf("%d\n", cust.cart_items[i].prod_id);
    }
    write(cart_fd, &cust, sizeof(struct Customer));
    write_lock.l_type = F_UNLCK;
    fcntl(cart_fd, F_SETLK, &write_lock);
    close(cart_fd);
    return cust.cust_id;
}

// Edit Cart does both adding a new item to cart as well as editing the quantity of an item existing in the cart.
// We simply do this by using int update variable. If update is 0, then this function does the Add Cart functionality. If update is 1, then this function
// does the Update Cart functionality.
void EditCart(int customerID, int productID, int qty, int update, int delete, int client_fd)
{
    int pdts_fd = open("Products", O_RDONLY, 0744);
    lseek(pdts_fd, 0, SEEK_SET);

    int cart_fd = open("Cart", O_RDWR | O_CREAT, 0744);
    lseek(cart_fd, 0, SEEK_SET);

    struct flock read_lock, write_lock;

    read_lock.l_type = F_RDLCK;
    read_lock.l_whence = SEEK_SET;
    read_lock.l_start = 0;
    read_lock.l_len = 0;
    fcntl(pdts_fd, F_SETLKW, &read_lock);

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;
    fcntl(cart_fd, F_SETLKW, &write_lock);

    struct Product data;
    struct Customer cust;
    // printf("Checker1");
    while (read(pdts_fd, &data, sizeof(struct Product)))
    {
        if (productID == data.prod_id && qty <= data.qty)
        {
            // printf("Checker2");
            struct Product item = {productID, "", qty, data.price};
            strcpy(item.prod_name, data.prod_name);
            while (read(cart_fd, &cust, sizeof(struct Customer)))
            {
                if (customerID == cust.cust_id)
                {
                    // printf("Checker3");
                    for (int i = 0; i < 50; i++)
                    {
                        if (update == 0 && delete == 0 && cust.cart_items[i].prod_id == -1) // Adding a new item to cart.
                        {
                            cust.cart_items[i] = item;
                            write(client_fd, "The product has been added!\n", 27);
                            break;
                        }
                        else if (update == 1 && cust.cart_items[i].prod_id == productID) // Updating an existing item in cart.
                        {
                            // printf("Checker4");
                            cust.cart_items[i] = item;
                            write(client_fd, "The product has been updated!\n", 31);
                            break;
                        }
                        else if (delete == 1 && cust.cart_items[i].prod_id == productID)
                        {
                            cust.cart_items[i].prod_id = -1;
                            write(client_fd, "The product has been deleted!\n", 31);
                            break;
                        }
                    }
                    lseek(cart_fd, -sizeof(struct Customer), SEEK_CUR);
                    write(cart_fd, &cust, sizeof(struct Customer));

                    read_lock.l_type = F_UNLCK;
                    write_lock.l_type = F_UNLCK;
                    fcntl(pdts_fd, F_SETLK, &read_lock);
                    fcntl(cart_fd, F_SETLK, &write_lock);

                    close(cart_fd);
                    close(pdts_fd);
                    return;
                }
            }
        }
        else if (productID == data.prod_id && qty > data.qty)
        {
            // have to add socket send to tell user about not being able to add product due to less qty.
            write(client_fd, "The product has not been added/updated/deleted due to less quantity!\n", 70);
        }
    }
    write(client_fd, "The product has not been added/updated/deleted due to invalid details!\n", 72);
    // add invalid details to add to cart response.
}

void emptyCart(int customerID)
{
    int cart_fd = open("Cart", O_RDWR | O_CREAT, 0744);
    lseek(cart_fd, 0, SEEK_SET);

    struct flock write_lock;

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;

    fcntl(cart_fd, F_SETLKW, &write_lock);

    struct Customer cart;
    while (read(cart_fd, &cart, sizeof(struct Customer)))
    {
        if (cart.cust_id == customerID)
        {
            for (int i = 0; i < 50; i++)
            {
                cart.cart_items[i].prod_id = -1;
            }
        }
        lseek(cart_fd, -sizeof(struct Customer), SEEK_CUR);
        write(cart_fd, &cart, sizeof(struct Customer));
        close(cart_fd);
        return;
    }
    write_lock.l_type = F_UNLCK;
    fcntl(cart_fd, F_SETLK, &write_lock);
    close(cart_fd);
}

void displayCart(int CustomerID, int client_fd)
{
    int fd = open("Cart", O_RDWR | O_CREAT, 0744);
    lseek(fd, 0, SEEK_SET);

    struct flock read_lock;
    read_lock.l_type = F_RDLCK;
    read_lock.l_whence = SEEK_SET;
    read_lock.l_start = 0;
    read_lock.l_len = 0;
    fcntl(fd, F_SETLKW, &read_lock);

    struct Customer cart;

    while (read(fd, &cart, sizeof(struct Customer)))
    {
        if (cart.cust_id == CustomerID)
        {
            for (int i = 0; i < 50; i++)
            {
                if (cart.cart_items[i].prod_id != -1)
                {
                    write(client_fd, &cart.cart_items[i], sizeof(struct Product));
                }
                else
                {
                    write(client_fd, &cart.cart_items[i], sizeof(struct Product));
                    break;
                }
            }
        }
    }
    read_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &read_lock);
    close(fd);
}

int totalCalc(int CustomerID, int client_fd)
{
    int pdts_fd = open("Products", O_RDONLY, 0744);
    lseek(pdts_fd, 0, SEEK_SET);

    int cart_fd = open("Cart", O_RDWR | O_CREAT, 0744);
    lseek(cart_fd, 0, SEEK_SET);

    int rcpt_fd = open("Receipt", O_CREAT, 0744);
    lseek(rcpt_fd, 0, SEEK_SET);

    struct flock read_lock, write_lock;
    read_lock.l_type = F_RDLCK;
    read_lock.l_whence = SEEK_SET;
    read_lock.l_start = 0;
    read_lock.l_len = 0;
    fcntl(pdts_fd, F_SETLKW, &read_lock);
    fcntl(cart_fd, F_SETLKW, &read_lock);

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;
    fcntl(rcpt_fd, F_SETLKW, &write_lock);

    struct Product data;
    struct Customer cart;
    struct Receipt rcpt;
    int total = 0;

    // printf("Checker2");

    while (read(cart_fd, &cart, sizeof(struct Customer)))
    {
        if (cart.cust_id == CustomerID)
        {
            for (int i = 0; i < 50; i++)
            {
                lseek(pdts_fd, 0, SEEK_SET);
                if (cart.cart_items[i].prod_id != -1)
                {
                    // printf("Prod id - %d\n", cart.cart_items[i].prod_id);
                    while (read(pdts_fd, &data, sizeof(struct Product)))
                    {
                        if (cart.cart_items[i].prod_id == data.prod_id && cart.cart_items[i].qty <= data.qty)
                        {
                            total += data.price * cart.cart_items[i].qty;
                            rcpt.prod_id = cart.cart_items[i].prod_id;
                            rcpt.price = data.price;
                            rcpt.qty = cart.cart_items[i].qty;
                            rcpt.cost = data.price * cart.cart_items[i].qty;
                            strcpy(rcpt.prod_name, cart.cart_items[i].prod_name);
                            strcpy(rcpt.msg, "Quantity is same as required.");
                            strcpy(rcpt.payment_status, "Awaiting Payment.");

                            write(rcpt_fd, &rcpt, sizeof(struct Receipt));
                        }
                        else if (cart.cart_items[i].prod_id == data.prod_id && cart.cart_items[i].qty <= data.qty)
                        {
                            total += data.price * data.qty;
                            rcpt.prod_id = cart.cart_items[i].prod_id;
                            rcpt.price = data.price;
                            rcpt.qty = data.qty;
                            rcpt.cost = data.price * data.qty;
                            strcpy(rcpt.prod_name, cart.cart_items[i].prod_name);
                            strcpy(rcpt.msg, "Quantity is added as available. Total has been changed accordingly");
                            strcpy(rcpt.payment_status, "Awaiting Payment.");

                            write(rcpt_fd, &rcpt, sizeof(struct Receipt));
                        }
                    }
                }
            }
        }
    }

    // following code prints the receipt before payment.
    struct Receipt temp;
    lseek(rcpt_fd, 0, SEEK_SET);
    while (read(rcpt_fd, &temp, sizeof(struct Receipt)))
    {
        write(client_fd, &temp, sizeof(struct Receipt));
    }
    temp.prod_id = -1;
    write(client_fd, &temp, sizeof(struct Receipt));

    read_lock.l_type = F_UNLCK;
    fcntl(pdts_fd, F_SETLK, &read_lock);
    fcntl(cart_fd, F_SETLK, &read_lock);

    write_lock.l_type = F_UNLCK;
    fcntl(rcpt_fd, F_SETLK, &write_lock);

    close(pdts_fd);
    close(cart_fd);
    close(rcpt_fd);

    return total;
}

void Payment(int CustomerID, int client_fd, int payment_flg)
{
    int pdts_fd = open("Products", O_RDWR, 0744);
    lseek(pdts_fd, 0, SEEK_SET);

    int rcpt_fd = open("Receipt", O_RDWR, 0744);
    lseek(rcpt_fd, 0, SEEK_SET);

    struct flock write_lock;

    write_lock.l_type = F_WRLCK;
    write_lock.l_whence = SEEK_SET;
    write_lock.l_start = 0;
    write_lock.l_len = 0;
    fcntl(rcpt_fd, F_SETLKW, &write_lock);
    fcntl(pdts_fd, F_SETLKW, &write_lock);

    struct Product data;

    if (payment_flg == 1)
    {
        emptyCart(CustomerID);

        struct Receipt temp;
        while (read(rcpt_fd, &temp, sizeof(struct Receipt)))
        {
            strcpy(temp.payment_status, "Payment Confirmed");
            lseek(rcpt_fd, -sizeof(struct Receipt), SEEK_CUR);
            write(rcpt_fd, &temp, sizeof(struct Receipt));

            lseek(pdts_fd, 0, SEEK_SET);

            while (read(pdts_fd, &data, sizeof(struct Product)))
            {
                if (data.prod_id == temp.prod_id)
                {
                    data.qty -= temp.qty;
                    lseek(pdts_fd, -sizeof(struct Product), SEEK_CUR);
                    write(pdts_fd, &data, sizeof(struct Product));
                    break;
                }
            }
        }
    }

    // following code prints the receipt after payment.
    lseek(rcpt_fd, 0, SEEK_SET);
    struct Receipt temp;
    while (read(rcpt_fd, &temp, sizeof(struct Receipt)))
    {
        write(client_fd, &temp, sizeof(struct Receipt));
    }
    temp.prod_id = -1;
    write(client_fd, &temp, sizeof(struct Receipt));

    write_lock.l_type = F_UNLCK;
    fcntl(rcpt_fd, F_SETLK, &write_lock);
    fcntl(pdts_fd, F_SETLK, &write_lock);

    close(pdts_fd);
    close(rcpt_fd);
}

int main()
{
    createProductFile();
    int new_fd, sockfd;
    printf("Setting up the server!\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Error: ");
        return -1;
    }

    struct sockaddr_in server, client;
    server.sin_family = AF_INET;
    // server.sin_addr.s_addr = INADDR_ANY;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5000);

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Error: ");
        return -1;
    }
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("Error: ");
        return -1;
    }
    if (listen(sockfd, 5) == -1)
    {
        perror("Error: ");
        return -1;
    }
    printf("Server set up successfully!\n");

    while (1)
    {
        int sizeclient = sizeof(client);
        new_fd = accept(sockfd, (struct sockaddr *)&client, &sizeclient);
        if (new_fd == -1)
        {
            // perror("Error5: ");
            return -1;
        }
        printf("Connection has been established!\n");

        if (!fork())
        {
            close(sockfd);
            int user_type = 0;
            while (user_type >= 0)
            {
                read(new_fd, &user_type, sizeof(int));
                while (1)
                {
                    if (user_type == 1)
                    {
                        int choice;
                        read(new_fd, &choice, sizeof(int));
                        if (choice == 1)
                        {
                            int prod_id, qty, price;
                            char prod_name[50];
                            // printf("Hello\n");
                            read(new_fd, &prod_id, sizeof(int));
                            // printf("Hello\n");
                            read(new_fd, prod_name, sizeof(prod_name));
                            // printf("Hello\n");
                            read(new_fd, &qty, sizeof(int));
                            // printf("Hello\n");
                            read(new_fd, &price, sizeof(int));
                            // printf("hdashdbs");
                            addProduct(prod_id, prod_name, qty, price, new_fd);
                        }
                        else if (choice == 2)
                        {
                            int prod_id;
                            read(new_fd, &prod_id, sizeof(int));
                            delProduct(prod_id, new_fd);
                        }
                        else if (choice == 3)
                        {
                            int prod_id, qty, price;
                            read(new_fd, &prod_id, sizeof(int));
                            read(new_fd, &qty, sizeof(int));
                            read(new_fd, &price, sizeof(int));
                            updateProduct(prod_id, qty, price, new_fd);
                        }
                        else if (choice == 4)
                        {
                            displayProduct(new_fd);
                        }
                        else if (choice == 5)
                        {
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (user_type == 2)
                    {
                        int choice;
                        read(new_fd, &choice, sizeof(int));
                        if (choice == 1)
                        {
                            displayProduct(new_fd);
                        }
                        else if (choice == 2)
                        {
                            int cust_id;
                            read(new_fd, &cust_id, sizeof(int));
                            if (cust_id == 0)
                            {
                                cust_id = createCart();
                                write(new_fd, &cust_id, sizeof(int));
                            }
                            int prod_id, qty;
                            read(new_fd, &prod_id, sizeof(int));
                            read(new_fd, &qty, sizeof(int));
                            // printf("%d %d\n", prod_id, qty);
                            EditCart(cust_id, prod_id, qty, 0, 0, new_fd);
                        }
                        else if (choice == 3)
                        {
                            int cust_id;
                            read(new_fd, &cust_id, sizeof(int));
                            int ch;
                            read(new_fd, &ch, sizeof(int));
                            // printf("%d %d\n", cust_id, ch);
                            if (ch == 1)
                            {
                                int prod_id, qty;
                                read(new_fd, &prod_id, sizeof(int));
                                read(new_fd, &qty, sizeof(int));
                                // printf("Inside %d %d\n", prod_id, qty);
                                EditCart(cust_id, prod_id, qty, 1, 0, new_fd);
                            }
                            else if (ch == 2)
                            {
                                int prod_id;
                                read(new_fd, &prod_id, sizeof(int));
                                EditCart(cust_id, prod_id, 0, 0, 1, new_fd);
                            }
                        }
                        else if (choice == 4)
                        {
                            int cust_id;
                            read(new_fd, &cust_id, sizeof(int));
                            displayCart(cust_id, new_fd);
                        }
                        else if (choice == 5)
                        {
                            int cust_id;
                            read(new_fd, &cust_id, sizeof(int));
                            int total_amt = totalCalc(cust_id, new_fd);
                            write(new_fd, &total_amt, sizeof(int));

                            int payment_flg = 0;
                            read(new_fd, &payment_flg, sizeof(int));
                            Payment(cust_id, new_fd, payment_flg);
                        }
                        else if (choice == 6)
                        {
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else if (user_type == 3)
                    {
                        printf("Connection has been ended!\n");
                        exit(1);
                    }
                }
            }
            close(new_fd);
        }
        else
        {
            close(new_fd);
        }
    }
    close(new_fd);
    return 0;
}