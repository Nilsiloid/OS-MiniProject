# OS-MiniProject
Project implementation for Operating Systems Course. Implements an E-Commerce system using socket programming and file locking.

# User Menu:
Any user of this E-commerce system can login as either one of the following:
1. Admin User - the Admin has the permissions and capabilities of adding new products to the list of available products and to update/delete currently existing products.
2. User - the regular user(or customer) has access to view products that are available, the products that they have added to the cart and any additions/updates/deletions they would like to do in their cart. They can also review their items and pay for the products in their cart.

# Functionalities of the Admin
1. Addition of a Product.
2. Delete a Product.
3. Update price and/or quantity of an existing Product.
4. View all Products that are existing in the system.

# Functionalities of the User
1. View all Available Products.
2. Add Product to Cart.
3. Update Products in Cart(this includes both changing quantity(updation) and removing an item).
4. View Products that are in Cart.
5. Payment of Cart items.

# Architecture
The implementation invloves the use of 2 main programs - Server.c and User.c.
The file Server.c implements the Server side whereas User.c implements Client side. These 2 files are connected via a socket and this socket is then used for communication among the 2 files.
During access of files that contain data about the available products, cart items or a customer's receipt - I have used file locking to ensure correct data is being read.
There are 3 other files used in the implementation - 
1. Products - this file stores the available products. It stores each product as a structure containing the following - productID, productName, price and quantity.
2. Cart - this file stores the products in a customer's cart along with the customerID. It stores each cart as a structure containing the following - customerID, array of 50 Product structures.
3. Receipt - this file stores a customer's cart(and their customerID) with the quantity as per availability. If the available quantity is less than that mentioned in the cart, then the quantity mentioned in receipt is reduced to fit the need.
The Receipt file also stores the payment status and a message regarding the quantity of products.

# How to run the Project?
1. Open a new terminal and type the following:
```bash
gcc Server.c -o Server
```
This will create an executable file called Server that we can then run to execute the Server side.

```bash
./Server
```
This will run the created executable - Server.

2. Open another separate new terminal and use the following command:
```bash
gcc User.c -o User
```
This will create an executable file called User that we can then run to execute the Use side.

```bash
./User
```
This will run the created executable - User. This will then connect to the socket on server side and we can then start using the functionalities provided for the E-commerce system.

Once we run the ./User command, we will get prompts on the terminal running the User executable and thus, we have successfully run the code.

For more details on the Project, kindly refer to the [Report](https://github.com/Nilsiloid/OS-MiniProject/blob/master/Report.pdf)
