#include <stdio.h>
#include <string.h>

#define MAX_ENTRIES 100

struct FoodEntry {
  char name[50];
  float quantity;
  char unit[10]; // "ml" "g" "mg"
};

struct FoodEntry food[MAX_ENTRIES];
int count = 0;


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
  if(count >= MAX_ENTRIES) {
    printf("Memory is Full!\n");
  }
  printf("Enter Food/Snack already eaten: ");
  fgets(food[count].name, sizeof(food[count].name), stdin);
  food[count].name [strcspn(food[count].name, "\n")] = '\0';

  printf("Enter quantity: ");
  scanf("%f", &food[count].quantity);
  while(getchar() != '\n');

  printf("Enter unit (mg/g/pcs): ");
  fgets(food[count].unit, sizeof(food[count].unit), stdin);
  food[count].unit [strcspn(food[count].name, "\n")] = '\0';

  printf("Food succesfully Added");
  printf("Name: %s\n", food[count].name);
  count++;
}

void view_entries() {
  if (count == 0) {
    printf("No Entries!");
    return;
  }
  for(int i = 0; i < count; i++) {
    printf("%d.%s %.2f %s\n",
      i + 1,
      food[i].name,
      food[i].quantity,
      food[i].unit
    );
  }
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
