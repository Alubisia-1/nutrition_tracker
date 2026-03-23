#include <stdio.h>
#include <string.h>


void display_menu() {
    printf("=== Nutrion Tracker ===\n"
          "1. Add Food Entry\n"
          "2. View Entries\n"
          "3. Exit\n"
      );
}

int get_choice() {
  int choice;
  printf("Chose a Menu: 1,2,3\n");
  scanf("%d", &choice);
  getchar();
  return choice;
}

void food_entry() {
  char food[50];

  printf("Enter Food/Snack already eaten: ");
  fgets(food, sizeof(food), stdin);
  food [strcspn(food, "\n")] = '\0';

  printf("You entered: %s\n", food);
}

void view_entries() {
  printf("No Entries yet!\n");
}

int main() {
  int choice;
  while (1) {
    display_menu();

    choice = get_choice();
  
   if(choice == 1) {
     food_entry();
   } else if (choice == 2) {
     view_entries();
   } else if(choice == 3) {
     printf("Exiting...!\n");
     break;
   } else {
     printf("Invalid Choice!");
   }
  }
 
  return 0;
}
