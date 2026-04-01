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

int add_food(struct FoodEntry entry) {
  FILE *file = fopen("Food.txt", "a");

  if(!file) {
    printf("Failed to write to file: Food.txt\n");
    return 0;
  }

  fprintf(file, "%s %.2f %s\n",
        entry.name,
        entry.quantity,
        entry.unit
      );
  fclose(file);
  return 1;
}

int read_food() {
  FILE *file = fopen("Food.txt", "r");
  count = 0;

  if(!file) {
    printf("Failed to read file: Food.txt\n");
    return 0;
  }

  while(fscanf(file, "%s %f %s",
          food[count].name,
          &food[count].quantity,
          food[count].unit
        ) == 3) {
    count++;
    if(count >= MAX_ENTRIES) break;
  }
  fclose(file);

  if(count == 0) {
    printf("No entries in the file: Food.txt\n");
    return 0;
  }
    return 1;
}

void food_entry() {
  if(count >= MAX_ENTRIES) {
    printf("Memory is Full!\n");
    return;
  }
  printf("Enter Food/Snack already eaten: ");
  fgets(food[count].name, sizeof(food[count].name), stdin);
  food[count].name [strcspn(food[count].name, "\n")] = '\0';

  printf("Enter quantity: ");
  scanf("%f", &food[count].quantity);
  while(getchar() != '\n');

  printf("Enter unit (mg/g/pcs): ");
  fgets(food[count].unit, sizeof(food[count].unit), stdin);
  food[count].unit [strcspn(food[count].unit, "\n")] = '\0';

  printf("Food succesfully Added");
  printf("Name: %s\n", food[count].name);

  if(add_food(food[count])) {
    printf("Food succesfully added\n");
  } else {
    printf("Failed to add Food\n");
  };
  count++;
}

void view_entries() {
  if (!read_food()) return;

  for(int i = 0; i < count; i++) {
    printf("%d. %s %.2f %s\n",
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
