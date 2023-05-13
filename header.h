#include <stdio.h>

struct Product
{
    int prod_id;
    char prod_name[50];
    int qty;
    int price;
};

struct Customer
{
    int cust_id;
    struct Product cart_items[50];
};

struct Receipt
{
    int prod_id;
    char prod_name[50];
    int qty;
    int price;
    int cost;
    char payment_status[20];
    char msg[100];
};