#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>

using namespace std;

//struct named Inventory to use throughout code
struct Inventory{
  string part_name;
  string manufacturer;
  int on_hand_quantity;
  int min_quantity;
  double unit_price;
  double total_value;
};

//Declaring function prototypes
int DisplayMenu();
int ReadPartInfo(Inventory [], double &);
void DisplayInventoryReport(Inventory [], int, double & );
void DisplayReorderReport(Inventory [], int, double &);
void SellPart(Inventory [], double&, int);
void OrderPart(Inventory [], double &, int);
int ReturnUnderStockedIndex(Inventory [], int, int);

//main function 
int main(){
  Inventory arr[150];
  double balance = 0;
  int count = ReadPartInfo(arr, balance);
  int choice = 0;
  choice = DisplayMenu();
  //switch statement for the choice selection
  while(choice != 5){
    switch(choice){
    case 1:
      DisplayInventoryReport(arr, count, balance);
      break;
    case 2:
      DisplayReorderReport(arr, count, balance);
      break;
    case 3:
      SellPart(arr, balance, count);
      break;
    case 4:
      OrderPart(arr, balance, count);
      break;
    }
    cout << endl;
    //Displaying menu after each choice
    choice = DisplayMenu();
  }
  return 0;
}

//Reading from file and sending to struct array 
int ReadPartInfo(Inventory arr[], double& balance){
  ifstream myFile ;
  myFile.open("inventory.dat");
  //Validating file 
  if(!myFile){
    cout << "File not opened";
    return 0;   
  }
  
  int n = 0;
  myFile>> n >> balance;
  int count = 0;

  while(myFile>>arr[count].part_name){
    myFile >> arr[count].manufacturer>> arr[count].on_hand_quantity>> arr[count].min_quantity>> arr[count].unit_price;
    
    count++;
  }
  return count;
}

//Displays the menu for user to pick which action they want to take
int DisplayMenu(){
  cout << endl;
  cout << "============================================"<<endl;
  cout << "   Mini Electronics Inventory Management"<<endl;
  cout<<"1:Inventory Report \n2:Reorder Report \n3:Sell Parts \n4:Order Parts \n5:Quit\n";
  cout << "============================================"<<endl;
  cout <<"Your Choice [1-5]: ";
  int choice;
  cin >> choice;
  //Validating choice 
  if(choice < 1 || choice > 5){
    cin >> choice;
  }
   
  return choice;
}

//Display the inventory using the struct array
void DisplayInventoryReport(Inventory arr[], int size, double & balance){
  int total_quantity = 0;
  double total_value = 0;
  cout <<"-------------------------------------------- Inventory Report";
 cout <<"--------------------------------------------"<<endl;
  cout << setw(20) << left << "Item" << setw(20) << left << "Manufacturer"; 
  cout << setw(20) << left << "Quantity" << setw(20) << left<< "Unit Price ($)";
  cout << setw(20) << left << "Total Value ($)" << endl;
  for(int i = 0; i < size; i ++){
    cout << setw(20) << left << arr[i].part_name << setw(20) << left
         << arr[i].manufacturer;
    cout << setw(20) << left << arr[i].on_hand_quantity << setw(20) << left
         << arr[i].unit_price;

    //Calculating values
    total_quantity += arr[i].on_hand_quantity;
    total_value += arr[i].unit_price * arr[i].on_hand_quantity;
    arr[i].total_value = arr[i].unit_price * arr[i].on_hand_quantity;
    cout << setw(20) << left << arr[i].total_value << endl;
  }

  //Output account and inventory information
  cout << "Number of parts: " << size << endl;
  cout << "Total Quantity: " << total_quantity << endl;
  cout << "Total Value: " << total_value << endl;
  cout << "Bank Balance: " << balance << endl;
 
}
  
//Finds the parts that need to be re-ordered based on current and minimum quantity
void DisplayReorderReport(Inventory arr[], int size, double & balance){
	int parts_to_order = 0;
	int understocked = 0;
    double total_cost = 0;
    cout << "-------------------------------------- Reorder Report";
    cout <<"---------------------------------------"<<endl;
    cout << setw(20) << left << "Item" << setw(20) << left << "Manufacturer";
    cout << setw(20) << left << "Quantity" << setw(20) <<left<< "Min Quantity";
    cout << setw(20) << left << "Unit Price ($)" << endl;
	for(int i = 0; i < size; i++){
		if(arr[i].on_hand_quantity < arr[i].min_quantity){
          cout << setw(20) << left << arr[i].part_name << setw(20)
               << left << arr[i].manufacturer;
          cout << setw(20) << left << arr[i].on_hand_quantity << setw(20)
               << left << arr[i].min_quantity;
          cout << setw(20) << left << arr[i].unit_price << endl;

          understocked ++;
          parts_to_order += arr[i].min_quantity - arr[i].on_hand_quantity; 
          total_cost += 0.8*(arr[i].unit_price*(arr[i].min_quantity
                                                - arr[i].on_hand_quantity));            
        }
    }
    cout << "Number of understocked parts: " << understocked << endl;
    cout << "Total number to order : " << parts_to_order << endl;
    cout << "Total Cost ($): " << total_cost << endl;
    cout << "Bank Balance ($): " << balance << endl;
    
}

