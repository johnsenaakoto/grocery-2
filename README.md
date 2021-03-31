# grocery-2
This is a system that stores customer loyalty accounts at a grocery store, then processes basic orders for said customers.

 Customers’ loyalty accounts will have the following:
 
 ● ID
  
  ○ A unique 4 digit ID number (0-9999). Do not worry about trailing zero
 
 ● Name 
  
  ○ The customer’s name (these do not have to be unique).
 
 ● Store Balance 
  
  ○ Amount of store credit they have accrued.


1. add​ ​num name balance a. int - ​num
​ is a valid, unique ​ID
​ ​number as detailed on page 1. b. string - ​name
​ is the ​name 
​ of the customer c. float - ​balance
​ is the amount (in $) of ​Store Balance
​ the customer will have 


2. delete ​num  a. int - ​num
​ is the ​ID
​ ​number of an existing customer as detailed on page 1. 


3. process ​filename a. string - ​filename 
​ is the name of the file which contains the orders as detailed in step 3 of page 1. 4. quit a. This will prompt a message “​Please enter output format: pre, post, or in order​” b. User should enter ​“pre”​, ​“post”​, or ​“in”​ to indicate the order they want. c. Please add trailing zeros to the customer’s ​ID
​ when printing to “newcustomers.txt”. ​I.e. 12 -> 0012 