//Selling the parts as long as there are enough in inventory
void SellPart(Inventory arr[], double & balance, int count){
  string input_name = " ";
  string input_manu = " ";
  int input_quantity = 0;

  //Variable to check if the input is valid
  bool found = false;
  
  cout << "Input the part name: " << endl;
  cin >> input_name;
  
  int n = 0;

  //Checking if the part_name entered is found and saving the index
  while(!found){
    for(int i = 0; i < count; i++){
      if(input_name == arr[i].part_name){
        found = true;
        n = i;
      }
    }

    //if the inputted part_name is not found, asking user to re-enter input
    if(found == false){
      cout << "Invalid part name!" << endl;
      cin >> input_name;    
    }
  } 
  cout << "Input the part manufacturer: " << endl;
  cin >> input_manu;
  //checking if manufacuter matches at the index that the part name is at
  while(input_manu != arr[n].manufacturer){
    cout << "Invalid manufacturer!" << endl;
    cin >> input_manu;
  }
  //Getting the quantity and the maximum quantity that can be sold
  int max_quantity = arr[n].on_hand_quantity;
  cout << "Input the quantity: [0 - "<< max_quantity << "]: " << endl;
  cin >> input_quantity;
  while(input_quantity >= arr[n].on_hand_quantity || input_quantity <= 0){
    cout << "Input the quantity [0 - "<< max_quantity << "]: " << endl;
    cin >> input_quantity;
  }
  //Updating quantity, total value, and balance
  arr[n].on_hand_quantity -= input_quantity;
  arr[n].total_value -= input_quantity * arr[n].unit_price;
  balance += input_quantity * arr[n].unit_price;
}

//Function called when ordering understocked or inputted parts
void OrderPart(Inventory arr[], double& balance, int count){
  //Creating new struct array to store understocked data
  Inventory understocked[150];
  int num_of_understocked = 0;
  int under_index = 0;
  int arr_index = 0;
  for(int i = 0; i < count; i++){
    if(arr[i].on_hand_quantity < arr[i].min_quantity){
      understocked[under_index].part_name = arr[i].part_name;
      understocked[under_index].manufacturer = arr[i].manufacturer;
      understocked[under_index].min_quantity = arr[i].min_quantity;
      understocked[under_index].on_hand_quantity = arr[i].on_hand_quantity;
      understocked[under_index].unit_price = arr[i].unit_price;
      num_of_understocked++;
      under_index++;
    }
  }
  //Executes if statement if there exists items that are understocked
  if(num_of_understocked > 0){
    //calls function to return index of understocked part to reorder
    int index = ReturnUnderStockedIndex(understocked, count, num_of_understocked);
    cout << "Order part " << understocked[index].part_name << " from "
         << understocked[index].manufacturer << endl;
    //Calculating maximum number to order
    //Ordering price is 20% less than selling price
    //Assuming unit price is selling price
    int num_can_order = balance / (understocked[index].unit_price*0.8);
    int num_to_order = 0;
    cout << "You may order quantity [0 - " << num_can_order << "]: " << endl;
    cin >> num_to_order;
    //validating input
    if(num_to_order < 0 || num_to_order > num_can_order){
      cin >> num_to_order;
    }
    for(int p = 0; p < count; p ++){
      if(understocked[index].part_name == arr[p].part_name){
        arr_index = p;
      }
    }
    //Updating balance and quantity
    arr[arr_index].on_hand_quantity += num_to_order;
    balance -= num_to_order * understocked[index].unit_price*0.8;
  }
  //else statement execuses if there are no understocked parts
  else{
    string input_name = " ";
    string input_manu = " ";
     int input_amount = 0;
    cout << "Enter part name: " << endl;
    cin >> input_name;
    bool found = false;
    int n = 0;
	while(!found){
      for(int i = 0; i < count; i++){
        if(input_name == arr[i].part_name){
          found = true;
          n = i;
        }
      }
    }
    if(found == false){
      cout << "Invalid part name!" << endl;
      cin >> input_name;
    }
    cout << "Input the part manufacturer: " << endl;
    cin >> input_manu;
    while(input_manu != arr[n].manufacturer){
      cout << "Invalid manufacturer!" << endl;
      cin >> input_manu;
    }
    cout << "Input the quantity you wish to order: " << endl;
    cin >> input_amount;
    while((balance/arr[n].unit_price*0.8) < input_amount){
      cout << "Input a smaller quantity: " << endl;
      cin >> input_amount;
    }
    arr[n].on_hand_quantity += input_amount;
    balance -= input_amount * arr[n].unit_price*0.8;
  }
}

//Returns index of the most understocked part
int ReturnUnderStockedIndex (Inventory understocked[], int count,
                             int num_of_understocked){
  int highest_index = 0;
  //checks if there is a need to compare parts
  if(num_of_understocked==1){
    return 0;
  }
  //if there are more than one parts, else statement executes
  else{
    for(int k = 0; k < num_of_understocked; k++){
      if (((understocked[k].min_quantity-understocked[k].on_hand_quantity)*
           understocked[k+1].min_quantity) <
          ((understocked[k+1].min_quantity-understocked[k+1].on_hand_quantity)*
           understocked[k].min_quantity)){
        highest_index +=1;
      }
    
      else if (((understocked[k].min_quantity-understocked[k].on_hand_quantity)
                *understocked[k+1].min_quantity) ==
               ((understocked[k+1].min_quantity
                 -understocked[k+1].on_hand_quantity)*understocked[k].min_quantity)){
        if (understocked[k].part_name < understocked[k+1].part_name)
          highest_index += 1;
        else{
          if (understocked[k].manufacturer < understocked[k+1].manufacturer)
            highest_index += 1;
        }
      }
    }
  }
  return highest_index;
}
